#ifndef _VD_BSP_UART_H
#define _VD_BSP_UART_H

#include "base_type.h"

#define    BSP_UART_TEST     0        //串口测试宏开关


typedef void (*usart_callback_handle)(void);
	

typedef enum {            
  VD_USART1 = 1,  
  VD_USART2,
  VD_USART3,
	
} VD_USARTx_E;


uint8_t vd_bsp_usart_init(VD_USARTx_E USARTx);     		//串口初始化
char vd_bsp_usart_ReceiveByte(VD_USARTx_E USARTx); 		//串口接收一个字节
void vd_bsp_usart_SendByte(VD_USARTx_E USARTx, char ch);//串口发送一个字节
void vd_bsp_usart_SendBytes(VD_USARTx_E USARTx, uint8_t* data, uint8_t len);    //串口发送多个字节
void vd_bsp_usart_SendString(VD_USARTx_E USARTx, char *String);                 //串口发送一个字符串
uint8_t vd_bsp_usart_register_rx_callback_handle(VD_USARTx_E USARTx, usart_callback_handle callback_handle); //串口注册回调函数

uint8_t vd_bsp_usart_get_receive_data_len(VD_USARTx_E USARTx);                  //获取接收到的数据长度
int8_t vd_bsp_usart_read_bytes(VD_USARTx_E USARTx, uint8_t* buff, uint8_t len); //读缓冲区数据
//bsp 串口初始化函数
#if BSP_UART_TEST
	void vd_bsp_usart_test_init(VD_USARTx_E USARTx);  //测试初始化函数
	void vd_bsp_usart_test(void);                     //主循环调用，PC发送一个字节，返回+1
#endif

#endif




