//
// Created by 13033 on 2025/7/10.
//

#ifndef PID_H
#define PID_H
typedef struct {
    float Kp, Ki, Kd;
    float integral;
    float prev_error;
    float output;
} PID_TypeDef;

void PID_Init(PID_TypeDef *pid, float Kp, float Ki, float Kd);
float PID_Compute(PID_TypeDef *pid, float target, float measured);
#endif //PID_H
