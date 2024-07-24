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
	VD_USARTx_E    USARTx;   //���ں�
	GPIO_PORT_E    tx_port;  //tx����
	GPIO_PORT_E    rx_port;  //rx����
	VD_UART_MODE_E mode;     //ģʽ
	uint32_t       buad;     //������
	uint8_t        pri;      //���ȼ�
}vd_uart_t;


typedef void (*usart_callback_handle)(void);





typedef enum {  
  VD_BAUD_4800 = 0,
  VD_BAUD_9600,  
  VD_BAUD_115200,
	
  VD_BAUD_MAX,
	
} VD_BAUD_E;

vd_int8_t vd_bsp_usart_init(VD_USARTx_E USARTx);

//uint8_t vd_bsp_usart_init(VD_USARTx_E USARTx);     		//���ڳ�ʼ��
//char vd_bsp_usart_ReceiveByte(VD_USARTx_E USARTx); 		//���ڽ���һ���ֽ�
//void vd_bsp_usart_SendByte(VD_USARTx_E USARTx, char ch);//���ڷ���һ���ֽ�
//void vd_bsp_usart_SendBytes(VD_USARTx_E USARTx, uint8_t* data, uint8_t len);    //���ڷ��Ͷ���ֽ�
void vd_bsp_usart_SendString(VD_USARTx_E USARTx, char *String);                 //���ڷ���һ���ַ���
//uint8_t vd_bsp_usart_register_rx_callback_handle(VD_USARTx_E USARTx, usart_callback_handle callback_handle); //����ע��ص�����

//uint8_t vd_bsp_usart_get_receive_data_len(VD_USARTx_E USARTx);                  //��ȡ���յ������ݳ���
//int8_t vd_bsp_usart_read_bytes(VD_USARTx_E USARTx, uint8_t* buff, uint8_t len); //������������
////bsp ���ڳ�ʼ������

	void vd_bsp_usart_test_init(VD_USARTx_E USARTx);  //���Գ�ʼ������
	void vd_bsp_usart_test(void);                     //��ѭ�����ã�PC����һ���ֽڣ�����+1


#endif




