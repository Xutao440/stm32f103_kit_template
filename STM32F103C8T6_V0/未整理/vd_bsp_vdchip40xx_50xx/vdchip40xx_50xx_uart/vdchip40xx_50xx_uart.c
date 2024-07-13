/*
 * Project: mcu-kit
 * Module: uart driver
 * File: vd_bsp_uart.c
 * Created Date: 2022-03-23 10:44:19
 * Author: 
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)

/* ======================================================================================
 * includes
 */
#include "vdchip40xx_50xx_uart.h"
#include "driver_uart_ex.h"
#include "driver_uart.h"
#include "vd_log.h"

typedef struct{
	vd_bsp_port_e								uart_id;
	UART_HandleTypeDef 					handle;				
	vd_uint8_t 									*rx_buf;
	vd_uint32_t									rx_buf_size;
	vd_bsp_uart_rx_callback_t 	rx_cb;
	vd_void_t										*rx_cb_arg;
	vd_bsp_uart_tx_callback_t 	tx_cb;
	vd_void_t										*tx_cb_arg;
}fr8000_uart_t;

static fr8000_uart_t fr8000_uart[2] = {
	{
		.uart_id = VD_PORT_UART0,
		.handle.UARTx = Uart0,
	},
	{
		.uart_id = VD_PORT_UART1,
		.handle.UARTx = Uart1,
	}
};

/**
 * @brief  初始化
 * 
 * @param uart 
 * @return vd_bool_t
 */
vd_bool_t vd_fr8000_uart_init(vd_bsp_uart_t *uart) 
{
	UART_HandleTypeDef *huartx = vd_null;
	
	vd_check_return_val_error(uart == vd_null, vd_false, "in func: %s   line: %d", __func__, __LINE__);
	
	log_debug("baud = %d\r\n", uart->baud);
	log_debug("data_bit = %d\r\n", uart->data_bits);
	log_debug("stop_bits = %d\r\n", uart->stop_bits);
	log_debug("parity = %d\r\n", uart->parity);
	log_debug("mode = %d\r\n", uart->mode);
	log_debug("tx_pin = %d\r\n", uart->tx_pin);
	log_debug("rx_pin = %d\r\n", uart->rx_pin);
	
	
	if(uart->id == VD_PORT_UART0){
		
		//引脚配置
		if(uart->rx_pin >= 0 && uart->rx_pin <= 32){//PA0 ~ PE0
			vd_check_return_val_error((uart->rx_pin % 4 != 0), vd_false, "in func: %s   line: %d", __func__, __LINE__);//串口0的rx pin只能被4整除
			system_set_port_mux(uart->rx_pin / 8, uart->rx_pin % 8, 4);
		}
		if(uart->tx_pin >= 1 && uart->tx_pin <= 33){//PA1 ~ PE1
			vd_check_return_val_error(((uart->tx_pin - 1) % 4 != 0), vd_false, "in func: %s   line: %d", __func__, __LINE__);//串口0的tx pin - 1只能被4整除
			system_set_port_mux(uart->tx_pin / 8, uart->tx_pin % 8, 4);
		}
		
		//时钟使能
		__SYSTEM_UART0_CLK_SELECT_96M();
		__SYSTEM_UART0_CLK_ENABLE();
		
		huartx = &fr8000_uart[0].handle;
		
	}else if(uart->id == VD_PORT_UART1){
		
		//引脚配置
		if(uart->rx_pin >= 2 && uart->rx_pin <= 32){//PA2 ~ PE0
			vd_check_return_val_error(((uart->rx_pin - 2) % 4 != 0) && (uart->rx_pin != 32), vd_false, "in func: %s   line: %d", __func__, __LINE__);//串口1的rx pin - 2只能被4整除，特殊:PE0(32)
			system_set_port_mux(uart->rx_pin / 8, uart->rx_pin % 8, 5);
		}
		if(uart->tx_pin >= 3 && uart->tx_pin <= 33){//PA3 ~ PE1
			vd_check_return_val_error(((uart->tx_pin - 3) % 4 != 0) && (uart->tx_pin != 33), vd_false, "in func: %s   line: %d", __func__, __LINE__);//串口1的rx pin - 3只能被4整除，特殊:PE1(33)
			system_set_port_mux(uart->tx_pin / 8, uart->tx_pin % 8, 5);
		}
		
		//时钟使能
		 __SYSTEM_UART1_CLK_ENABLE();
		
		huartx = &fr8000_uart[1].handle;
	}
	
	vd_check_return_val_error(huartx == vd_null, vd_false, "in func: %s   line: %d", __func__, __LINE__);

	huartx->Init.BaudRate   = uart->baud;
	huartx->Init.DataLength = uart->data_bits;
	huartx->Init.StopBits   = uart->stop_bits;
	huartx->Init.Parity     = uart->parity;
	huartx->Init.FIFO_Mode  = UART_FIFO_DISABLE;
	uart_init_ex(huartx);
	

//				if(uart->mode == VD_UART_IT_MODE){
//					uart0_cb.rx_cplt_callback = vd_uart_cfg->rx_it_cb;
//					uart0_cb.rx_cb_arg = vd_uart_cfg->arg;
//					NVIC_EnableIRQ(UART0_IRQn);
//					NVIC_SetPriority(UART0_IRQn, 0);
//					huart0.RxCpltCallback = (volatile void (*)(struct __UART_HandleTypeDef *huart))uart_rx_cplt_handler;//强转防止警告
//					uart_receive_IT(&huart0, &uart_rx_it_temp[0], 1);//接收到一字节产生中断         
//				}
			
	return vd_true;
}

