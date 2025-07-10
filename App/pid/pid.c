//
// Created by 13033 on 2025/7/10.
//
#include "pid.h"
void PID_Init(PID_TypeDef *pid, float Kp, float Ki, float Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->integral = 0;
    pid->prev_error = 0;
    pid->output = 0;
}

float PID_Compute(PID_TypeDef *pid, float target, float measured) {
    float error = target - measured;
    pid->integral += error;
    float derivative = error - pid->prev_error;
    pid->prev_error = error;
    pid->output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
    return pid->output;
}