/*
 * Project: mcu-kit
 * Module:led driver
 * File: vd_fml_per_led.h
 * Created Date: 2022-04-08 10:25:59
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_LED_H
#define _DRV_LED_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "vd_display_lib.h"
#include "vd_bsp_type.h"
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
typedef enum {
    LED_RED,
    LED_GREEN,
    LED_BLUE,
} vd_fml_per_led_e;


enum {
		LED_STATE_OFF,     // 灭
    LED_STATE_ON = 0,  // 亮
};

typedef enum {
    DRV_LED_OFF,          // 常灭
    DRV_LED_ON,           // 常亮
    DRV_LED_FLASH,        // 常闪
    DRV_LED_BREATHE,      // 常呼吸（仅pwm模式下支持）
    DRV_LED_ADJUST_TIME,  // LED灯指定亮的时间
} vd_fml_per_led_mode_e;

typedef struct {
    vd_ulong_t  bright_time;  // 一次闪烁或者呼吸时亮的时间
    vd_ulong_t  dark_time;    // 一次闪烁或者呼吸时灭的时间
    vd_int16_t mode_count;   // 闪烁或呼吸的次数
} vd_fml_per_led_param_t;

typedef struct {
    vd_uint8_t      id;
    vd_uint8_t      pin;
    vd_uint8_t      active_high : 1;
    vd_uint8_t      led_mode;         //'vd_fml_per_led_mode_e'
    vd_uint8_t      curr_mode_count;  // 当前闪烁或呼吸的次数
    vd_ulong_t      curr_time;        // 当前led时间
    vd_uint8_t      curr_state;      // 当前led是亮还是灭
    vd_fml_per_led_param_t param;     //呼吸闪烁控制
    vd_bsp_port_e   pwm_port;
    vd_uint32_t      duty_cycle;       // 当前占空比(仅pwm模式下支持)
		vd_uint32_t     breath_speed;
} vd_fml_per_led_t;
/* ======================================================================================
 * declaration
 */

/**
 * @brief : led灯常亮
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_on(vd_uint8_t led_id);

/**
 * @brief : led灯常灭
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_off(vd_uint8_t led_id);

/**
 * @brief :LED灯自定义亮起时间
 *
 * @param led_id
 * @param adjust_time
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_adjust(vd_uint8_t led_id, vd_ulong_t adjust_time);

/**
 * @brief :led灯常闪
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_flash(vd_uint8_t led_id);

/**
 * @brief :led灯闪几次
 *
 * @param led_id
 * @param flash_count
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_flash_count(vd_uint8_t led_id, vd_uint32_t flash_count);

/**
 * @brief : led灯常呼吸
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_breathe(vd_uint8_t led_id, vd_uint32_t breath_speed);

/**
 * @brief led灯呼吸几次
 *
 * @param led_id
 * @param flash_count
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_breathe_count(vd_uint8_t led_id, vd_uint32_t breathe_count);

/**
 * @brief :设置led,所有参数可自定义设置
 *
 * @param led_id
 * @param mode
 * @param param
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_set(vd_uint8_t led_id, vd_uint8_t mode, vd_fml_per_led_param_t* param);

/* ======================================================================================
 * extern
 */

vd_bool_t vd_fml_per_led_init(vd_void_t);

vd_void_t vd_fml_per_led_loop(vd_void_t);

__vd_extern_c_leave__
#endif  // _DRV_LED_H