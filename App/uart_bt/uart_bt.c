#include "uart_bt.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"

#include "control_state.h"
#include "balance_ctrl_turn.h"
#include "encoder.h"
#include "imu_filter.h"
#include "mpu6050.h"
#include "pid.h"
#include "uart_util.h"

extern PID_TypeDef pid_angle;
extern PID_TypeDef pid_speed;
extern PID_TypeDef pid_yaw;

static uint8_t bt_dma_buffer[BT_RX_BUF_LEN];
static uint16_t last_pos = 0;
static char line_buf[64];

void UART_BT_Init(void) {
    HAL_UART_Receive_DMA(&huart2, bt_dma_buffer, BT_RX_BUF_LEN);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
}

void UART_BT_Check_IdleCallback(void) {
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);

        uint16_t curr_pos = BT_RX_BUF_LEN - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
        while (last_pos != curr_pos) {
            static uint16_t line_idx = 0;
            uint8_t byte = bt_dma_buffer[last_pos++];

            if (last_pos >= BT_RX_BUF_LEN) last_pos = 0;

            if (byte == '\n') {
                line_buf[line_idx] = '\0';
                UART_BT_ProcessLine(line_buf);
                line_idx = 0;
            } else {
                if (line_idx < sizeof(line_buf) - 1)
                    line_buf[line_idx++] = byte;
                else
                    line_idx = 0;
            }
        }
    }
}

void UART_BT_ProcessLine(const char *line) {
    BT_ParsedCommand_t cmd = { .type = CMD_UNKNOWN, .param1 = 0 };

    if (strstr(line, "FORWARD")) cmd.type = CMD_FORWARD;
    else if (strstr(line, "BACKWARD")) cmd.type = CMD_BACKWARD;
    else if (strstr(line, "STOP")) cmd.type = CMD_STOP;
    else if (strstr(line, "LEFT")) cmd.type = CMD_LEFT;
    else if (strstr(line, "RIGHT")) cmd.type = CMD_RIGHT;
    else if (strstr(line, "MODE:0")) cmd.type = CMD_MODE0;
    else if (strstr(line, "MODE:1")) cmd.type = CMD_MODE1;
    else if (strstr(line, "MODE:2")) cmd.type = CMD_MODE2;
    else if (sscanf(line, "ANGLE:%f", &cmd.param1) == 1) cmd.type = CMD_ANGLE;
    else if (sscanf(line, "YAW:%f", &cmd.param1) == 1) cmd.type = CMD_YAW;
    else if (sscanf(line, "RATIO:%f,%f", &cmd.param1, &cmd.param2) == 2) cmd.type = CMD_RATIO;
    else if (sscanf(line, "PID_A:%f,%f,%f", &cmd.param1, &cmd.param2, &cmd.param3) == 3) cmd.type = CMD_PID_A;
    else if (sscanf(line, "PID_S:%f,%f,%f", &cmd.param1, &cmd.param2, &cmd.param3) == 3) cmd.type = CMD_PID_S;
    else if (sscanf(line, "PID_Y:%f,%f,%f", &cmd.param1, &cmd.param2, &cmd.param3) == 3) cmd.type = CMD_PID_Y;
    else if (strstr(line, "STATUS")) cmd.type = CMD_STATUS;

    UART_BT_ExecuteParsedCommand(&cmd);
}

