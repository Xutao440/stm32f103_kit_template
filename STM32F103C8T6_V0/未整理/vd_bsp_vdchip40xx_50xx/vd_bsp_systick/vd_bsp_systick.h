/*
 * Project: mcu-kit
 * Module:�δ�ʱ������
 * File: drv_systick.h
 * Created Date: 2022-03-28 19:22:10
 * Author: meizhen
 * Description: ���ڱ�ʾmcuϵͳʱ��
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
 * @brief : ��ȡϵͳ�δ�ʱ��
 *
 * @return vd_uin32_t
 */
vd_ulong_t drv_systick_get(vd_void_t);

vd_void_t drv_systick_loop(vd_void_t);

/**
 * @brief ϵͳ�δ�ʱ����ʼ��
 *
 * @return vd_bool_t
 */
vd_bool_t drv_systick_init(vd_void_t);

__vd_extern_c_leave__

#endif

#endif  // _DRV_SYSTICK_H