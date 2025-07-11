#ifndef IMU_FILTER_H
#define IMU_FILTER_H

void IMU_Filter_Init(void);

// 更新：加速度角（Pitch） + 陀螺仪俯仰角速度 + 偏航角速度
void IMU_Filter_Update(float acc_pitch, float gyro_pitch_rate, float gyro_yaw_rate);

float IMU_GetPitch(void);
float IMU_GetYaw(void);

#endif
