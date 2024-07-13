/*
 * Project: DT1866
 * Module: Protocol Driver (uart,can, gpio)
 * File: protocol_driver.h
 * Created Date: 2021-07-22 20:48:55
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef PROTOCOL_DRIVER_H
#define PROTOCOL_DRIVER_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "vd_bsp_type.h"
#include "iservice_def.h"

/* ======================================================================================
 * extern
 */
// __vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */

#define PROTOCOL_SIMULATE_DRIVER_READ  0  // ģ����ӿ����������ڵ���Э��
#define PROTOCOL_SIMULATE_DRIVER_WRITE 0  // ģ��д�ӿ����������ڵ���Э��
/* ======================================================================================
 * types
 */
 
typedef vd_void_t (*driver_rx_it_callback_t)(vd_bsp_port_e port, vd_uint8_t *pbuf, vd_uint16_t len, vd_void_t *arg);//�жϽ��ջص�
 
typedef vd_bool_t (*driver_init_t)(vd_bsp_port_e port_id, read_method_e method);

typedef vd_uint16_t (*driver_read_t)(vd_bsp_port_e port_id, uint8_t* data, vd_uint16_t data_len);

typedef vd_bool_t (*driver_write_t)(vd_bsp_port_e port_id, uint8_t* data, vd_uint16_t data_len);

/* Э�������˿ڶ��� */
typedef struct {
	driver_init_t  								d_init;   		// Ӳ��ͨ����ʼ��
	driver_read_t  								d_read;   		// Ӳ��ͨ��������
	driver_write_t 								d_write;  		// Ӳ��ͨ��д����
} protocol_driver_t;
/* ======================================================================================
 * declaration
 */
/**
 * @brief ��ȡЭ������
 *
 * @param 	port_id  	�����˿ں�
 * @return 	vd_bool_t
 */
vd_bool_t protocol_driver_get(vd_bsp_port_e port_id, protocol_driver_t* p_driver);

/**
 * @brief �жϽ��ջص�ע��
 *
 * @param 	cb  	�ص�����
 * @param 	arg  	�ص�����
 * @return 	vd_bool_t
 */
vd_bool_t protocol_driver_rx_it_callback_register(driver_rx_it_callback_t cb, vd_void_t *arg);

/* ======================================================================================
 * extern
 */
// __vd_extern_c_leave__

#endif  // PROTOCOL_DRIVER_H
