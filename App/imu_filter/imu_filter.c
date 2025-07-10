//
// Created by 13033 on 2025/7/10.
//

#include "imu_filter.h"


static float angle = 0.0f;

void IMU_Filter_Init(void) {
    angle = 0;
}

void IMU_Filter_Update(float acc_angle, float gyro_rate) {
    static const float alpha = 0.98f;
    static const float dt = 0.005f; // 5ms
    angle = alpha * (angle + gyro_rate * dt) + (1 - alpha) * acc_angle;
}

float IMU_GetAngle(void) {
    return angle;
}