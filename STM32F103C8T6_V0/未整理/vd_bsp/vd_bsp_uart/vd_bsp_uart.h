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



/* ����ģʽ */
typedef enum {
	UART_MODE_TX,		//����ģʽ
	UART_MODE_RX,		//����ģʽ
	UART_MODE_TXRX,	//�շ�ģʽ
	UART_MODE_MAX,
}vd_uart_mode_e;

/* ������ */
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

/* ����λ */
typedef enum {
	UART_DATA_BITS_5,
	UART_DATA_BITS_6,
	UART_DATA_BITS_7,
	UART_DATA_BITS_8,
	UART_DATA_BITS_MAX,
}vd_uart_data_e;

/* ֹͣλ */
typedef enum {
	UART_STOP_BITS_1,
	UART_STOP_BITS_2,
	UART_STOP_BITS_MAX,
}vd_uart_stop_e;

/* У��λ */
typedef enum {
	UART_PARITY_BIT_NONE,	//����żУ��λ
	UART_PARITY_BIT_ODD,	//��У��
	UART_PARITY_BIT_EVEN,	//żУ��
	UART_PARITY_BIT_MAX,
}vd_uart_parity_e;

/* ���ڽ�����ɻص� */
typedef vd_void_t (*vd_bsp_uart_rx_callback_t)(vd_bsp_port_e uart_id, vd_uint8_t *data, vd_uint16_t data_len, vd_void_t *arg);
/* ���ڷ�����ɻص� */
typedef vd_void_t (*vd_bsp_uart_tx_callback_t)(vd_bsp_port_e uart_id, vd_void_t *arg);

/* ���ڲ��� */
typedef struct{
	vd_bsp_port_e 		id;					//���ں�
	vd_bsp_port_e 		tx_pin;			//��������
	vd_bsp_port_e 		rx_pin;			//��������
	vd_uart_baud_e 		baud;				//������
	vd_uart_mode_e		mode;				//ģʽ
	vd_uart_data_e		data_bits;	//����λ
	vd_uart_stop_e		stop_bits;	//ֹͣλ
	vd_uart_parity_e	parity;			//У��λ
}vd_bsp_uart_t;

/**���������ӿ�**/
typedef struct
{
	vd_bool_t 	(*init)(vd_bsp_uart_t *uart);//��ʼ��
	vd_int32_t 	(*read)(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime);//������ʽ��
	vd_bool_t 	(*read_it)(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg);//�жϷ�ʽ��
	vd_bool_t 	(*read_dma)(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg);//DMA��ʽ��
	vd_int32_t 	(*write) (vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime);//������ʽд
	vd_bool_t 	(*write_it)(vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg);//�жϷ�ʽд
	vd_bool_t 	(*write_dma) (vd_bsp_uart_t *uart, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg);//DMA��ʽд
}vd_bsp_uart_ops_t;

/* ����ע��ӿ� */
typedef vd_bool_t (*vd_bsp_uart_register_t)(vd_bsp_uart_ops_t *ops);



/**
 * @brief ���ڳ�ʼ��
 * 
 * @param uart_id  		����id
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_init( vd_bsp_port_e  uart_id);

/**
 * @brief ������ʽ��
 * 
 * @param uart_id  			����id
 * @param data 					���ݻ���
 * @param data_len 			���ݳ���
 * @param outtime 			��ʱʱ��
 * @return 	�Ǹ�ֵ��		ʵ�ʶ�ȡ����
						��ֵ��			������ 		@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_read(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime);

/**
 * @brief  �жϷ�ʽ��
 * 
 * @param uart_id 			���ں�
 * @param cb 						�ص�����
 * @param arg 					�ص�����
 * @return vd_int32_t		������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_read_it(vd_bsp_port_e uart_id, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg);

/**
 * @brief  DMA��ʽ��
 * 
 * @param uart_id 			���ں�
 * @param cb 						�ص�����
 * @param arg 					�ص�����
 * @return vd_int32_t		������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_read_dma(vd_bsp_port_e uart_id, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg);

/**
 * @brief  ������ʽд
 * 
 * @param uart_id 			���ں�
 * @param data 					���ݻ���
 * @param data_len 			���ݳ���
 * @param outtime 			��ʱʱ��
 * @return vd_int32_t		�Ǹ�ֵ -- ʵ��д�볤��
												��ֵ	 -- ������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_write(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime);

/**
 * @brief  �жϷ�ʽд
 * 
 * @param uart_id 			���ں�
 * @param data 					���ݻ���
 * @param data_len 			���ݳ���
 * @param cb 						�ص�����
 * @param arg 					�ص�����
 * @return vd_int32_t		�Ǹ�ֵ -- ʵ��д�볤��
												��ֵ	 -- ������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_write_it(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg);

/**
 * @brief  DMA��ʽд
 * 
 * @param uart_id 			���ں�
 * @param data 					���ݻ���
 * @param data_len 			���ݳ���
 * @param cb 						�ص�����
 * @param arg 					�ص�����
 * @return vd_int32_t		�Ǹ�ֵ -- ʵ��д�볤��
												��ֵ	 -- ������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_write_dma(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg);




__vd_extern_c_leave__
#endif  // _DRV_UART_H
