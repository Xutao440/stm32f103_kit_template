/*
 * Project: mcu-kit
 * Module:key driver
 * File: vd_fml_per_key.h
 * Created Date: 2022-03-28 17:19:48
 * Author: meizhen
 * Description: description
 * -----
 * @Descripttion:
 * @version: 1.0版本
 * @Author: daibaoye
 * @Date: 2022-05-05 09:27:36
 * @LastEditors: daibaoye
 * @LastEditTime: 2022-05-23 19:43:31
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_KEY_H
#define _DRV_KEY_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "key_code.h"
#include "vd_display_lib.h"
#include "platform_config.h"
#include "project_config.h"
/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
#define DRV_KEY_POLL_TIME 10  // ms
/* ======================================================================================
 * types
 */

typedef enum {
    KEY_PRESS,      // 按下
    KEY_RELEASE,    // 释放
		KEY_CLICK,				//单击
    KEY_LONG_3s_PRESS,  //长按事件
		KEY_LONG_5s_PRESS,  //长按事件
		KEY_LONG_10s_PRESS,  //长按事件
} key_flag_e;

enum {
    ACTIVE_LOW,  // 低有效，低电平为按下
    ACTIVE_HIGH  // 高有效，高电平为按下
};

enum {
    KEY_REPORT_NONE,  // 按键无须上报
    KEY_REPORT_LIMIT  // 按键上报
};

typedef struct {
    vd_uint8_t pin;
    vd_uint8_t value;            // 'key_code_e'
    vd_uint8_t active_high : 1;  //'key_flag_e'
    vd_uint8_t reserve : 7;
} key_info_t;

// key上报的event事件信息
typedef struct {
    vd_uint8_t ev_type;  
    vd_uint8_t key_code;  // 'key_code_e'  key_code.h中的编码值
    vd_uint8_t key_flag;  // 'key_flag_e'  按下 释放 长按 长按释放
    vd_ulong_t time;      //当前时间
} key_event_t;
/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */

/**
 * @brief : 按键驱动轮询
 *
 * @return vd_void_t
 */
vd_void_t vd_fml_per_key_loop(vd_void_t);

/**
 * @brief : 按键初始化
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_key_init(vd_void_t);


__vd_extern_c_leave__
#endif  // _DRV_KEY_H