#ifndef CONTROL_STATE_H
#define CONTROL_STATE_H

typedef enum {
    MODE_BALANCE,
    MODE_TRACE,
    MODE_BT
} ModeEnum;

typedef enum {
    MOTION_STOP,
    MOTION_FORWARD,
    MOTION_BACKWARD,
    MOTION_LEFT,
    MOTION_RIGHT
} MotionEnum;

void ControlState_Init(void);

void ControlState_SetMode(ModeEnum m);
ModeEnum ControlState_GetMode(void);

void ControlState_SetMotion(MotionEnum m);
MotionEnum ControlState_GetMotion(void);

void ControlState_SetTargetAngle(float angle);
float ControlState_GetTargetAngle(void);

#endif