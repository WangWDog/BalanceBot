#include "control_state.h"

static ModeEnum current_mode = MODE_BALANCE;
static MotionEnum current_motion = MOTION_STOP;

static float target_angle = 0.0f;        // 前后倾角目标
static float target_yaw_angle = 0.0f;    // 偏航角目标（左右转）

void ControlState_Init(void) {
    current_mode = MODE_BALANCE;
    current_motion = MOTION_STOP;
    target_angle = 0.0f;
    target_yaw_angle = 0.0f;
}

// ==== 模式控制 ====

void ControlState_SetMode(ModeEnum m) {
    current_mode = m;
}

ModeEnum ControlState_GetMode(void) {
    return current_mode;
}

// ==== 运动状态 ====

void ControlState_SetMotion(MotionEnum m) {
    current_motion = m;
}

MotionEnum ControlState_GetMotion(void) {
    return current_motion;
}

// ==== 前后平衡角 ====

void ControlState_SetTargetAngle(float angle) {
    target_angle = angle;
}

float ControlState_GetTargetAngle(void) {
    return target_angle;
}

// ==== 偏航角控制（用于原地转向） ====

void ControlState_SetTargetYawAngle(float yaw) {
    target_yaw_angle = yaw;
}

void ControlState_AddYawAngleDelta(float delta) {
    target_yaw_angle += delta;
}

float ControlState_GetTargetYawAngle(void) {
    return target_yaw_angle;
}

void ControlState_ResetTargetYawAngle(void) {
    target_yaw_angle = 0.0f;
}
