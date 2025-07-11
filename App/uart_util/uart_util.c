#include "uart_util.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

#define UART_TX_BUF_LEN 128  // 可根据需要增大

void uart_printf(UART_HandleTypeDef *huart, const char *fmt, ...) {
    char buffer[UART_TX_BUF_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    HAL_UART_Transmit(huart, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}