//
// Created by 13033 on 2025/7/10.
//
#include "uart_bt.h"
#include "usart.h"
#include "string.h"
#include "mode.h"

static uint8_t bt_dma_buffer[BT_RX_BUF_LEN];
static BT_Command_t last_command = CMD_NONE;

void UART_BT_Init(void) {
    // 启动DMA接收，循环缓冲
    HAL_UART_Receive_DMA(&huart2, bt_dma_buffer, BT_RX_BUF_LEN);

    // 开启 IDLE 中断检测（检测一帧结束）
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
}

// 由 USART2_IRQHandler 调用
void UART_BT_Check_IdleCallback(void) {
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2); //清除IDLE FLAG 中断标志位

        // 禁止DMA，计算有效数据长度
        HAL_UART_DMAStop(&huart2);
        uint16_t len = BT_RX_BUF_LEN - __HAL_DMA_GET_COUNTER(huart2.hdmarx); //一个是剩下来的字节数
        bt_dma_buffer[len] = '\0';  // 添加结尾

        // 解析指令
        if (strstr((char *)bt_dma_buffer, "FORWARD")) last_command = CMD_FORWARD;
        else if (strstr((char *)bt_dma_buffer, "BACKWARD")) last_command = CMD_BACKWARD;
        else if (strstr((char *)bt_dma_buffer, "LEFT")) last_command = CMD_LEFT;
        else if (strstr((char *)bt_dma_buffer, "RIGHT")) last_command = CMD_RIGHT;
        else if (strstr((char *)bt_dma_buffer, "STOP")) last_command = CMD_STOP;
        else if (strstr((char *)bt_dma_buffer, "MODE0")) { Mode_Set(MODE_BALANCE); last_command = CMD_MODE0; }
        else if (strstr((char *)bt_dma_buffer, "MODE1")) { Mode_Set(MODE_TRACE); last_command = CMD_MODE1; }
        else if (strstr((char *)bt_dma_buffer, "MODE2")) { Mode_Set(MODE_BT); last_command = CMD_MODE2; }

        // 重新启用 DMA
        HAL_UART_Receive_DMA(&huart2, bt_dma_buffer, BT_RX_BUF_LEN);
    }
}

BT_Command_t UART_BT_GetLastCommand(void) {
    return last_command;
}

void UART_BT_ClearCommand(void) {
    last_command = CMD_NONE;
}