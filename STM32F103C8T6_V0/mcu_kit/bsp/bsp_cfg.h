#ifndef _VD_BSP_CFG_H
#define _VD_BSP_CFG_H

#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_dwt.h"
#include "log.h"
#include "sys.h" 
#include "queue.h"
/************************** UART1 ******************************/
#define BSP_UART_TEST      1
#define USART1_ENABLE      1

#define USART1_RX_PORT	   GPIO_PA10
#define USART1_TX_PORT	   GPIO_PA9
#define USART1_MODE        VD_UART_RX_TX
#define USART1_BAUDR       115200
#define USART1_PRI         3

#define USART2_ENABLE      0

#define USART3_ENABLE      0
/************************** LOG ******************************/
#define LOG_ENABLE         1

#define ABORT_LOG          1 //中断
#define ERROR_LOG          1 //错误
#define INFO_LOG           1 //信息
#define DEBUG_LOG          1 //调试
#define FATAL_LOG          1 //致命错误
#define WARN_LOG           1 //警告


#endif