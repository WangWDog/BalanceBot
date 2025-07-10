//
// Created by 13033 on 2025/7/10.
//

#include "mode.h"

static ModeEnum current_mode = MODE_BALANCE;

void Mode_Init(void) {
    current_mode = MODE_BALANCE;
}

void Mode_Set(ModeEnum m) {
    current_mode = m;
}

ModeEnum Mode_Get(void) {
    return current_mode;
}