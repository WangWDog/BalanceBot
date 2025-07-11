//
// Created by 13033 on 2025/7/10.
//

#ifndef BALANCE_CTRL_TURN_H
#define BALANCE_CTRL_TURN_H

void BalanceCtrl_ApplyTurn(float *left_pwm, float *right_pwm);

// 比例参数设置（例如通过串口调试或 config.h 初始化）
void BalanceCtrl_SetTurnRatio(float left_ratio, float right_ratio);
void BalanceCtrl_GetTurnRatio(float *left_ratio, float *right_ratio);
#endif //BALANCE_CTRL_TURN_H
