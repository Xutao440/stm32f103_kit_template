/*
 * Project: rf01
 * Module: module_name
 * File: vd_bsp_pwm.h
 * Created Date: 2022-05-05 10:57:10
 * Author: lijianzhong
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef __VDCHIP40XX_50XX_PWM_H
#define __VDCHIP40XX_50XX_PWM_H


/* ======================================================================================
 * includes
 */
#include "vd_bsp_pwm/vd_bsp_pwm.h"

/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

vd_bool_t vd_fr8000_pwm_register(vd_pwm_ops_t *ops);

__vd_extern_c_leave__
#endif  // module_name_DRV_PWM_H