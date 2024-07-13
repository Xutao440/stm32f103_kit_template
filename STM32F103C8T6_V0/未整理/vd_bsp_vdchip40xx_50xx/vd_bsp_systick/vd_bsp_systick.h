/*
 * Project: mcu-kit
 * Module:滴答定时器驱动
 * File: drv_systick.h
 * Created Date: 2022-03-28 19:22:10
 * Author: meizhen
 * Description: 用于表示mcu系统时间
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_SYSTICK_H
#define _DRV_SYSTICK_H

/* ======================================================================================
 * includes
 */
#include "base.h"
//#include "kit_lib.h"
#include "queue/cmd_queue.h"
#include "queue/mq_queue.h"
#include "vd_log.h"

#include "platform_config.h"

#if (PLATFORM_SYSTICK_EN == ON)
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

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */

/**
 * @brief : 获取系统滴答定时器
 *
 * @return vd_uin32_t
 */
vd_ulong_t drv_systick_get(vd_void_t);

vd_void_t drv_systick_loop(vd_void_t);

/**
 * @brief 系统滴答定时器初始化
 *
 * @return vd_bool_t
 */
vd_bool_t drv_systick_init(vd_void_t);

__vd_extern_c_leave__

#endif

#endif  // _DRV_SYSTICK_H