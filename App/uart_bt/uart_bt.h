#ifndef UART_BT_H
#define UART_BT_H

#define BT_RX_BUF_LEN 128

typedef enum {
    CMD_NONE,
    CMD_FORWARD,
    CMD_BACKWARD,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_STOP,
    CMD_MODE0,
    CMD_MODE1,
    CMD_MODE2,
    CMD_PID_A,
    CMD_PID_S,
    CMD_PID_Y,
    CMD_YAW,
    CMD_ANGLE,
    CMD_RATIO,
    CMD_STATUS,
    CMD_UNKNOWN
} BT_Command_t;

typedef struct {
    BT_Command_t type;
    float param1;
    float param2;
    float param3;
} BT_ParsedCommand_t;

void UART_BT_Init(void);
void UART_BT_Check_IdleCallback(void);  // 在 USART2_IRQHandler 中调用
void UART_BT_ProcessLine(const char *line);
void UART_BT_ExecuteParsedCommand(const BT_ParsedCommand_t *cmd);
void UART_BT_PrintStatus(void);

void BT_SendOK(const char *format, ...);
void BT_SendERR(const char *format, ...);


#endif
