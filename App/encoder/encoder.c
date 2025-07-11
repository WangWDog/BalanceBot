#include "encoder.h"
#include "tim.h"  // 包含 htim2 / htim4
#include "stm32f4xx_hal.h"

static volatile int16_t speed_left = 0;
static volatile int16_t speed_right = 0;

void Encoder_Init(void) {
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);  // 左轮
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);  // 右轮
    HAL_TIM_Base_Start_IT(&htim7);  // 计算速度的定时器
}

void Encoder_Update(void) {
    static int16_t last_cnt_left = 0;
    static int16_t last_cnt_right = 0;

    int16_t cnt_left = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
    int16_t cnt_right = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);

    speed_left = cnt_left - last_cnt_left;
    speed_right = cnt_right - last_cnt_right;

    last_cnt_left = cnt_left;
    last_cnt_right = cnt_right;
}

int16_t Encoder_GetSpeed_Left(void) {
    return speed_left;
}

int16_t Encoder_GetSpeed_Right(void) {
    return speed_right;
}
