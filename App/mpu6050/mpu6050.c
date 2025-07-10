//
// Created by 13033 on 2025/7/10.
//

#include "mpu6050.h"


MPU6050_Data mpu_data;

uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c) {
    // 写入寄存器初始化 MPU6050
    return 0; // 返回0代表成功
}

void MPU6050_Read_All(I2C_HandleTypeDef *hi2c) {
    // 读取加速度 & 陀螺仪数据，更新 mpu_data
}