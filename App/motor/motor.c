//
// Created by 13033 on 2025/7/10.
//

#include "motor.h"

#include "tim.h"

void Motor_Init(void) {
    // 初始化PWM和方向引脚（TIM已由CubeMX初始化）
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);  // 左电机
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);  // 右电机
}

void Motor_SetPWM(int left, int right) {
    // 设置电机方向与PWM占空比
}

void Motor_Stop(void) {
    // 停止电机
}