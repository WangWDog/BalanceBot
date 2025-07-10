//
// Created by 13033 on 2025/7/10.
//

#include "ir_tracking.h"

static uint8_t ir[5];  // 假设5路红外

void IR_Tracking_Init(void) {
    // GPIO或ADC初始化完成后，无需特别处理
}

void IR_Tracking_Update(void) {
    // GPIO读取版本：
    ir[0] = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
    ir[1] = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
    ir[2] = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
    ir[3] = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
    ir[4] = !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);
}

int IR_Get_Line_Position(void) {
    if (ir[0] || ir[1]) return -1;
    if (ir[3] || ir[4]) return 1;
    return 0;
}