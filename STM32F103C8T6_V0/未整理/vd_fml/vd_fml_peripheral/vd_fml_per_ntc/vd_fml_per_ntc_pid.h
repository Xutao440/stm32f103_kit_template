/*
 * Project: WH001
 * Module: 
 * File: pid.h
 * Created Date: 2022-11-26 10:06:21
 * Author: your_name
 * Description: description
 * -----
 * todo: modified 
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _PID_H
#define _PID_H

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
typedef struct pid_calibration {
    vd_uint8_t kp; 
    vd_uint8_t ki;
    vd_uint8_t kd; 
} pid_calibration;
typedef struct pid_state {

    vd_uint8_t actual; //��ǰֵ
    vd_uint8_t target; //Ŀ��ֵ
    vd_uint8_t time_delta; //�ۻ�ʱ�� 
    vd_int16_t previous_error;//��һ�����
    vd_int16_t integral; //�ۻ����
    vd_int16_t output; //��� 
} pid_state;
pid_state vd_fml_per_ntc_temp_set(pid_calibration calibration, pid_state state);
/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */ 
__vd_extern_c_leave__
#endif // _PID_H