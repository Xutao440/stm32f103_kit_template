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
		LED_STATE_OFF,     // ��
    LED_STATE_ON = 0,  // ��
};

typedef enum {
    DRV_LED_OFF,          // ����
    DRV_LED_ON,           // ����
    DRV_LED_FLASH,        // ����
    DRV_LED_BREATHE,      // ����������pwmģʽ��֧�֣�
    DRV_LED_ADJUST_TIME,  // LED��ָ������ʱ��
} vd_fml_per_led_mode_e;

typedef struct {
    vd_ulong_t  bright_time;  // һ����˸���ߺ���ʱ����ʱ��
    vd_ulong_t  dark_time;    // һ����˸���ߺ���ʱ���ʱ��
    vd_int16_t mode_count;   // ��˸������Ĵ���
} vd_fml_per_led_param_t;

typedef struct {
    vd_uint8_t      id;
    vd_uint8_t      pin;
    vd_uint8_t      active_high : 1;
    vd_uint8_t      led_mode;         //'vd_fml_per_led_mode_e'
    vd_uint8_t      curr_mode_count;  // ��ǰ��˸������Ĵ���
    vd_ulong_t      curr_time;        // ��ǰledʱ��
    vd_uint8_t      curr_state;      // ��ǰled����������
    vd_fml_per_led_param_t param;     //������˸����
    vd_bsp_port_e   pwm_port;
    vd_uint32_t      duty_cycle;       // ��ǰռ�ձ�(��pwmģʽ��֧��)
		vd_uint32_t     breath_speed;
} vd_fml_per_led_t;
/* ======================================================================================
 * declaration
 */

/**
 * @brief : led�Ƴ���
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_on(vd_uint8_t led_id);

/**
 * @brief : led�Ƴ���
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_off(vd_uint8_t led_id);

/**
 * @brief :LED���Զ�������ʱ��
 *
 * @param led_id
 * @param adjust_time
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_adjust(vd_uint8_t led_id, vd_ulong_t adjust_time);

/**
 * @brief :led�Ƴ���
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_flash(vd_uint8_t led_id);

/**
 * @brief :led��������
 *
 * @param led_id
 * @param flash_count
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_flash_count(vd_uint8_t led_id, vd_uint32_t flash_count);

/**
 * @brief : led�Ƴ�����
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_breathe(vd_uint8_t led_id, vd_uint32_t breath_speed);

/**
 * @brief led�ƺ�������
 *
 * @param led_id
 * @param flash_count
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_breathe_count(vd_uint8_t led_id, vd_uint32_t breathe_count);

/**
 * @brief :����led,���в������Զ�������
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