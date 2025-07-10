#include "balance_ctrl.h"

#include <math.h>

#include "balance_ctrl_turn.h"
#include "mpu6050.h"
#include "imu_filter.h"
#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include "i2c.h"
#include "control_state.h"  // ✅ 加入状态管理模块

#define PWM_LIMIT 1000

static PID_TypeDef pid_angle;
static PID_TypeDef pid_speed;

void BalanceCtrl_Init(void) {
    PID_Init(&pid_angle, 22.0f, 0.6f, 0.8f);  // 需调参
    PID_Init(&pid_speed, 1.2f, 0.01f, 0.03f);
    IMU_Filter_Init();
    Encoder_Init();
    Motor_Init();
}

void BalanceCtrl_Loop(void) {
    // 1. 读取IMU数据
    MPU6050_Read_All(&hi2c1);

    // 2. 姿态解算：用互补滤波融合加速度与角速度
    float acc_angle = atan2f((float)mpu_data.ay, (float)mpu_data.az) * 57.3f;
    float gyro = (float)mpu_data.gx / 131.0f;  // 陀螺仪X轴（左右转）方向
    IMU_Filter_Update(acc_angle, gyro);

    float angle = IMU_GetAngle();  // 当前车体倾角（正前倾）

    // 3. 读取目标角度
    float target_angle = ControlState_GetTargetAngle();  // 来自蓝牙或循迹控制
    float target_speed = PID_Compute(&pid_angle, target_angle, angle);

    // 4. 编码器读取当前速度（正表示前进）
    Encoder_Update();
    float left_speed = (float)Encoder_GetSpeed_Left();
    float right_speed = (float)Encoder_GetSpeed_Right();
    float speed = (left_speed + right_speed) / 2.0f;

    // 5. 速度环 PID 控制 → 输出 PWM
    float pwm = PID_Compute(&pid_speed, target_speed, speed);

    // 6. 限制 PWM 输出
    if (pwm > PWM_LIMIT) pwm = PWM_LIMIT;
    if (pwm < -PWM_LIMIT) pwm = -PWM_LIMIT;

    float left_pwm = pwm;
    float right_pwm = pwm;

    BalanceCtrl_ApplyTurn(&left_pwm,&right_pwm);

    // 7. 电机输出（左右轮输出相同，后续可扩展转向差速）
    Motor_SetPWM((int)left_pwm, (int)right_pwm);
}