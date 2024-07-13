/*
 * Project: mcu-kit
 * Module: uart driver
 * File: vd_bsp_uart.h
 * Created Date: 2022-03-23 10:45:36
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _VD_BSP_UART_H
#define _VD_BSP_UART_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "vd_bsp_type.h"



/* 串口模式 */
typedef enum {
	UART_MODE_TX,		//发送模式
	UART_MODE_RX,		//接收模式
	UART_MODE_TXRX,	//收发模式
	UART_MODE_MAX,
}vd_uart_mode_e;

/* 波特率 */
typedef enum {
	UART_BAUD_2400 = 2400,
	UART_BAUD_4800 = 4800,
	UART_BAUD_9600 = 9600,
	UART_BAUD_19200 = 19200,
	UART_BAUD_38400 = 38400,
	UART_BAUD_57600 = 57600,
	UART_BAUD_115200 = 115200,
	UART_BAUD_230400 = 230400,
	UART_BAUD_460800 = 460800,
	UART_BAUD_921600 = 921600,
	UART_BAUD_2000000 = 2000000,
}vd_uart_baud_e;

/* 数据位 */
typedef enum {
	UART_DATA_BITS_5,
	UART_DATA_BITS_6,
	UART_DATA_BITS_7,
	UART_DATA_BITS_8,
	UART_DATA_BITS_MAX,
}vd_uart_data_e;

/* 停止位 */
typedef enum {
	UART_STOP_BITS_1,
	UART_STOP_BITS_2,
	UART_STOP_BITS_MAX,
}vd_uart_stop_e;

/* 校验位 */
typedef enum {
	UART_PARITY_BIT_NONE,	//无奇偶校验位
	UART_PARITY_BIT_ODD,	//奇校验
	UART_PARITY_BIT_EVEN,	//偶校验
	UART_PARITY_BIT_MAX,
}vd_uart_parity_e;

/* 串口接收完成回调 */
typedef vd_void_t (*vd_bsp_uart_rx_callback_t)(vd_bsp_port_e uart_id, vd_uint8_t *data, vd_uint16_t data_len, vd_void_t *arg);
/* 串口发送完成回调 */
typedef vd_void_t (*vd_bsp_uart_tx_callback_t)(vd_bsp_port_e uart_id, vd_void_t *arg);

/* 串口参数 */
typedef struct{
	vd_bsp_port_e 		id;					//串口号
	vd_bsp_port_e 		tx_pin;			//发送引脚
	vd_bsp_port_e 		rx_pin;			//接收引脚
	vd_uart_baud_e 		baud;				//波特率
	vd_uart_mode_e		mode;				//模式
	vd_uart_data_e		data_bits;	//数据位
	vd_uart_stop_e		stop_bits;	//停止位
	vd_uart_parity_e	parity;			//校验位
}vd_bsp_uart_t;

/**串口驱动接口**/
typedef struct
{
	vd_bool_t 	(*init)(vd_bsp_uart_t *uart);//初始化
	vd_int32_t 	(*read)(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime);//阻塞方式读
	vd_bool_t 	(*read_it)(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg);//中断方式读
	vd_bool_t 	(*read_dma)(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg);//DMA方式读
	vd_int32_t 	(*write) (vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime);//阻塞方式写
	vd_bool_t 	(*write_it)(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg);//中断方式写
	vd_bool_t 	(*write_dma) (vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg);//DMA方式写
}vd_bsp_uart_ops_t;

/* 串口注册接口 */
typedef vd_bool_t (*vd_bsp_uart_register_t)(vd_bsp_uart_ops_t *ops);



/**
 * @brief 串口初始化
 * 
 * @param uart_id  		串口id
 * @return 	非负值：	成功
						负值：		错误码 		@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_init( vd_bsp_port_e  uart_id);

/**
 * @brief 阻塞方式读
 * 
 * @param uart_id  			串口id
 * @param data 					数据缓存
 * @param data_len 			数据长度
 * @param outtime 			超时时间
 * @return 	非负值：		实际读取长度
						负值：			错误码 		@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_read(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime);

/**
 * @brief  中断方式读
 * 
 * @param uart_id 			串口号
 * @param cb 						回调函数
 * @param arg 					回调参数
 * @return vd_int32_t		错误码	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_read_it(vd_bsp_port_e uart_id, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg);

/**
 * @brief  DMA方式读
 * 
 * @param uart_id 			串口号
 * @param cb 						回调函数
 * @param arg 					回调参数
 * @return vd_int32_t		错误码	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_read_dma(vd_bsp_port_e uart_id, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg);

/**
 * @brief  阻塞方式写
 * 
 * @param uart_id 			串口号
 * @param data 					数据缓存
 * @param data_len 			数据长度
 * @param outtime 			超时时间
 * @return vd_int32_t		非负值 -- 实际写入长度
												负值	 -- 错误码	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_write(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime);

/**
 * @brief  中断方式写
 * 
 * @param uart_id 			串口号
 * @param data 					数据缓存
 * @param data_len 			数据长度
 * @param cb 						回调函数
 * @param arg 					回调参数
 * @return vd_int32_t		非负值 -- 实际写入长度
												负值	 -- 错误码	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_write_it(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg);

/**
 * @brief  DMA方式写
 * 
 * @param uart_id 			串口号
 * @param data 					数据缓存
 * @param data_len 			数据长度
 * @param cb 						回调函数
 * @param arg 					回调参数
 * @return vd_int32_t		非负值 -- 实际写入长度
												负值	 -- 错误码	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_write_dma(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg);




__vd_extern_c_leave__
#endif  // _DRV_UART_H
