/*
 * Project: WH001
 * Module: 
 * File: vd_fml_per_ntc_table.h
 * Created Date: 2022-11-25 16:57:46
 * Author: yangdingyuan
 * Description: description
 * -----
 * todo: modified 
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _NTC_TABLE_H
#define _NTC_TABLE_H

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
#define NUM 251
/* ======================================================================================
 * types
 */
extern const unsigned int vd_fml_per_ntc_table[NUM];
/* ======================================================================================
 * declaration
 */
vd_int_t read_vd_fml_per_ntc_temperature(unsigned int r_adc);
/* ======================================================================================
 * extern
 */ 
__vd_extern_c_leave__
#endif // _NTC_TABLE_H