/*
 * Project: mcu-kit
 * Module: mcu-kit 驱动层头文件汇总
 * File: kit_driver_bsp_config.h
 * Created Date: 2022-03-23 21:13:09
 * Author: 
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _KIT_DRIVER_BSP_CONFIG_H
#define _KIT_DRIVER_BSP_CONFIG_H
/*kit_driver->kit_driver_bsp_config*/
/* ======================================================================================
 * includes
 */
#include "project_config.h"


// 1, 驱动接口库头文件
#include "vd_bsp_gpio/vd_bsp_gpio.h"

#include "vd_bsp_uart/vd_bsp_uart.h"

#include "vd_bsp_spi/vd_bsp_spi.h"

//#include "i2c/drv_i2c.h"

#include "vd_bsp_pwm/vd_bsp_pwm.h"

//#include "adc/drv_adc.h"

#include "vd_bsp_timer/vd_bsp_timer.h"

#include "vd_bsp_flash/vd_bsp_flash.h"

#include "vd_bsp_watch_dog/vd_bsp_watch_dog.h"


//#if (PLATFORM_DEBUG_EN == ON)
//#include "debug/drv_debug.h"
//#endif

//#if (PLATFORM_SYSTICK_EN == ON)
//#include "systick/drv_systick.h"
//#endif

// 2, 驱动模块头文件
//#if (PLATFORM_KEY_EN == ON)
//#include "vd_fml_per_key/vd_fml_per_key.h"
//#endif

#if (PROJECT_LED_EN == ON)
#include "vd_fml_per_led/vd_fml_per_led.h"
#endif

#if (PROJECT_KNOB_EN == ON)
#include "vd_fml_per_knob/vd_fml_per_knob.h"
#endif

//#if (PROJECT_ACC_EN == ON)
//#include "acc/drv_acc.h"
//#endif

//#if (PROJECT_TP_EN == ON)
//#include "tp/drv_tp.h"
//#endif

//#if (PROJECT_SENSOR_EN == ON)
//#include "sensor/drv_sensor.h"
//#endif

#if (PROJECT_LCD_EN == ON)
#include "vd_fml_per_lcd/vd_fml_per_lcd.h"
#endif


//#if (PROJECT_BATTERY_EN == ON)
//#include "battery/drv_battery.h"
//#endif

//#if (PROJECT_POWER_EN == ON)
//#include "power/drv_power.h"
//#endif

//#if (PROJECT_CHARGER_EN == ON)
//#include "charge/drv_charge.h"
//#endif


//功能模块头文件
//#if (DRIVER_FML_HEAT_EN == ON)
//#include "vd_control_heat_module/vd_control_heat_module.h"
//#endif
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
#endif  // _KIT_DRIVER_BSP_CONFIG_H
