/*
 * Project: mcu-kit
 * Module:iservice2.0单片机版
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
 * @brief : 					注册接收回调处理函数
 *
 * @param hander			回调函数
 * @return vd_bool_t 	注册结果
 */
vd_bool_t iservice_receive_callback_register(iservice_receive_callback_t hander);

/**
 * @brief : iservice主循环，解析上行数据，下发下行数据
 *
 * @return vd_void_t
 */
vd_void_t iservice_loop(vd_void_t);

/**
 * @brief : iservcie初始化
 *
 * @return vd_bool_t
 */
vd_bool_t iservice_init(vd_void_t);

/**
 * @brief :格式化写，把payload格式化一帧完整的数据，再使用写函数写入端口
 *
 * @param port_id: 端口
 * @param format_id: 协议格式
 * @param pbuf: 待格式化数据
 * @param len: 待格式化数据长度
 * @return vd_bool_t: 格式化以及写入结果
 */
vd_bool_t iservice_write(vd_bsp_port_e port_id, protocol_format_e format_id, vd_uint8_t *pbuf, vd_uint16_t len);


vd_void_t iservice_push_queue(vd_bsp_port_e port_id, vd_uint8_t* pbuf, vd_uint16_t len, vd_void_t *arg); 


__vd_extern_c_leave__

#endif  // _ISERVICE_H
