//
// Created by 13033 on 2025/7/10.
//

#include "balance_ctrl_turn.h"

#include "control_state.h"
#include "ir_tracking.h"

// 默认差速比例
static float left_ratio = 0.6f;
static float right_ratio = 0.6f;

void BalanceCtrl_ApplyTurn(float *left_pwm, float *right_pwm)
{
    if (!left_pwm || !right_pwm) return;

    ModeEnum mode = ControlState_GetMode();

    if (mode == MODE_BT) {
        switch (ControlState_GetMotion()) {
        case MOTION_LEFT:
            *left_pwm *= left_ratio;
            break;
        case MOTION_RIGHT:
            *right_pwm *= right_ratio;
            break;
        default:
            break;
        }
    }

    else if (mode == MODE_TRACE) {
        int offset = IR_Get_Line_Position();  // -1 左偏，0 中，1 右偏
        if (offset == -1) {
            *left_pwm *= left_ratio;
        } else if (offset == 1) {
            *right_pwm *= right_ratio;
        }
    }
}

void BalanceCtrl_SetTurnRatio(float l, float r) {
    left_ratio = l;
    right_ratio = r;
}

void BalanceCtrl_GetTurnRatio(float *l, float *r) {
    if (l) *l = left_ratio;
    if (r) *r = right_ratio;
}
