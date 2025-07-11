#ifndef UART_UTIL_H
#define UART_UTIL_H

#include "usart.h"

void uart_printf(UART_HandleTypeDef *huart, const char *fmt, ...);

#endif