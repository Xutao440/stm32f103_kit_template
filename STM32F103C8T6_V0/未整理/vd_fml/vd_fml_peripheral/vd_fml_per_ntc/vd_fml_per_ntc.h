/*
 * Project: kit_sdk
 * Module: 
 * File: NTC.h
 * Created Date: 2022-11-05 14:56:15
 * Author: YangDingYuan
 * Description: description
 * -----
 * todo: modified 
 * -----
 * Copyright (c) 2022, Inc
 */
#ifndef _NTC_H
#define _NTC_H

/* ======================================================================================
 * includes
 */
#include "driver_gpio.h"
#include "driver_adc.h"
#include "base.h"
#include "vd_fml_per_ntc_table.h"
#include "vd_fml_per_ntc_pid.h"
//#include "wh001_drivers.h"
/* ======================================================================================
 * extern
 */


/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */
bool vd_fml_per_ntc_init(void);
int vd_fml_per_ntc_temp_get(void);
void vd_fml_per_ntc_loop(void);
vd_uint16_t adc_value_get(void);
vd_uint16_t adc_average_get(vd_uint8_t times);
void vd_fml_per_ntc_target_set(vd_uint8_t target,vd_uint8_t kp,vd_uint8_t ki,vd_uint8_t kd);
void pid_adjusting_enable(vd_bool_t enable,vd_uint8_t target);
vd_uint8_t temp_error_check();
/* ======================================================================================
 * extern
 */ 
#endif // _NTC_H