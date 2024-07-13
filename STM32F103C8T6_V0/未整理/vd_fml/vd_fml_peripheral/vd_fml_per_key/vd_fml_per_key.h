/*
 * Project: mcu-kit
 * Module:key driver
 * File: vd_fml_per_key.h
 * Created Date: 2022-03-28 17:19:48
 * Author: meizhen
 * Description: description
 * -----
 * @Descripttion:
 * @version: 1.0�汾
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
    KEY_PRESS,      // ����
    KEY_RELEASE,    // �ͷ�
		KEY_CLICK,				//����
    KEY_LONG_3s_PRESS,  //�����¼�
		KEY_LONG_5s_PRESS,  //�����¼�
		KEY_LONG_10s_PRESS,  //�����¼�
} key_flag_e;

enum {
    ACTIVE_LOW,  // ����Ч���͵�ƽΪ����
    ACTIVE_HIGH  // ����Ч���ߵ�ƽΪ����
};

enum {
    KEY_REPORT_NONE,  // ���������ϱ�
    KEY_REPORT_LIMIT  // �����ϱ�
};

typedef struct {
    vd_uint8_t pin;
    vd_uint8_t value;            // 'key_code_e'
    vd_uint8_t active_high : 1;  //'key_flag_e'
    vd_uint8_t reserve : 7;
} key_info_t;

// key�ϱ���event�¼���Ϣ
typedef struct {
    vd_uint8_t ev_type;  
    vd_uint8_t key_code;  // 'key_code_e'  key_code.h�еı���ֵ
    vd_uint8_t key_flag;  // 'key_flag_e'  ���� �ͷ� ���� �����ͷ�
    vd_ulong_t time;      //��ǰʱ��
} key_event_t;
/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */

/**
 * @brief : ����������ѯ
 *
 * @return vd_void_t
 */
vd_void_t vd_fml_per_key_loop(vd_void_t);

/**
 * @brief : ������ʼ��
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_key_init(vd_void_t);


__vd_extern_c_leave__
#endif  // _DRV_KEY_H