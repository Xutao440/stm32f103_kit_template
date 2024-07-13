/*
 * Project: mcu-kit
 * Module: 协议解析处理层
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
    CMD_TYPE_NORMAL = 0,  // 普通指令
    CMD_TYPE_HEART,       // 心跳指令
    CMD_TYPE_LOOP,        // 循环指令
    CMD_TYPE_COUNTDOWN,   // 倒计时指令
    CMD_TYPE_CLEAR,       // 清除指令，清除发送队列的所有指令
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
    vd_uint64_t 		interval;                         // 指令间隔
    vd_uint8_t  		num;                              // 指令序列号
    vd_uint8_t  		type : 3;                         // 指令类型,‘cmd_type_t’
    vd_uint8_t  		is_nr : 1;                        // 指令是否需要回复
    vd_uint8_t  		is_active : 1;                    // 指令是否有效
    vd_bsp_port_e  	port_id;                          // 指令发送的目的端口
		vd_uint8_t  		format_id;                        // 指令发送的协议格式
    vd_uint8_t  		data[PROTOCOL_PAYLOAD_SIZE_MAX];  // 下发的数据和上报的指令
    vd_uint32_t 		data_len;                         // 指令长度
    vd_uint32_t 		nr_code;  // 指令回复码， 和回复的cmdcode比较，用于判断此指令是否回复了
} protocol_cmd_t;

typedef struct {
    vd_uint8_t  		ev_type;  // 'EV_SYS_PROTOCOL'
    vd_bsp_port_e  	port_id;  // 协议ID,即对应port_id端口号，一个端口对应一套协议
    vd_uint8_t  	protocol_data[PROTOCOL_PAYLOAD_SIZE_MAX];
    vd_uint32_t 	data_len;  // 指令长度
} protocol_event_t;        // 上行协议事件

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */

//vd_uint32_t protocol_server_get_cmdcode(vd_bsp_port_e port_id, vd_uint8_t* data);

/**
 * @brief : 指令发送函数
 *
 * @param protocol_cmd	指令信息
 * @return vd_bool_t
 */
vd_bool_t protocol_server_send_cmd(protocol_cmd_t* protocol_cmd);

/**
 * @brief : 协议服务层初始化,注册服务接收处理函数
 *
 * @return vd_bool_t
 */
vd_bool_t protocol_server_init(vd_void_t);

/**
 * @brief : 协议服务循环
 *
 * @return vd_void_t
 */
vd_void_t protocol_server_loop(vd_void_t);

//vd_void_t protocol_server_ble_rec(vd_uint8_t * _buf, vd_uint8_t len);
__vd_extern_c_leave__
#endif  // _PROTOCOL_SERVER_H