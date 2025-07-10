//
// Created by 13033 on 2025/7/10.
//

#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx_hal.h"

void Motor_Init(void);
void Motor_SetPWM(int left, int right);  // 设置左右轮PWM
void Motor_Stop(void);

#endif //MOTOR_H
