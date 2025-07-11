#include "balance_ctrl.h"

#include <math.h>
#include <stdbool.h>

#include "balance_ctrl_turn.h"
#include "mpu6050.h"
#include "imu_filter.h"
#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include "i2c.h"
#include "control_state.h"
#include "tim.h"

#define PWM_LIMIT       1000
#define YAW_PWM_LIMIT   400  // 原地转向最大差速 PWM

static PID_TypeDef pid_pitch;  // 俯仰角 PID（外环）
static PID_TypeDef pid_speed;  // 速度 PID（内环）
static PID_TypeDef pid_yaw;    // 偏航角 PID（转向位置环）

// 判断是否是“蓝牙控制原地转向模式”
static bool IsYawOnlyMode(void) {
    return (ControlState_GetMode() == MODE_BT &&
           (ControlState_GetMotion() == MOTION_LEFT ||
            ControlState_GetMotion() == MOTION_RIGHT));
}

void BalanceCtrl_Init(void) {
    HAL_TIM_Base_Start_IT(&htim6);  // 启动主控循环定时器

    PID_Init(&pid_pitch, 22.0f, 0.6f, 0.8f);
    PID_Init(&pid_speed, 1.2f, 0.01f, 0.03f);
    PID_Init(&pid_yaw,   2.5f, 0.02f, 0.1f);  // 控制 Yaw 到达目标角度
}

void BalanceCtrl_Loop(void) {
    // === 1. 读取 MPU6050 数据 ===
    MPU6050_Read_All(&hi2c1);

    // === 2. 当前偏航角（Yaw） ===
    float yaw_angle = IMU_GetYaw();  // 单位：deg

    // === A. 原地转向模式 ===
    if (IsYawOnlyMode()) {
        float target_yaw_angle = ControlState_GetTargetYawAngle();
        float yaw_pwm = PID_Compute(&pid_yaw, target_yaw_angle, yaw_angle);

        if (yaw_pwm > YAW_PWM_LIMIT) yaw_pwm = YAW_PWM_LIMIT;
        if (yaw_pwm < -YAW_PWM_LIMIT) yaw_pwm = -YAW_PWM_LIMIT;

        // 原地差速输出
        Motor_SetPWM((int)(-yaw_pwm), (int)(yaw_pwm));
        return;
    }

    // === B. 正常平衡运动 ===

    // === 3. 当前俯仰角（Pitch）姿态解算 ===
    float acc_pitch = atan2f((float)mpu_data.ay, (float)mpu_data.az) * 57.3f;
    float pitch_gyro_rate = (float)mpu_data.gx / 131.0f;  // X轴角速度（deg/s）
    float gyro_yaw = (float)mpu_data.gz / 131.0f;
    IMU_Filter_Update(acc_pitch, pitch_gyro_rate, gyro_yaw);
    float pitch_angle = IMU_GetPitch();  // 当前姿态角，单位 deg

    // === 4. 外环：俯仰角 PID → 目标速度 ===
    float target_pitch_angle = ControlState_GetTargetAngle();
    float target_speed = PID_Compute(&pid_pitch, target_pitch_angle, pitch_angle);

    // === 5. 编码器测量速度 ===
    Encoder_Update();
    float left_speed = (float)Encoder_GetSpeed_Left();
    float right_speed = (float)Encoder_GetSpeed_Right();
    float actual_speed = (left_speed + right_speed) / 2.0f;

    // === 6. 内环：速度 PID → PWM 输出 ===
    float pwm = PID_Compute(&pid_speed, target_speed, actual_speed);

    if (pwm > PWM_LIMIT) pwm = PWM_LIMIT;
    if (pwm < -PWM_LIMIT) pwm = -PWM_LIMIT;

    float left_pwm = pwm;
    float right_pwm = pwm;

    // === 7. 若为循迹或其他模式，应用差速调节 ===
    BalanceCtrl_ApplyTurn(&left_pwm, &right_pwm);

    // === 8. 输出到电机 ===
    Motor_SetPWM((int)left_pwm, (int)right_pwm);
}
