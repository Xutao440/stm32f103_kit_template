#include "string.h"
#include "bsp_cfg.h"

usart_callback_handle vd_usart1_callback_handle = NULL;
usart_callback_handle vd_usart2_callback_handle = NULL;
usart_callback_handle vd_usart3_callback_handle = NULL;
//接收缓存队列
queue_t  vd_usart1_rx_queue;
queue_t  vd_usart2_rx_queue;
queue_t  vd_usart3_rx_queue;
/*
************************************************************
*	函数名称：	vd_bsp_usart_init
*	函数功能：	串口初始化
*	入口参数：	USARTx：串口号
*	返回参数：	无	
************************************************************
*/
vd_int8_t vd_bsp_usart_init(VD_USARTx_E USARTx)
{
	vd_int8_t ret = 0;
	vd_uint8_t div = 1;   //分频系数
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
					vd_bsp_gpio_init(USART1_RX_PORT, VD_GPIO_IPU);    //RX  输入上拉
					ret = creat_queue(&vd_usart1_rx_queue, 64); 
					if(ret !=0){
						log_error("%s: creat queue fail !",__FUNCTION__);
						goto erro;
					}
				}
				if(USART1_MODE & VD_UART_TX){
					vd_bsp_gpio_init(USART1_TX_PORT, VD_GPIO_AF_PP);  //TX  复用推挽
				}
				RCC->APB2ENR|=1<<14;  //使能串口1时钟 
				baud = USART1_BAUDR;
			#endif
			break;
		case VD_USART2:
			#if USART2_ENABLE
				div  = 2;
				if(USART2_MODE & VD_UART_RX){
					vd_bsp_gpio_init(USART2_RX_PORT, VD_GPIO_IPU);    //RX  输入上拉
				}
				if(USART2_MODE & VD_UART_TX){
					vd_bsp_gpio_init(USART2_TX_PORT, VD_GPIO_AF_PP);  //TX  复用推挽
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
	
	//波特率设置		
	temp=(float)(72/div*1000000)/(baud*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
    	   	RCC->APB2RSTR|=1<<14;   //复位串口1
			RCC->APB2RSTR&=~(1<<14);//停止复位	
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
	
    USART1->CR1|= 0X01<<5;    //接收缓冲区非空中断使能	    	
	VD_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
erro:	
	return ret;
}


/*
************************************************************
*	函数名称：	vd_bsp_usart_get_receive_data_len
*	函数功能：	获取接收到的数据长度
*	入口参数：	USARTx: 串口号
*	返回参数：	数据长度	
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
*	函数名称：	vd_bsp_usart_read_bytes
*	函数功能：	串口接收一个字节
*	入口参数：	USARTx: 串口号
*	返回参数：	0：读成功，<0：读失败	
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
*	函数名称：	vd_bsp_usart_ReceiveByte
*	函数功能：	串口接收一个字节
*	入口参数：	USARTx: 串口号
*	返回参数：	接收到字节	
************************************************************
*/
char vd_bsp_usart_ReceiveByte(VD_USARTx_E USARTx)
{
	USART_TypeDef     *vd_usart;
	
	/*端口参数赋值*/
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
*	函数名称：	vd_bsp_usart_SendByte
*	函数功能：	串口发送一个字节
*	入口参数：	USARTx: 串口号，ch：数据
*	返回参数：	无	
************************************************************
*/
void vd_bsp_usart_SendByte(VD_USARTx_E USARTx, char ch)
{
	uint16_t timeout = 0;
	USART_TypeDef  *vd_usart;
	
	/*端口参数赋值*/
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
*	函数名称：	vd_bsp_usart_SendBytes
*	函数功能：	串口发送多字节
*	入口参数：	USARTx: 串口号，data：数据， len：数据长度
*	返回参数：	无	
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
*	函数名称：	vd_bsp_usart_SendString
*	函数功能：	串口发送一个字符串
*	入口参数：	USARTx: 串口号，String：字符串
*	返回参数：	无	
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
*	函数名称：	vd_bsp_usart_register_rx_callback_handle
*	函数功能：	注册串口接收回调函数
*	入口参数：	USARTx: 串口号， callback_handle：回调函数
*	返回参数：	成功返回0， 失败返回>0
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
*	函数名称：	UART1_IRQHandler
*	函数功能：	串口1中断处理函数
*	入口参数：	无
*	返回参数：	无	
************************************************************
*/
void USART1_IRQHandler(void)
{
	uint8_t data = 0;
	
    if (USART1->SR & (1u << 5)){ //bit5: RXNE：读数据寄存器非空 
		
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
*	函数名称：	UART2_IRQHandler
*	函数功能：	串口2中断处理函数
*	入口参数：	无
*	返回参数：	无	
************************************************************
*/
void USART2_IRQHandler(void)
{
	uint8_t data = 0;
	
   if (USART2->SR & (1u << 5)){ //bit5: RXNE：读数据寄存器非空 
		
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
//*	函数名称：	UART3_IRQHandler
//*	函数功能：	串口3中断处理函数
//*	入口参数：	无
//*	返回参数：	无	
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

