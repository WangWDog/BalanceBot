//
// Created by 13033 on 2025/7/10.
//

#ifndef MPU6050_H
#define MPU6050_H
#include "stm32f4xx_hal.h"

typedef struct {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
} MPU6050_Data;

extern MPU6050_Data mpu_data;

uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c);
void MPU6050_Read_All(I2C_HandleTypeDef *hi2c);


#endif //MPU6050_H