/**
 * @brief  通过id 查找串口
 * 
 * @param uart_id 			串口id
 * @return fr8000_uart_t	
 */
static fr8000_uart_t *vd_fr8000_uart_find(vd_bsp_port_e uart_id)
{
	vd_uint8_t i;
	
	for(i = 0; i < vd_array_size(fr8000_uart); i++){
		if(uart_id == fr8000_uart[i].uart_id){
			return &fr8000_uart[i];
		}
	}
	
	return vd_null;
}

/**
 * @brief  阻塞方式读
 * 
 * @param uart 			串口
 * @param data 			数据缓存
 * @param data_len 	读取长度
 * @param outtime 	超时时间
 * @return vd_int32_t	非负值 -- 实际读取长度
											负值	 -- 读取失败		
 */
vd_int32_t vd_fr8000_uart_read(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime)
{
	fr8000_uart_t *uartx = vd_null;
	
	vd_check_return_val_error(uart == vd_null, -1, "in func: %s   line: %d", __func__, __LINE__);
	
	uartx = vd_fr8000_uart_find(uart->id);
	vd_check_return_val_error(uartx == vd_null, -1, "in func: %s   line: %d", __func__, __LINE__);
	
	return uart_receive(&uartx->handle, data, data_len, outtime);
}

/**
 * @brief  串口中断接收完成回调
 * 
 * @param huart 			串口句柄
 * @return vd_void_t		
 */
static vd_void_t vd_fr8000_uart_rx_it_cplt_callback(UART_HandleTypeDef *huart)
{
	fr8000_uart_t *uartx = vd_null;

	if(huart == &fr8000_uart[0].handle){
		uartx = &fr8000_uart[0];		
	}
	else if(huart == &fr8000_uart[1].handle){
		uartx = &fr8000_uart[1];
	}
	
	vd_check_return(uartx == vd_null);
	
	if(uartx->rx_cb){
		uartx->rx_cb(uartx->uart_id, uartx->rx_buf, uartx->rx_buf_size, uartx->rx_cb_arg);
	}
	
	uart_receive_IT(&uartx->handle, uartx->rx_buf, uartx->rx_buf_size);
}

/**
 * @brief  中断方式读
 * 
 * @param uart_id 			串口号
 * @param cb 						回调函数
 * @param arg 					回调参数
 * @return vd_bool_t		
 */
