//
// Created by 13033 on 2025/7/10.
//

#ifndef IMU_FILTER_H
#define IMU_FILTER_H

void IMU_Filter_Init(void);
void IMU_Filter_Update(float acc_angle, float gyro_rate);
float IMU_GetAngle(void);

#endif //IMU_FILTER_H
