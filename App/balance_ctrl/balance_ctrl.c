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
#define YAW_PWM_LIMIT   400  // 偏航控制最大输出

static PID_TypeDef pid_angle;
static PID_TypeDef pid_speed;
static PID_TypeDef pid_yaw;

// 判断是否为“原地转向模式”
static bool IsYawOnlyMode(void) {
    return (ControlState_GetMode() == MODE_BT &&
           (ControlState_GetMotion() == MOTION_LEFT ||
            ControlState_GetMotion() == MOTION_RIGHT));
}

void BalanceCtrl_Init(void) {
    HAL_TIM_Base_Start_IT(&htim6);  // 控制循环定时器
    PID_Init(&pid_angle, 22.0f, 0.6f, 0.8f);    // 外环：角度控制
    PID_Init(&pid_speed, 1.2f, 0.01f, 0.03f);   // 内环：速度控制
    PID_Init(&pid_yaw, 5.0f, 0.0f, 0.2f);       // 偏航角速度控制
}

void BalanceCtrl_Loop(void) {
    // 1. 读取 IMU 数据
    MPU6050_Read_All(&hi2c1);

    // 2. 提取当前偏航角速度（单位：deg/s）
    float current_yaw_rate = (float)mpu_data.gz / 131.0f;

    // ----------------------------
    // A. 原地转向模式（蓝牙控制转弯）
    // ----------------------------
    if (IsYawOnlyMode()) {
        float target_yaw_rate = ControlState_GetTargetYawRate();
        float yaw_pwm = PID_Compute(&pid_yaw, target_yaw_rate, current_yaw_rate);

        // 限幅
        if (yaw_pwm > YAW_PWM_LIMIT) yaw_pwm = YAW_PWM_LIMIT;
        if (yaw_pwm < -YAW_PWM_LIMIT) yaw_pwm = -YAW_PWM_LIMIT;

        // 左右轮反转实现原地旋转
        Motor_SetPWM((int)(-yaw_pwm), (int)(yaw_pwm));
        return;
    }

    // ----------------------------
    // B. 普通平衡控制（直行 / 后退 / 巡线）
    // ----------------------------

    // 3. 解算当前俯仰角（Pitch）
    float acc_angle = atan2f((float)mpu_data.ay, (float)mpu_data.az) * 57.3f;
    float gyro_x = (float)mpu_data.gx / 131.0f;
    IMU_Filter_Update(acc_angle, gyro_x);
    float angle = IMU_GetAngle();  // 当前车体倾角

    // 4. 角度环 PID（目标角 → 目标速度）
    float target_angle = ControlState_GetTargetAngle();
    float target_speed = PID_Compute(&pid_angle, target_angle, angle);

    // 5. 编码器读取当前速度
    Encoder_Update();
    float left_speed = (float)Encoder_GetSpeed_Left();
    float right_speed = (float)Encoder_GetSpeed_Right();
    float speed = (left_speed + right_speed) / 2.0f;

    // 6. 速度环 PID（目标速度 → PWM）
    float pwm = PID_Compute(&pid_speed, target_speed, speed);

    // 7. 限制总 PWM
    if (pwm > PWM_LIMIT) pwm = PWM_LIMIT;
    if (pwm < -PWM_LIMIT) pwm = -PWM_LIMIT;

    // 8. 左右轮初始输出
    float left_pwm = pwm;
    float right_pwm = pwm;

    // 9. 根据模式做循迹差速调整（红外循迹 / 自定义）
    BalanceCtrl_ApplyTurn(&left_pwm, &right_pwm);

    // 10. 输出到电机
    Motor_SetPWM((int)left_pwm, (int)right_pwm);
}
