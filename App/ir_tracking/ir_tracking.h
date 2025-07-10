//
// Created by 13033 on 2025/7/10.
//

#ifndef IR_TRACKING_H
#define IR_TRACKING_H
#include "stm32f4xx_hal.h"

void IR_Tracking_Init(void);
void IR_Tracking_Update(void);
int IR_Get_Line_Position(void);  // 返回 -1:左偏, 0:中, 1:右偏


#endif //IR_TRACKING_H
