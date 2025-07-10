//
// Created by 13033 on 2025/7/10.
//

#include "balance_ctrl_turn.h"

#include "control_state.h"

void BalanceCtrl_ApplyTurn(float *left_pwm, float *right_pwm) {
    if (ControlState_GetMode() == MODE_BT) {
        switch (ControlState_GetMotion()) {
        case MOTION_LEFT:
            *left_pwm *= 0.6f;
            *right_pwm *= 1.0f;
            break;
        case MOTION_RIGHT:
            *left_pwm *= 1.0f;
            *right_pwm *= 0.6f;
            break;
        default:
            break;
        }
    }
    // 可拓展：MODE_TRACE 差速控制
}
