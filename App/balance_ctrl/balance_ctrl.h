//
// Created by 13033 on 2025/7/10.
//

#ifndef BALANCE_CTRL_H
#define BALANCE_CTRL_H
#include "stm32f4xx_hal.h"

void BalanceCtrl_Init(void);
void BalanceCtrl_Loop(void);  // 每5ms调用一次


#endif //BALANCE_CTRL_H
