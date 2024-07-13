#ifndef _VD_BSP_UART_H
#define _VD_BSP_UART_H

#include "base_type.h"

#define    BSP_UART_TEST     0        //���ڲ��Ժ꿪��


typedef void (*usart_callback_handle)(void);
	

typedef enum {            
  VD_USART1 = 1,  
  VD_USART2,
  VD_USART3,
	
} VD_USARTx_E;


uint8_t vd_bsp_usart_init(VD_USARTx_E USARTx);     		//���ڳ�ʼ��
char vd_bsp_usart_ReceiveByte(VD_USARTx_E USARTx); 		//���ڽ���һ���ֽ�
void vd_bsp_usart_SendByte(VD_USARTx_E USARTx, char ch);//���ڷ���һ���ֽ�
void vd_bsp_usart_SendBytes(VD_USARTx_E USARTx, uint8_t* data, uint8_t len);    //���ڷ��Ͷ���ֽ�
void vd_bsp_usart_SendString(VD_USARTx_E USARTx, char *String);                 //���ڷ���һ���ַ���
uint8_t vd_bsp_usart_register_rx_callback_handle(VD_USARTx_E USARTx, usart_callback_handle callback_handle); //����ע��ص�����

uint8_t vd_bsp_usart_get_receive_data_len(VD_USARTx_E USARTx);                  //��ȡ���յ������ݳ���
int8_t vd_bsp_usart_read_bytes(VD_USARTx_E USARTx, uint8_t* buff, uint8_t len); //������������
//bsp ���ڳ�ʼ������
#if BSP_UART_TEST
	void vd_bsp_usart_test_init(VD_USARTx_E USARTx);  //���Գ�ʼ������
	void vd_bsp_usart_test(void);                     //��ѭ�����ã�PC����һ���ֽڣ�����+1
#endif

#endif




