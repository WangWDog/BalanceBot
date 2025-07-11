#include "motor.h"

#include <stdlib.h>

#include "tim.h"
#include "gpio.h"
#include "main.h"

#define PWM_MAX 1000

void Motor_Init(void) {
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);  // 左电机 PWM
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);  // 右电机 PWM
}

void Motor_SetPWM(int left, int right) {
    if (left > PWM_MAX)  left = PWM_MAX;
    if (left < -PWM_MAX) left = -PWM_MAX;
    if (right > PWM_MAX)  right = PWM_MAX;
    if (right < -PWM_MAX) right = -PWM_MAX;

    // === 左电机方向控制 ===
    if (left > 0) {
        HAL_GPIO_WritePin(MOTOR_L_IN1_GPIO_Port, MOTOR_L_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_L_IN2_GPIO_Port, MOTOR_L_IN2_Pin, GPIO_PIN_RESET);
    } else if (left < 0) {
        HAL_GPIO_WritePin(MOTOR_L_IN1_GPIO_Port, MOTOR_L_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_L_IN2_GPIO_Port, MOTOR_L_IN2_Pin, GPIO_PIN_SET);
    } else { // 停止
        HAL_GPIO_WritePin(MOTOR_L_IN1_GPIO_Port, MOTOR_L_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_L_IN2_GPIO_Port, MOTOR_L_IN2_Pin, GPIO_PIN_RESET);
    }

    // === 右电机方向控制 ===
    if (right > 0) {
        HAL_GPIO_WritePin(MOTOR_R_IN1_GPIO_Port, MOTOR_R_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_R_IN2_GPIO_Port, MOTOR_R_IN2_Pin, GPIO_PIN_RESET);
    } else if (right < 0) {
        HAL_GPIO_WritePin(MOTOR_R_IN1_GPIO_Port, MOTOR_R_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_R_IN2_GPIO_Port, MOTOR_R_IN2_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(MOTOR_R_IN1_GPIO_Port, MOTOR_R_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_R_IN2_GPIO_Port, MOTOR_R_IN2_Pin, GPIO_PIN_RESET);
    }

    // 设置 PWM 占空比
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, abs(left));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, abs(right));
}

void Motor_Stop(void) {
    Motor_SetPWM(0, 0);  // 等价于停止
}
