#include "imu_filter.h"

// === Pitch (俯仰) ===
static float pitch_angle = 0.0f;

// === Yaw (偏航) ===
static float yaw_angle = 0.0f;

void IMU_Filter_Init(void) {
    pitch_angle = 0.0f;
    yaw_angle = 0.0f;
}

void IMU_Filter_Update(float acc_pitch, float gyro_pitch_rate, float gyro_yaw_rate) {
    static const float alpha = 0.98f;
    static const float dt = 0.005f;  // 控制周期 = 5ms

    // Pitch 使用互补滤波
    pitch_angle = alpha * (pitch_angle + gyro_pitch_rate * dt) + (1 - alpha) * acc_pitch;

    // Yaw 使用积分（假设初始为 0，且无漂移修正）
    yaw_angle += gyro_yaw_rate * dt;

    // 防止 yaw 角无限累加（限制在 [-180,180] 或 [0,360]，这里选择后者）
    if (yaw_angle >= 360.0f) yaw_angle -= 360.0f;
    else if (yaw_angle < 0.0f) yaw_angle += 360.0f;
}

float IMU_GetPitch(void) {
    return pitch_angle;
}

float IMU_GetYaw(void) {
    return yaw_angle;
}