vd_bool_t vd_fr8000_uart_read_it(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg)
{
	fr8000_uart_t *uartx = vd_null;
	
	vd_check_return_val_error(uart == vd_null, vd_false, "in func: %s   line: %d", __func__, __LINE__);

	uartx = vd_fr8000_uart_find(uart->id);
	vd_check_return_val_error(uartx == vd_null, vd_false, "in func: %s   line: %d", __func__, __LINE__);
	
	if(uartx->handle.b_RxBusy){//之前开启过串口接收
		__UART_INT_RX_DISABLE(uartx->handle.UARTx);
		uartx->handle.b_RxBusy = false;
	}
	
	uartx->handle.RxCpltCallback = (volatile void (*)(struct __UART_HandleTypeDef *huart))vd_fr8000_uart_rx_it_cplt_callback;//强转防止警告
	
	uartx->rx_buf = data;
	uartx->rx_buf_size = data_len;
	uartx->rx_cb = cb;
	uartx->rx_cb_arg = arg;
	
	uart_receive_IT(&uartx->handle, uartx->rx_buf, uartx->rx_buf_size);
	
    if(uart->id == VD_PORT_UART0){
     NVIC_EnableIRQ(UART0_IRQn);
	 NVIC_SetPriority(UART0_IRQn, 0);
    }else if(uart->id == VD_PORT_UART1){
     NVIC_EnableIRQ(UART1_IRQn);
	 NVIC_SetPriority(UART1_IRQn, 0);
    }
	
	
	return vd_true;
}

/**
 * @brief  串口DMA接收完成回调
 * 
 * @param huart 			串口句柄
 * @return vd_void_t		
 */
static vd_void_t vd_fr8000_uart_rx_dma_cplt_callback(UART_HandleTypeDef *huart)
{
	fr8000_uart_t *uartx = vd_null;

	if(huart == &fr8000_uart[0].handle){
		uartx = &fr8000_uart[0];		
	}
	else if(huart == &fr8000_uart[1].handle){
		uartx = &fr8000_uart[1];
	}
	
	vd_check_return(uartx == vd_null);
	
	//dma接收字节计算
	
//	if(uartx->rx_cb){
//		uartx->rx_cb(uartx->uart_id, uartx->rx_buf, uartx->rx_buf_size, uartx->rx_cb_arg);
//	}
//	
	
	//重新打开dma接收
//	uart_receive_IT(&uartx->handle, uartx->rx_buf, uartx->rx_buf_size);
}

/**
 * @brief  DMA方式读
 * 
 * @param uart_id 			串口号
 * @param cb 						回调函数
 * @param arg 					回调参数
 * @return vd_bool_t		
 */
vd_bool_t vd_fr8000_uart_read_dma(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg)
{
	fr8000_uart_t *uartx = vd_null;
	
	vd_check_return_val_error(uart == vd_null, vd_false, "in func: %s   line: %d", __func__, __LINE__);

	uartx = vd_fr8000_uart_find(uart->id);
	vd_check_return_val_error(uartx == vd_null, vd_false, "in func: %s   line: %d", __func__, __LINE__);
	
	if(uartx->handle.b_RxBusy){//之前开启过串口接收
		__UART_INT_RX_DISABLE(uartx->handle.UARTx);
		uartx->handle.b_RxBusy = vd_false;
	}
	
	uartx->rx_buf = data;
	uartx->rx_buf_size = data_len;
	
//	uart_receive_IT(&uartx->handle, uartx->rx_buf, uartx->rx_buf_size);
	
	if(uart->id == VD_PORT_UART0){
     NVIC_EnableIRQ(UART0_IRQn);
	 NVIC_SetPriority(UART0_IRQn, 0);
    }else if(uart->id == VD_PORT_UART1){
     NVIC_EnableIRQ(UART1_IRQn);
	 NVIC_SetPriority(UART1_IRQn, 0);
    }
	
	return vd_false;
}




/**
 * @brief  阻塞方式写
 * 
 * @param uart 			串口
 * @param data 			数据缓存
 * @param data_len 	读取长度
 * @return vd_int32_t	非负值 -- 实际写入长度
											负值	 -- 读取失败	
 */
vd_int32_t vd_fr8000_uart_write(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime)
{
	fr8000_uart_t *uartx = vd_null;
	
	vd_check_return_val_error(uart == vd_null, -1, "in func: %s   line: %d", __func__, __LINE__);

	uartx = vd_fr8000_uart_find(uart->id);
	vd_check_return_val_error(uartx == vd_null, -1, "in func: %s   line: %d", __func__, __LINE__);
	
	uart_transmit(&uartx->handle, data, data_len);
	
	return data_len;
} 

