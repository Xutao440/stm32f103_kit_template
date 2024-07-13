/*
 * Project: mcu-kit
 * Module: mcu-kit 驱动层头文件汇总
 * File: vd_driver.h
 * Created Date: 2022-03-23 21:13:09
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _VD_DRIVER_H
#define _VD_DRIVER_H

/* ======================================================================================
 * includes
 */
#include "project_config.h"

// 1, 驱动接口库头文件
#if (PLATFORM_GPIO_EN == ON)
#include "vd_bsp_gpio/vd_bsp_gpio.h"
#endif

#if (PLATFORM_UART0_EN == ON || PLATFORM_UART1_EN == ON || PLATFORM_UART2_EN == ON)
#include "vd_bsp_uart/vd_bsp_uart.h"
#endif

#if (PLATFORM_SPIM0_EN == ON || PLATFORM_SPIM1_EN == ON || PLATFORM_SPIS_EN == ON)
#include "vd_bsp_spi/vd_bsp_spi.h"
#endif

#if (PLATFORM_I2C0_EN == ON || PLATFORM_I2C1_EN == ON | PLATFORM_I2C2_EN == ON)
#include "vd_bsp_i2c/vd_bsp_i2c.h"
#endif

#if (PLATFORM_PWM0_EN == ON || PLATFORM_PWM1_EN == ON)
#include "vd_bsp_pwm/vd_bsp_pwm.h"
#endif

#if (PLATFORM_ADC0_EN == ON || PLATFROM_ADC1_EN == ON)
#include "vd_bsp_adc/vd_bsp_adc.h"
#endif

#if (PLATFORM_TIME0_EN == ON || PLATFORM_TIME1_EN == ON)
#include "vd_bsp_timer/vd_bsp_timer.h"
#endif

#if (PLATFORM_SYSTICK_EN == ON)
#include "vd_bsp_systick/vd_bsp_systick.h"
#endif

#if (PLATFORM_FLASH_EN == ON)
#include "vd_bsp_flash/vd_bsp_flash.h"
#endif

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
__vd_extern_c_leave__
#endif  // _VD_DRIVER_H
