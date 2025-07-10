//
// Created by 13033 on 2025/7/10.
//

#ifndef UART_BT_H
#define UART_BT_H

#include "stm32f4xx_hal.h"

#define BT_RX_BUF_LEN 64

typedef enum {
    CMD_NONE,
    CMD_FORWARD,
    CMD_BACKWARD,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_STOP,
    CMD_MODE0,
    CMD_MODE1,
    CMD_MODE2
} BT_Command_t;

void UART_BT_Init(void);
void UART_BT_Check_IdleCallback(void);  // 在 USART2_IRQHandler 中调用
BT_Command_t UART_BT_GetLastCommand(void);
void UART_BT_ClearCommand(void);



#endif //UART_BT_H