/**
 * @brief  串口发送完成回调
 * 
 * @param huart 			串口句柄
 * @return vd_void_t		
 */
static vd_void_t vd_fr8000_uart_tx_cplt_callback(UART_HandleTypeDef *huart)
{
	fr8000_uart_t *uartx = vd_null;

	if(huart == &fr8000_uart[0].handle){
		uartx = &fr8000_uart[0];		
	}
	else if(huart == &fr8000_uart[1].handle){
		uartx = &fr8000_uart[1];
	}
	
	vd_check_return(uartx == vd_null);
	
	if(uartx->tx_cb){
		uartx->tx_cb(uartx->uart_id, uartx->tx_cb_arg);
		uartx->tx_cb = vd_null;
		uartx->tx_cb_arg = vd_null;
	}
}

/**
 * @brief  中断方式写
 * 
 * @param uart 			串口
 * @param data 			数据缓存
 * @param data_len 	读取长度
 * @param cb 				回调函数
 * @param arg 			回调参数
 * @return vd_bool_t	
 */
vd_bool_t vd_fr8000_uart_write_it(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg)
{
	fr8000_uart_t *uartx = vd_null;
	
	vd_check_return_val_error(uart == vd_null, -1, "in func: %s   line: %d", __func__, __LINE__);

	uartx = vd_fr8000_uart_find(uart->id);
	vd_check_return_val_error(uartx == vd_null, -1, "in func: %s   line: %d", __func__, __LINE__);
	
	if(uartx->handle.b_TxBusy){
		__UART_INT_TXE_DISABLE(uartx->handle.UARTx);
		uartx->handle.b_TxBusy = vd_false;
	}
	
	uartx->tx_cb = cb;
	uartx->tx_cb_arg = arg;
	uartx->handle.TxCpltCallback = (volatile void (*)(struct __UART_HandleTypeDef *huart))vd_fr8000_uart_tx_cplt_callback;//强转防止警告
	
	uart_transmit_IT(&uartx->handle, data, data_len);
	
	  if(uart->id == VD_PORT_UART0){
     NVIC_EnableIRQ(UART0_IRQn);
	 NVIC_SetPriority(UART0_IRQn, 0);
    }else if(uart->id == VD_PORT_UART1){
     NVIC_EnableIRQ(UART1_IRQn);
	 NVIC_SetPriority(UART1_IRQn, 0);
    }
	
	return vd_true;
}

/**
 * @brief  DMA方式写
 * 
 * @param uart 			串口
 * @param data 			数据缓存
 * @param data_len 	读取长度
 * @param cb 				回调函数
 * @param arg 			回调参数
 * @return vd_bool_t	
 */
vd_bool_t vd_fr8000_uart_write_dma(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg)
{
	

	
	return vd_false;
}


/**
 * @brief  驱动注册
 * 
 * @param ops 				驱动接口
 * @return vd_bool_t	结果
 */
vd_bool_t vd_fr8000_uart_register(vd_bsp_uart_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null, vd_false, "in func: %s   line: %d", __func__, __LINE__);
	
	ops->init = vd_fr8000_uart_init;
	ops->read = vd_fr8000_uart_read;
	ops->read_it = vd_fr8000_uart_read_it;
	ops->read_dma = vd_fr8000_uart_read_dma;
	ops->write= vd_fr8000_uart_write;
	ops->write_it = vd_fr8000_uart_write_it;
	ops->write_dma = vd_fr8000_uart_write_dma;
	
	return vd_true;
}




void uart0_isr(void)
{
//	static int ir_cnt = 0;
//	log_debug("ir_cnt = %d\n", ++ir_cnt);
//	
//	if(huart0.UARTx->FCR_IID.IID == 0x4){
//		uart_rx_it_temp[0] = huart0.UARTx->DATA_DLL.DATA;
//		uart0_cb.rx_cplt_callback(&uart_rx_it_temp[0], 1, uart0_cb.rx_cb_arg);
//	}
	
  uart_IRQHandler(&fr8000_uart[0].handle);
}

void uart1_isr(void)
{
  uart_IRQHandler(&fr8000_uart[1].handle);
}
