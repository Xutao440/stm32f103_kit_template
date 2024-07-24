#include "string.h"
#include "bsp_cfg.h"

usart_callback_handle vd_usart1_callback_handle = NULL;
usart_callback_handle vd_usart2_callback_handle = NULL;
usart_callback_handle vd_usart3_callback_handle = NULL;
//���ջ������
queue_t  vd_usart1_rx_queue;
queue_t  vd_usart2_rx_queue;
queue_t  vd_usart3_rx_queue;
/*
************************************************************
*	�������ƣ�	vd_bsp_usart_init
*	�������ܣ�	���ڳ�ʼ��
*	��ڲ�����	USARTx�����ں�
*	���ز�����	��	
************************************************************
*/
vd_int8_t vd_bsp_usart_init(VD_USARTx_E USARTx)
{
	vd_int8_t ret = 0;
	vd_uint8_t div = 1;   //��Ƶϵ��
	vd_uint32_t baud = 0;
	USART_TypeDef* usart = NULL;
	float temp = 0;
	vd_uint16_t mantissa = 0, fraction = 0;
		
	switch((vd_uint8_t)USARTx)
	{
		case VD_USART1:
			#if USART1_ENABLE
				div = 1;
				if(USART1_MODE & VD_UART_RX){
					vd_bsp_gpio_init(USART1_RX_PORT, VD_GPIO_IPU);    //RX  ��������
					ret = creat_queue(&vd_usart1_rx_queue, 64); 
					if(ret !=0){
						log_error("%s: creat queue fail !",__FUNCTION__);
						goto erro;
					}
				}
				if(USART1_MODE & VD_UART_TX){
					vd_bsp_gpio_init(USART1_TX_PORT, VD_GPIO_AF_PP);  //TX  ��������
				}
				RCC->APB2ENR|=1<<14;  //ʹ�ܴ���1ʱ�� 
				baud = USART1_BAUDR;
			#endif
			break;
		case VD_USART2:
			#if USART2_ENABLE
				div  = 2;
				if(USART2_MODE & VD_UART_RX){
					vd_bsp_gpio_init(USART2_RX_PORT, VD_GPIO_IPU);    //RX  ��������
				}
				if(USART2_MODE & VD_UART_TX){
					vd_bsp_gpio_init(USART2_TX_PORT, VD_GPIO_AF_PP);  //TX  ��������
				}	
			#endif			
			break;	

		#if USART3_ENABLE			
		case VD_USART3:
			break;
		#endif
		default:
			break;			
	}
	
	//����������		
	temp=(float)(72/div*1000000)/(baud*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
    	   	RCC->APB2RSTR|=1<<14;   //��λ����1
			RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
	
    USART1->CR1|= 0X01<<5;    //���ջ������ǿ��ж�ʹ��	    	
	VD_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
erro:	
	return ret;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_usart_get_receive_data_len
*	�������ܣ�	��ȡ���յ������ݳ���
*	��ڲ�����	USARTx: ���ں�
*	���ز�����	���ݳ���	
************************************************************
*/
uint8_t vd_bsp_usart_get_receive_data_len(VD_USARTx_E USARTx)
{
	uint8_t  data_len = 0;
	
	if(USARTx == VD_USART1){
		data_len = vd_usart1_rx_queue.len;
		
	}else if(USARTx == VD_USART2){
		data_len = vd_usart2_rx_queue.len;
		
	}else if(USARTx == VD_USART3){
		data_len = vd_usart3_rx_queue.len;
	}
		
	return  data_len;
}

/*
************************************************************
*	�������ƣ�	vd_bsp_usart_read_bytes
*	�������ܣ�	���ڽ���һ���ֽ�
*	��ڲ�����	USARTx: ���ں�
*	���ز�����	0�����ɹ���<0����ʧ��	
************************************************************
*/
int8_t vd_bsp_usart_read_bytes(VD_USARTx_E USARTx, uint8_t* buff, uint8_t len)
{
	int8_t  ret_value = 0;
	
	if(USARTx == VD_USART1){
		ret_value = queue_pull_bytes(&vd_usart1_rx_queue, buff, len);
		
	}else if(USARTx == VD_USART2){
		ret_value = queue_pull_bytes(&vd_usart2_rx_queue, buff, len);

	}else if(USARTx == VD_USART3){
		ret_value = queue_pull_bytes(&vd_usart3_rx_queue, buff, len);	
	}
	
	return ret_value;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_usart_ReceiveByte
*	�������ܣ�	���ڽ���һ���ֽ�
*	��ڲ�����	USARTx: ���ں�
*	���ز�����	���յ��ֽ�	
************************************************************
*/
char vd_bsp_usart_ReceiveByte(VD_USARTx_E USARTx)
{
	USART_TypeDef     *vd_usart;
	
	/*�˿ڲ�����ֵ*/
	if(USARTx == VD_USART1){
		vd_usart = USART1;
	}else if(USARTx == VD_USART2){
		vd_usart = USART2;
	}else if(USARTx == VD_USART3){
		vd_usart = USART3;		
	}
	
	return (uint16_t)(vd_usart->DR & (uint16_t)0x01FF);
}



/*
************************************************************
*	�������ƣ�	vd_bsp_usart_SendByte
*	�������ܣ�	���ڷ���һ���ֽ�
*	��ڲ�����	USARTx: ���ںţ�ch������
*	���ز�����	��	
************************************************************
*/
void vd_bsp_usart_SendByte(VD_USARTx_E USARTx, char ch)
{
	uint16_t timeout = 0;
	USART_TypeDef  *vd_usart;
	
	/*�˿ڲ�����ֵ*/
	if(USARTx == VD_USART1){
		vd_usart = USART1;
	}else if(USARTx == VD_USART2){
		vd_usart = USART2;
	}else if(USARTx == VD_USART3){
		vd_usart = USART3;		
	}
  
	while((vd_usart->SR & USART_FLAG_TC) == RESET){
		timeout++;
		if(timeout > 5000){
			break;
		}	
	};
	
	vd_usart->DR = (ch & (uint16_t)0x01FF);
 
}



/*
************************************************************
*	�������ƣ�	vd_bsp_usart_SendBytes
*	�������ܣ�	���ڷ��Ͷ��ֽ�
*	��ڲ�����	USARTx: ���ںţ�data�����ݣ� len�����ݳ���
*	���ز�����	��	
************************************************************
*/
void vd_bsp_usart_SendBytes(VD_USARTx_E USARTx, uint8_t* data, uint8_t len)
{
	uint8_t index = 0;
	
	for(index = 0; index < len; index++){
		vd_bsp_usart_SendByte(USARTx, data[index]);
	}
   
}


/*
************************************************************
*	�������ƣ�	vd_bsp_usart_SendString
*	�������ܣ�	���ڷ���һ���ַ���
*	��ڲ�����	USARTx: ���ںţ�String���ַ���
*	���ز�����	��	
************************************************************
*/
void vd_bsp_usart_SendString(VD_USARTx_E USARTx, char *String)
{
	uint8_t index;
	for (index = 0; String[index] != '\0'; index++){
		vd_bsp_usart_SendByte(USARTx, String[index]);
	}
}



/*
************************************************************
*	�������ƣ�	vd_bsp_usart_register_rx_callback_handle
*	�������ܣ�	ע�ᴮ�ڽ��ջص�����
*	��ڲ�����	USARTx: ���ںţ� callback_handle���ص�����
*	���ز�����	�ɹ�����0�� ʧ�ܷ���>0
************************************************************
*/
uint8_t vd_bsp_usart_register_rx_callback_handle(VD_USARTx_E USARTx, usart_callback_handle callback_handle)
{
	uint8_t vd_erro = 0;
	
	if((USARTx == VD_USART1) && (vd_usart1_callback_handle == NULL)){
		vd_usart1_callback_handle = callback_handle;
	}else if((USARTx == VD_USART2) && (vd_usart2_callback_handle == NULL)){
		vd_usart2_callback_handle = callback_handle;
	}else if((USARTx == VD_USART3) && (vd_usart3_callback_handle == NULL)){
		vd_usart3_callback_handle = callback_handle;		
	}else{
		vd_erro = 1;
		goto erro;
	}
	
	erro:
		return vd_erro;
}

/*
************************************************************
*	�������ƣ�	UART1_IRQHandler
*	�������ܣ�	����1�жϴ�����
*	��ڲ�����	��
*	���ز�����	��	
************************************************************
*/
void USART1_IRQHandler(void)
{
	uint8_t data = 0;
	
    if (USART1->SR & (1u << 5)){ //bit5: RXNE�������ݼĴ����ǿ� 
		
		if(vd_usart1_callback_handle != NULL){
			vd_usart1_callback_handle();
		}else{
			data = USART1->DR;
			queue_pull_bytes(&vd_usart1_rx_queue, &data, 1);
			vd_usart1_rx_queue.len++;
		}

        USART1->SR &= ~(1u << 5);
    }
}


/*
************************************************************
*	�������ƣ�	UART2_IRQHandler
*	�������ܣ�	����2�жϴ�����
*	��ڲ�����	��
*	���ز�����	��	
************************************************************
*/
void USART2_IRQHandler(void)
{
	uint8_t data = 0;
	
   if (USART2->SR & (1u << 5)){ //bit5: RXNE�������ݼĴ����ǿ� 
		
		if(vd_usart2_callback_handle != NULL){
			vd_usart2_callback_handle();
		}else{
			data = USART2->DR;
			queue_push_bytes(&vd_usart2_rx_queue, &data, 1);
			vd_usart2_rx_queue.len++;
		}
		
          USART2->SR &= ~(1u << 5);
    }
}


///*
//************************************************************
//*	�������ƣ�	UART3_IRQHandler
//*	�������ܣ�	����3�жϴ�����
//*	��ڲ�����	��
//*	���ز�����	��	
//************************************************************
//*/
//void USART3_IRQHandler(void)
//{
////	uint8_t data = 0;
//	
//    if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET){
//		
//		if(vd_usart3_callback_handle != NULL){
//			vd_usart3_callback_handle();
//		}else{
////			data = USART_ReceiveData(USART3);
//			//cycle_buff_push_bytes(&vd_usart3_rx_cycbuff, &data, 1);
//			//vd_usart3_rx_cycbuff.len++;
//		}
//		
//        USART_ClearITPendingBit(USART3, USART_FLAG_RXNE);
//    }
//}



	uint8_t test_number = 0, test_rx_flag = 0, USART_number = 0;

	void vd_bsp_usart_test_callback_handle(void)
	{
		test_number = vd_bsp_usart_ReceiveByte(USART_number);
		test_number++;
		test_rx_flag = 1;	
	}

	void vd_bsp_usart_test_init(VD_USARTx_E USARTx)
	{
		USART_number = USARTx;
		vd_bsp_usart_init(USARTx);
		vd_bsp_usart_register_rx_callback_handle(USARTx, vd_bsp_usart_test_callback_handle);	
	}

	void vd_bsp_usart_test(void)
	{
		if(test_rx_flag == 1){
			test_rx_flag = 0;
			vd_bsp_usart_SendByte(USART_number, test_number);	
		}
	}

