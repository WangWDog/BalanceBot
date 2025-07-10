#include "control_state.h"

static ModeEnum current_mode = MODE_BALANCE;
static MotionEnum current_motion = MOTION_STOP;
static float target_angle = 0;

void ControlState_Init(void) {
    current_mode = MODE_BALANCE;
    current_motion = MOTION_STOP;
    target_angle = 0;
}

void ControlState_SetMode(ModeEnum m) {
    current_mode = m;
}

ModeEnum ControlState_GetMode(void) {
    return current_mode;
}

void ControlState_SetMotion(MotionEnum m) {
    current_motion = m;
}

MotionEnum ControlState_GetMotion(void) {
    return current_motion;
}

void ControlState_SetTargetAngle(float angle) {
    target_angle = angle;
}

float ControlState_GetTargetAngle(void) {
    return target_angle;
}
