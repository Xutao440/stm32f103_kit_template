/*
 * Project: mcu-kit
 * Module:iservice2.0��Ƭ����
 * File: iservice.h
 * Created Date: 2022-03-24 11:50:10
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _ISERVICE_H
#define _ISERVICE_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "vd_bsp_type.h"
#include "iservice_def.h"

/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

typedef vd_void_t (*iservice_receive_callback_t)(vd_bsp_port_e port_id, vd_uint8_t* data, vd_uint16_t data_len);



/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */
 
/**
 * @brief : 					ע����ջص�������
 *
 * @param hander			�ص�����
 * @return vd_bool_t 	ע����
 */
vd_bool_t iservice_receive_callback_register(iservice_receive_callback_t hander);

/**
 * @brief : iservice��ѭ���������������ݣ��·���������
 *
 * @return vd_void_t
 */
vd_void_t iservice_loop(vd_void_t);

/**
 * @brief : iservcie��ʼ��
 *
 * @return vd_bool_t
 */
vd_bool_t iservice_init(vd_void_t);

/**
 * @brief :��ʽ��д����payload��ʽ��һ֡���������ݣ���ʹ��д����д��˿�
 *
 * @param port_id: �˿�
 * @param format_id: Э���ʽ
 * @param pbuf: ����ʽ������
 * @param len: ����ʽ�����ݳ���
 * @return vd_bool_t: ��ʽ���Լ�д����
 */
vd_bool_t iservice_write(vd_bsp_port_e port_id, protocol_format_e format_id, vd_uint8_t *pbuf, vd_uint16_t len);


vd_void_t iservice_push_queue(vd_bsp_port_e port_id, vd_uint8_t* pbuf, vd_uint16_t len, vd_void_t *arg); 


__vd_extern_c_leave__

#endif  // _ISERVICE_H