void UART_BT_ExecuteParsedCommand(const BT_ParsedCommand_t *cmd) {
    switch (cmd->type) {
        case CMD_FORWARD:
            ControlState_SetTargetAngle(5.0f);
            BT_SendOK("FORWARD");
            break;
        case CMD_BACKWARD:
            ControlState_SetTargetAngle(-5.0f);
            BT_SendOK("BACKWARD");
            break;
        case CMD_STOP:
            ControlState_SetTargetAngle(0);
            ControlState_SetTargetYawRate(0);
            ControlState_SetMotion(MOTION_STOP);
            BT_SendOK("STOP");
            break;
        case CMD_LEFT:
            ControlState_SetTargetYawRate(-80);
            ControlState_SetMotion(MOTION_LEFT);
            BT_SendOK("LEFT");
            break;
        case CMD_RIGHT:
            ControlState_SetTargetYawRate(80);
            ControlState_SetMotion(MOTION_RIGHT);
            BT_SendOK("RIGHT");
            break;
        case CMD_MODE0:
            ControlState_SetMode(MODE_BALANCE);
            BT_SendOK("MODE=0");
            break;
        case CMD_MODE1:
            ControlState_SetMode(MODE_TRACE);
            BT_SendOK("MODE=1");
            break;
        case CMD_MODE2:
            ControlState_SetMode(MODE_BT);
            BT_SendOK("MODE=2");
            break;
        case CMD_ANGLE:
            ControlState_SetTargetAngle(cmd->param1);
            BT_SendOK("ANGLE=%.2f", cmd->param1);
            break;
        case CMD_YAW:
            ControlState_SetTargetYawRate(cmd->param1);
            BT_SendOK("YAW=%.2f", cmd->param1);
            break;
        case CMD_RATIO:
            BalanceCtrl_SetTurnRatio(cmd->param1, cmd->param2);
            BT_SendOK("RATIO=%.2f,%.2f", cmd->param1, cmd->param2);
            break;
        case CMD_PID_A:
            PID_Init(&pid_angle, cmd->param1, cmd->param2, cmd->param3);
            BT_SendOK("PID_A=%.2f,%.2f,%.2f", cmd->param1, cmd->param2, cmd->param3);
            break;
        case CMD_PID_S:
            PID_Init(&pid_speed, cmd->param1, cmd->param2, cmd->param3);
            BT_SendOK("PID_S=%.2f,%.2f,%.2f", cmd->param1, cmd->param2, cmd->param3);
            break;
        case CMD_PID_Y:
            PID_Init(&pid_yaw, cmd->param1, cmd->param2, cmd->param3);
            BT_SendOK("PID_Y=%.2f,%.2f,%.2f", cmd->param1, cmd->param2, cmd->param3);
            break;
        case CMD_STATUS:
            UART_BT_PrintStatus();
            break;
        case CMD_UNKNOWN:
        default:
            BT_SendERR("Unknown command");
            break;
    }
}

void BT_SendOK(const char *format, ...) {
    uart_printf(&huart2, "OK:");
    va_list args;
    va_start(args, format);
    uart_printf(&huart2, format, args);
    va_end(args);
    uart_printf(&huart2, "\n");
}

void BT_SendERR(const char *format, ...) {
    uart_printf(&huart2, "ERR:");
    va_list args;
    va_start(args, format);
    uart_printf(&huart2, format, args);
    va_end(args);
    uart_printf(&huart2, "\n");
}

void UART_BT_PrintStatus(void) {
    float angle = IMU_GetAngle();
    float yaw_rate = (float)mpu_data.gz / 131.0f;
    int16_t left_speed = Encoder_GetSpeed_Left();
    int16_t right_speed = Encoder_GetSpeed_Right();
    float target_angle = ControlState_GetTargetAngle();
    float target_yaw = ControlState_GetTargetYawRate();

    uart_printf(&huart2, "\n==== STATUS ====\n");
    uart_printf(&huart2, "MODE     : %d\n", ControlState_GetMode());
    uart_printf(&huart2, "ANGLE    : %.2f deg\n", angle);
    uart_printf(&huart2, "YAW_RATE : %.2f deg/s\n", yaw_rate);
    uart_printf(&huart2, "TARGET_A : %.2f deg\n", target_angle);
    uart_printf(&huart2, "TARGET_Y : %.2f deg/s\n", target_yaw);
    uart_printf(&huart2, "SPEED_L  : %d\n", left_speed);
    uart_printf(&huart2, "SPEED_R  : %d\n", right_speed);
    uart_printf(&huart2, "PID_ANGLE: Kp=%.2f Ki=%.2f Kd=%.2f\n", pid_angle.Kp, pid_angle.Ki, pid_angle.Kd);
    uart_printf(&huart2, "PID_SPEED: Kp=%.2f Ki=%.2f Kd=%.2f\n", pid_speed.Kp, pid_speed.Ki, pid_speed.Kd);
    uart_printf(&huart2, "PID_YAW  : Kp=%.2f Ki=%.2f Kd=%.2f\n", pid_yaw.Kp, pid_yaw.Ki, pid_yaw.Kd);
    uart_printf(&huart2, "================\n");
}
