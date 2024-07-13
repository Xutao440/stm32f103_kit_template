/*
 * Project: mcu-kit
 * Module: mcu-kit 驱动层头文件汇总
 * File: kit_driver_fml.h
 * Created Date: 2022年6月21日19:45:48
 * Author: gwx
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _KIT_DRIVER_FML_H
#define _KIT_DRIVER_FML_H

/* ======================================================================================
 * includes
 */
#include "base.h"
//#include "mcu_kit_config.h"
#include "kit_fml_peripheral.h"
#include "project_config.h"
#if (DRIVER_FML_EN==ON)//模块开关
//#include "kit_driver_bsp.h"
#if (DRIVER_FML_TIME_EN==ON)
#include "time/hrd_time_module.h"
#endif

// 2, 驱动模块头文件
#if (DRIVER_FML_GPIO_KEY_EN == ON)
#include "key/hrd_key_module.h"
#endif

#if (DRIVER_FML_LED_EN == ON)
#include "led/hrd_led_module.h"
#endif

#if (DRIVER_FML_BATTERY_EN == ON)
#include "battery/hrd_battery_module.h"
#endif
#if (DRIVER_FML_ADC_KEY_EN==ON)
#include "key/hrd_adc_key_module.h"
#endif
#if (DRIVER_FML_LED_EN==ON)
#include "led/hrd_led_module.h"
#endif
#if (DRIVER_FML_LED_DISPLAY_EN==ON)
#include "display/hrd_led_display_module.h"
#endif
#if (DRIVER_FML_UART_DISPLAY_EN==ON)
#include "display/hrd_uart_display_module.h"
#endif

#if (DRIVER_FML_INFRARED_EN==ON)
#include "infrared/hrd_infrared_module.h"
#endif
#if (DRIVER_FML_MOTOR_TYPE_DC==ON)
#include "roller/hrd_roller_module.h"
#endif
#if (DRIVER_FML_MOTOR_TYPE_BLDC == ON)||(DRIVER_FML_MOTOR_TYPE_BSAC == ON)
#include "motor/hrd_motor_module.h"
#endif
#if (DRIVER_FML_BMS_UART_EN==ON)
#include "bms_uart/hrd_bms_uart_module.h"
#endif
#if (DRIVER_FML_BMS_SPI_EN==ON)
#include "bms_spi/hrd_bms_spi_module.h"
#endif
#if (DRIVER_FML_DATA_BACKUP_EN==ON)
#include "data_backup/hrd_data_backup_module.h"
#endif

#if (DRIVER_FML_FAN_EN==ON)
#include "fan/hrd_fan_module.h"
#endif
#if (DRIVER_FML_HEAT_EN==ON)
#include "heat/hrd_heat_module.h"
#endif
#if (DRIVER_FML_POT_DETECTION_EN==ON)
#include "pot_detection/hrd_pot_module.h"
#endif
#if (DRIVER_FML_BEEP_EN==ON)
#include "beep/hrd_beep_module.h"
#endif
#if (DRIVER_FML_ZERO_CROSSING_DETECTION_EN == ON)
#include "zero_crossing_detection/hrd_zero_crossing_detection_module.h"
#endif
#if (DRIVER_FML_RELAY_EN==ON)
#include "relay/hrd_relay_module.h"
#endif
#if (DRIVER_FML_TEMP_EN==ON)
#include "temp/hrd_temp_module.h"
#endif
#if (DRIVER_FML_UART_WEIGHING_EN==ON)
#include "weighing/hrd_weighing_uart_module.h"
#endif
#endif   //DRIVER_FML_EN
/* ======================================================================================
 * extern
 */

// 2, 驱动模块头文件
#if (PLATFORM_KEY_EN == ON)
#include "vd_fml_per_key/vd_fml_per_key.h"
#endif

#if (PLATFORM_LED_EN == ON)
#include "led/drv_led.h"
#endif

#if (PLATFORM_KNOB_EN == ON)
#include "vd_fml_per_knob/vd_fml_per_knob.h"
#endif

#if (PLATFORM_ACC_EN == ON)
#include "acc/drv_acc.h"
#endif

#if (PLATFORM_TP_EN == ON)
#include "tp/drv_tp.h"
#endif

#if (PLATFORM_SENSOR_EN == ON)
#include "sensor/drv_sensor.h"
#endif

#if (PLATFORM_LCD_EN == ON)
#include "vd_fml_per_lcd/vd_fml_per_lcd.h"
#endif

#if (PLATFORM_BATTERY_EN == ON)
#include "battery/drv_battery.h"
#endif

#if (PLATFORM_POWER_EN == ON)
#include "power/drv_power.h"
#endif

#if (PLATFORM_CHARGER_EN == ON)
#include "charge/drv_charge.h"
#endif
#if (PLATFORM_DEBUG_EN == ON)
#include "debug/drv_debug.h"
#endif
/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */
 #if (DRIVER_FML_EN==ON)//模块开关
//vd_void_t kit_hrd_init( vd_void_t );
//异常检测模块运行调度接口
vd_void_t hrd_drivers_check_module(vd_uint16_t check_20ms_once_num);
//模块功能测试初始化接口
vd_bool_t hrd_drivers_module_test_init(vd_void_t);
//模块功能测试调度接口
vd_bool_t hrd_drivers_module_test(vd_void_t);
#endif   //DRIVER_FML_EN
/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // _KIT_DRIVER_FML_H
