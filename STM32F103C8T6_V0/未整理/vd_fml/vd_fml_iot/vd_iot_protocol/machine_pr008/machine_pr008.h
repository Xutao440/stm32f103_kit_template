/*
 * Project: linux-kit_dev
 * Module: 具体某个机器协议处理
 * File: machine_pr008.h
 * Created Date: 2022-05-17 17:04:21
 * Author: meizhen
 * Description: 此文件模板通过工具生成
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _MACHINE_PR008_H
#define _MACHINE_PR008_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "project_config.h"
/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros

/////////////////////////////////////////////////////////////////////////////////////////
/**指令结构体和宏统一定义模板：
 *
 * 1，xxx_cmd_t: 表示发送的指令结构体
 * 2，xxx_reply_t: 表示接收的指令结构体
 * 3，CMD_xxx: 表示发送的指令码
 * 4，REPLY_xxx: 表示接收的指令码
/////////////////////////////////////////////////////////////////////////////////////////
 *
 */

#define DEVICE_TYPE 0x06

#define CMD_VERSION   0x01
#define REPLY_VERSION 0x01

#define CMD_QUERYSTATUS   0xA5
#define REPLY_QUERYSTATUS 0xC0

/* ======================================================================================
 * types enum
 */

/* ======================================================================================
 * types struct
 */

#pragma pack(1)
typedef struct {
    vd_uint8_t cmd;   // 0x01
    vd_uint8_t veri;  // 0x06
} version_cmd_t;

typedef struct {
    vd_uint8_t  cmd;   // 0x01
    vd_uint8_t  veri;  // 0x06
    vd_uint16_t companycode;
    vd_uint16_t hardcode;
    vd_uint16_t softcode;
    vd_uint8_t  protocode;
    vd_uint8_t  maccode;
} version_reply_t;

typedef struct {
    vd_uint8_t  cmd;   // 0xa5
    vd_uint8_t  veri;  // 0x06
    vd_uint8_t  serialnum;
    vd_uint8_t  device_work;
    vd_uint8_t  device_mode;
    vd_uint16_t device_temp;
    vd_uint8_t  device_time[3];
    vd_uint8_t  device_buzzer_type : 4;
    vd_uint8_t  device_buzzer_count : 4;
    vd_uint8_t  device_buzzer_times : 4;
    vd_uint8_t  device_buzzer_num : 4;
} querystatus_cmd_t;

typedef struct {
    vd_uint8_t  cmd;   // 0xc0
    vd_uint8_t  veri;  // 0x06
    vd_uint16_t mcu_temp;
    vd_uint8_t  mcu_work_state;
    vd_uint8_t  mcu_err_code;
} querystatus_reply_t;

#pragma pack()

/* ======================================================================================
 * declaration
 */

vd_uint32_t machine_pr008_get_cmdcode(vd_uint8_t* data);

version_reply_t* machine_pr008_get_version(vd_void_t);

querystatus_reply_t* machine_pr008_get_querystatus(vd_void_t);

vd_bool_t machine_pr008_version_cmd_send(version_cmd_t* version_cmd);

vd_bool_t machine_pr008_querystatus_cmd_send(querystatus_cmd_t* querystatus_cmd);

vd_bool_t machine_pr008_receive_handle(vd_uint8_t port_id, vd_uint8_t* data, vd_uint16_t data_len);

vd_bool_t machine_pr008_init(vd_void_t);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // machine_pr008_H
