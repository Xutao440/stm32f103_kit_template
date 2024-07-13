/*
 * Project: mcu-kit
 * Module:knob driver
 * File: drv_knob.h
 * Created Date: 2022-03-31 17:19:07
 * Author: meizhen
 * Description: 旋钮驱动
 * -----
 * @Descripttion:
 * @version: 1.0版本
 * @Author: daibaoye
 * @Date: 2022-05-05 09:27:36
 * @LastEditors: daibaoye
 * @LastEditTime: 2022-07-09 10:56:01
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_KNOB_H
#define _DRV_KNOB_H

/* ======================================================================================
 * includes
 */
#include "base.h"
//#include "kit_lib.h"
#include "platform_config.h"

//#if (PLATFORM_KNOB_EN == ON)
/* ======================================================================================
 * extern
 */
//__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
typedef enum {
	ROTATE_NONE,   // 不旋
    ROTATE_LEFT,   // 左旋
    ROTATE_RIGHT,  // 右旋
    
} knob_flag_e;
/* ======================================================================================
 * types
 */
typedef struct {
    vd_uint8_t a_pin;
    vd_uint8_t b_pin;
    vd_uint8_t cycle;  // 0;旋转一次半个周期，1，旋转一次一个周期
} drv_knob_t;

typedef struct {
    vd_uint8_t  ev_type;
    vd_uint8_t  rotate;  // 0: 左旋，1：右旋 knob_flag_e
    vd_uint32_t type_num;
} knob_event_t;

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */
//vd_bool_t drv_knob_init(vd_void_t);

//vd_void_t drv_knob_loop(vd_void_t);

//vd_uint8_t knobpad_read(vd_void_t);

//vd_void_t knob_report(knob_event_t* knob_event);

vd_bool_t vd_fml_per_knob_init(vd_void_t);

vd_void_t vd_fml_per_knob_task_handler(vd_void_t);

//#endif
//__vd_extern_c_leave__
#endif  // _DRV_KNOB_H