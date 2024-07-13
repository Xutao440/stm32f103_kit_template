/*
 * Project: mcu-kit
 * Module: Э����������
 * File: protocol_server.h
 * Created Date: 2022-03-24 15:03:48
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _PROTOCOL_SERVER_H
#define _PROTOCOL_SERVER_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "project_config.h"

#if (MACHINE_IF006 == ON)
#include "machine_if006/machine_if006.h"
#endif

/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
typedef enum {
    CMD_TYPE_NORMAL = 0,  // ��ָͨ��
    CMD_TYPE_HEART,       // ����ָ��
    CMD_TYPE_LOOP,        // ѭ��ָ��
    CMD_TYPE_COUNTDOWN,   // ����ʱָ��
    CMD_TYPE_CLEAR,       // ���ָ�������Ͷ��е�����ָ��
} cmd_type_t;

typedef vd_bool_t (*protocol_machine_init_t)(vd_void_t);
typedef vd_uint32_t (*protocol_machine_get_cmdcode_t)(vd_uint8_t *data);
typedef vd_bool_t (*protocol_machine_recv_handler_t)(vd_bsp_port_e port_id, vd_uint8_t *data, vd_uint16_t data_len);
typedef vd_void_t (*msg_dispatch_fun_t)(vd_bsp_port_e port_id, vd_uint8_t* data, vd_uint32_t data_len);
/* ======================================================================================
 * types
 */
typedef struct {
    vd_bsp_port_e                   port_id;
    protocol_machine_init_t         init;
    protocol_machine_get_cmdcode_t  get_cmdcode;
    protocol_machine_recv_handler_t recv_handler;
} protocol_machine_handler_t;

typedef struct {
    vd_uint64_t 		interval;                         // ָ����
    vd_uint8_t  		num;                              // ָ�����к�
    vd_uint8_t  		type : 3;                         // ָ������,��cmd_type_t��
    vd_uint8_t  		is_nr : 1;                        // ָ���Ƿ���Ҫ�ظ�
    vd_uint8_t  		is_active : 1;                    // ָ���Ƿ���Ч
    vd_bsp_port_e  	port_id;                          // ָ��͵�Ŀ�Ķ˿�
		vd_uint8_t  		format_id;                        // ָ��͵�Э���ʽ
    vd_uint8_t  		data[PROTOCOL_PAYLOAD_SIZE_MAX];  // �·������ݺ��ϱ���ָ��
    vd_uint32_t 		data_len;                         // ָ���
    vd_uint32_t 		nr_code;  // ָ��ظ��룬 �ͻظ���cmdcode�Ƚϣ������жϴ�ָ���Ƿ�ظ���
} protocol_cmd_t;

typedef struct {
    vd_uint8_t  		ev_type;  // 'EV_SYS_PROTOCOL'
    vd_bsp_port_e  	port_id;  // Э��ID,����Ӧport_id�˿ںţ�һ���˿ڶ�Ӧһ��Э��
    vd_uint8_t  	protocol_data[PROTOCOL_PAYLOAD_SIZE_MAX];
    vd_uint32_t 	data_len;  // ָ���
} protocol_event_t;        // ����Э���¼�

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */

//vd_uint32_t protocol_server_get_cmdcode(vd_bsp_port_e port_id, vd_uint8_t* data);

/**
 * @brief : ָ��ͺ���
 *
 * @param protocol_cmd	ָ����Ϣ
 * @return vd_bool_t
 */
vd_bool_t protocol_server_send_cmd(protocol_cmd_t* protocol_cmd);

/**
 * @brief : Э�������ʼ��,ע�������մ�����
 *
 * @return vd_bool_t
 */
vd_bool_t protocol_server_init(vd_void_t);

/**
 * @brief : Э�����ѭ��
 *
 * @return vd_void_t
 */
vd_void_t protocol_server_loop(vd_void_t);

//vd_void_t protocol_server_ble_rec(vd_uint8_t * _buf, vd_uint8_t len);
__vd_extern_c_leave__
#endif  // _PROTOCOL_SERVER_H