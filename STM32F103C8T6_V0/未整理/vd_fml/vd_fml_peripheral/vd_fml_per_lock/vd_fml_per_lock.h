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
//     LOW = 0,  //�͵�ƽ
//     HIGH      //�ߵ�ƽ
// } high_low_e;
/* ======================================================================================
 * types
 */
typedef enum {
    lock_on_ok = 1,       //�����򿪳ɹ�
    lock_on_off,          //��ǰ���״̬��������ʧ��
    lock_on_on_operate,   //��ǰ�Ƿ�����״̬�� �����ٲ���
    lock_off_ok,          //�����رճɹ�
    lock_off_on_operate,  //��ǰ�Ƿ����ر�״̬�� �����ٲ���
} LOCK_STATE;             /* ����״̬ */

typedef struct {
    vd_uint8_t lock_type;

} lock_event_t;

/* ======================================================================================
 * declaration
 */

/**
 * @brief : ��������
 * @param : bit_value
 * @return vd_bool_t
 */
// vd_bool_t drv_isdn_init(void);
vd_uint8_t drv_lock_switch_on(vd_void_t);

/**
 * @brief : ��������
 * @param : bit_value
 * @return vd_bool_t
 */
// vd_bool_t drv_isdn_init(void);
void drv_lock_handle(void);
/**
 * @brief : ��������
 * @return vd_bool_t
 */
vd_uint8_t drv_lock_switch_off(vd_void_t);

/**
 * @brief : lock  loop
 * @return vd_bool_t
 */

vd_void_t drv_lock_loop(vd_void_t);

/**
 * @brief : ���Ƶ�����
 * @return vd_uint8_t
 */

vd_uint8_t lock_ele_control(vd_void_t);

/**
 * @brief :�õ�������״̬
 * @return vd_uint8_t
 */
vd_uint8_t lock_ele_get_state(vd_uint8_t data);

/**
 * @brief : lock ��ʼ��
 * @return vd_bool_t
 */

vd_bool_t drv_lock_init(vd_void_t);
/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__

#endif
