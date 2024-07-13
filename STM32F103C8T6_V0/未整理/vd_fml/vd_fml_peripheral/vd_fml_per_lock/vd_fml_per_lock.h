/*
 * Project: ble_freertos_demo_pr008_9_cJson
 * Module: nfc
 * File: drv_nfc.h
 * Created Date: 2022-07-23 11:21:41
 * Author: liaozewei
 * Description: nfc
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef VD_FML_PER_LOCK_H
#define VD_FML_PER_LOCK_H

/* ======================================================================================
 * includes
 */
#include "base.h"

/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
// typedef enum {
//     LOW = 0,  //低电平
//     HIGH      //高电平
// } high_low_e;
/* ======================================================================================
 * types
 */
typedef enum {
    lock_on_ok = 1,       //防盗打开成功
    lock_on_off,          //当前点火状态，防盗打开失败
    lock_on_on_operate,   //当前是防盗打开状态， 无需再操作
    lock_off_ok,          //防盗关闭成功
    lock_off_on_operate,  //当前是防盗关闭状态， 无需再操作
} LOCK_STATE;             /* 锁车状态 */

typedef struct {
    vd_uint8_t lock_type;

} lock_event_t;

/* ======================================================================================
 * declaration
 */

/**
 * @brief : 锁车开关
 * @param : bit_value
 * @return vd_bool_t
 */
// vd_bool_t drv_isdn_init(void);
vd_uint8_t drv_lock_switch_on(vd_void_t);

/**
 * @brief : 防盗处理
 * @param : bit_value
 * @return vd_bool_t
 */
// vd_bool_t drv_isdn_init(void);
void drv_lock_handle(void);
/**
 * @brief : 解锁开关
 * @return vd_bool_t
 */
vd_uint8_t drv_lock_switch_off(vd_void_t);

/**
 * @brief : lock  loop
 * @return vd_bool_t
 */

vd_void_t drv_lock_loop(vd_void_t);

/**
 * @brief : 控制电门锁
 * @return vd_uint8_t
 */

vd_uint8_t lock_ele_control(vd_void_t);

/**
 * @brief :得到电门锁状态
 * @return vd_uint8_t
 */
vd_uint8_t lock_ele_get_state(vd_uint8_t data);

/**
 * @brief : lock 初始化
 * @return vd_bool_t
 */

vd_bool_t drv_lock_init(vd_void_t);
/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__

#endif
