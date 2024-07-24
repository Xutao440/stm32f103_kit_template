#ifndef _VD_BSP_UART_H
#define _VD_BSP_UART_H

#include "base_type.h"
#include "bsp_cfg.h"

#define RCC_APB2Periph_USART1            ((uint32_t)0x00004000)
#define RCC_APB1Periph_USART2            ((uint32_t)0x00020000)
#define RCC_APB1Periph_USART3            ((uint32_t)0x00040000)
#define RCC_APB1Periph_UART4             ((uint32_t)0x00080000)
#define RCC_APB1Periph_UART5             ((uint32_t)0x00100000)

#define USART_FLAG_CTS                       ((uint16_t)0x0200)
#define USART_FLAG_LBD                       ((uint16_t)0x0100)
#define USART_FLAG_TXE                       ((uint16_t)0x0080)
#define USART_FLAG_TC                        ((uint16_t)0x0040)
#define USART_FLAG_RXNE                      ((uint16_t)0x0020)
#define USART_FLAG_IDLE                      ((uint16_t)0x0010)
#define USART_FLAG_ORE                       ((uint16_t)0x0008)
#define USART_FLAG_NE                        ((uint16_t)0x0004)
#define USART_FLAG_FE                        ((uint16_t)0x0002)
#define USART_FLAG_PE                        ((uint16_t)0x0001)


typedef enum {            
  VD_UART_RX = ((uint16_t)0x0004),  
  VD_UART_TX = ((uint16_t)0x0008),
  VD_UART_RX_TX = ((uint16_t)0x000C),
	
} VD_UART_MODE_E;

typedef enum {            
  VD_USART1 = 1,  
  VD_USART2,
  VD_USART3,
	
} VD_USARTx_E;

typedef struct{
	VD_USARTx_E    USARTx;   //串口号
	GPIO_PORT_E    tx_port;  //tx引脚
	GPIO_PORT_E    rx_port;  //rx引脚
	VD_UART_MODE_E mode;     //模式
	uint32_t       buad;     //波特率
	uint8_t        pri;      //优先级
}vd_uart_t;


typedef void (*usart_callback_handle)(void);





typedef enum {  
  VD_BAUD_4800 = 0,
  VD_BAUD_9600,  
  VD_BAUD_115200,
	
  VD_BAUD_MAX,
	
} VD_BAUD_E;

vd_int8_t vd_bsp_usart_init(VD_USARTx_E USARTx);

//uint8_t vd_bsp_usart_init(VD_USARTx_E USARTx);     		//串口初始化
//char vd_bsp_usart_ReceiveByte(VD_USARTx_E USARTx); 		//串口接收一个字节
//void vd_bsp_usart_SendByte(VD_USARTx_E USARTx, char ch);//串口发送一个字节
//void vd_bsp_usart_SendBytes(VD_USARTx_E USARTx, uint8_t* data, uint8_t len);    //串口发送多个字节
void vd_bsp_usart_SendString(VD_USARTx_E USARTx, char *String);                 //串口发送一个字符串
//uint8_t vd_bsp_usart_register_rx_callback_handle(VD_USARTx_E USARTx, usart_callback_handle callback_handle); //串口注册回调函数

//uint8_t vd_bsp_usart_get_receive_data_len(VD_USARTx_E USARTx);                  //获取接收到的数据长度
//int8_t vd_bsp_usart_read_bytes(VD_USARTx_E USARTx, uint8_t* buff, uint8_t len); //读缓冲区数据
////bsp 串口初始化函数

	void vd_bsp_usart_test_init(VD_USARTx_E USARTx);  //测试初始化函数
	void vd_bsp_usart_test(void);                     //主循环调用，PC发送一个字节，返回+1


#endif




