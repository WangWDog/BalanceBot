//
// Created by 13033 on 2025/7/10.
//

#ifndef ENCODER_H
#define ENCODER_H
#include "stm32f4xx_hal.h"

void Encoder_Init(void);
void Encoder_Update(void);
int16_t Encoder_GetSpeed_Left(void);
int16_t Encoder_GetSpeed_Right(void);

#endif //ENCODER_H
