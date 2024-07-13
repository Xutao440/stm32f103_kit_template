#include "vd_bsp_uart.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"
#include "string.h"

usart_callback_handle vd_usart1_callback_handle = NULL;
usart_callback_handle vd_usart2_callback_handle = NULL;
usart_callback_handle vd_usart3_callback_handle = NULL;

//cycbuf_t vd_usart1_rx_cycbuff;
//cycbuf_t vd_usart2_rx_cycbuff;
//cycbuf_t vd_usart3_rx_cycbuff;

/*
************************************************************
*	函数名称：	vd_bsp_usart_init
*	函数功能：	串口初始化
*	入口参数：	USARTx：串口号
*	返回参数：	无	
************************************************************
*/
uint8_t vd_bsp_usart_init(VD_USARTx_E USARTx)
{
	uint32_t gpio_ahb_periph = 0, vd_baudrate = 0;
	GPIO_InitTypeDef  GPIO_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStruct;
	USART_TypeDef     *vd_usart;
	GPIO_TypeDef 	  *vd_tx_port, *vd_rx_port; 
	uint16_t          vd_tx_pin,   vd_rx_pin, vd_usart_mode = 0;
	uint8_t 		  vd_priority, vd_erro = 0, vd_usart_irqn = 0;
	
//	memset(&vd_usart1_rx_cycbuff, 0, sizeof(cycbuf_t));
//	memset(&vd_usart2_rx_cycbuff, 0, sizeof(cycbuf_t));
//	memset(&vd_usart3_rx_cycbuff, 0, sizeof(cycbuf_t));
	
	/*端口参数赋值*/
	if(USARTx == VD_USART1){
		vd_tx_port = USART1_TX_PORT; vd_rx_port = USART1_RX_PORT;
		vd_tx_pin = USART1_TX_PIN;   vd_rx_pin = USART1_RX_PIN;		
		vd_baudrate = USART1_BAUDRATE;
		vd_priority = USART1_PRIORITY;
		vd_usart = USART1;
		vd_usart_mode = USART1_MODE;
		vd_usart_irqn = USART1_IRQn;
		/*UART时钟*/	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		//creat_cycle_buff(&vd_usart1_rx_cycbuff, 100);
	}else if(USARTx == VD_USART2){
		vd_tx_port = USART2_TX_PORT; vd_rx_port = USART2_RX_PORT;
		vd_tx_pin = USART2_TX_PIN;   vd_rx_pin = USART2_RX_PIN;		
		vd_baudrate = USART2_BAUDRATE;
		vd_priority = USART2_PRIORITY;
		vd_usart = USART2;
		vd_usart_mode = USART2_MODE;
		vd_usart_irqn = USART2_IRQn;
		/*UART时钟*/	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		//creat_cycle_buff(&vd_usart2_rx_cycbuff, 100);
	}else if(USARTx == VD_USART3){
		vd_tx_port = USART3_TX_PORT; vd_rx_port = USART3_RX_PORT;
		vd_tx_pin = USART3_TX_PIN;   vd_rx_pin = USART3_RX_PIN;		
		vd_baudrate = USART3_BAUDRATE;
		vd_priority = USART3_PRIORITY;
		vd_usart = USART3;
		vd_usart_mode = USART3_MODE;
		vd_usart_irqn = USART3_IRQn;		
		GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE); //USART3 remap 		
		/*UART时钟*/	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //复用时钟开启	
		//creat_cycle_buff(&vd_usart3_rx_cycbuff, 100);		
	}else{
		vd_erro = 1;
		goto erro;
	}

	/*GPIO时钟*/
	gpio_ahb_periph = vd_bsp_gpio_get_PeriphClock(vd_tx_port);
	RCC_APB2PeriphClockCmd(gpio_ahb_periph, ENABLE);     		//TX时钟	
	gpio_ahb_periph = vd_bsp_gpio_get_PeriphClock(vd_rx_port);
	RCC_APB2PeriphClockCmd(gpio_ahb_periph, ENABLE);     		//RX时钟

	//GPIO_TX
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = vd_tx_pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(vd_tx_port, &GPIO_InitStruct);
	
	//GPIO_RX
    GPIO_InitStruct.GPIO_Pin  = vd_rx_pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(vd_rx_port, &GPIO_InitStruct);
	
		
	USART_InitStruct.USART_BaudRate = vd_baudrate;   //波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = vd_usart_mode;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(vd_usart, &USART_InitStruct);
	
	
	//中断配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = vd_usart_irqn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = vd_priority;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(vd_usart, USART_IT_RXNE, ENABLE);
	USART_Cmd(vd_usart, ENABLE);	
	
	
	erro:
		return vd_erro;
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
		//data_len = vd_usart1_rx_cycbuff.len;
		
	}else if(USARTx == VD_USART2){
		//data_len = vd_usart2_rx_cycbuff.len;
		
	}else if(USARTx == VD_USART3){
		//data_len = vd_usart3_rx_cycbuff.len;
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
		//ret_value = cycle_buff_pull_bytes(&vd_usart1_rx_cycbuff, buff, len);
		
	}else if(USARTx == VD_USART2){
		//ret_value = cycle_buff_pull_bytes(&vd_usart2_rx_cycbuff, buff, len);

	}else if(USARTx == VD_USART3){
		//ret_value = cycle_buff_pull_bytes(&vd_usart3_rx_cycbuff, buff, len);	
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
	
	return USART_ReceiveData(vd_usart);
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
	
	while(USART_GetFlagStatus(vd_usart, USART_FLAG_TC) == RESET){
		timeout++;
		if(timeout > 5000){
			break;
		}	
	};
	
	USART_SendData(vd_usart, (u16)ch);
 
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
//	uint8_t data = 0;
	
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET){
		
		if(vd_usart1_callback_handle != NULL){
			vd_usart1_callback_handle();
		}else{
//			data = USART_ReceiveData(USART1);
			//cycle_buff_push_bytes(&vd_usart1_rx_cycbuff, &data, 1);
			//vd_usart1_rx_cycbuff.len++;
		}

        USART_ClearITPendingBit(USART1, USART_FLAG_RXNE);
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
//	uint8_t data = 0;
	
    if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET){
		
		if(vd_usart2_callback_handle != NULL){
			vd_usart2_callback_handle();
		}else{
//			data = USART_ReceiveData(USART2);
			//cycle_buff_push_bytes(&vd_usart2_rx_cycbuff, &data, 1);
			//vd_usart2_rx_cycbuff.len++;
		}
		
        USART_ClearITPendingBit(USART2, USART_FLAG_RXNE);
    }
}


/*
************************************************************
*	函数名称：	UART3_IRQHandler
*	函数功能：	串口3中断处理函数
*	入口参数：	无
*	返回参数：	无	
************************************************************
*/
void USART3_IRQHandler(void)
{
//	uint8_t data = 0;
	
    if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET){
		
		if(vd_usart3_callback_handle != NULL){
			vd_usart3_callback_handle();
		}else{
//			data = USART_ReceiveData(USART3);
			//cycle_buff_push_bytes(&vd_usart3_rx_cycbuff, &data, 1);
			//vd_usart3_rx_cycbuff.len++;
		}
		
        USART_ClearITPendingBit(USART3, USART_FLAG_RXNE);
    }
}


/*bsp 串口测试函数*/
#if BSP_UART_TEST
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
#endif
