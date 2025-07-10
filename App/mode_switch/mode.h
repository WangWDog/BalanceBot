//
// Created by 13033 on 2025/7/10.
//

#ifndef MODE_H
#define MODE_H
typedef enum {
    MODE_BALANCE,
    MODE_TRACE,
    MODE_BT
} ModeEnum;

void Mode_Init(void);
void Mode_Set(ModeEnum m);
ModeEnum Mode_Get(void);
#endif //MODE_H
