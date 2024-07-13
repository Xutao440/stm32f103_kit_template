/*
* Project: mcu-kit
 * Module: module_name
 * File: hrd_infrared_module.h
 * Created Date: 2022年7月13日10:33:06
 * Author: gwx
 * Description: description
 * -----
 * todo: modified 
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _VD_FML_PER_INFRARED_REMOTE_CONTROL_H
#define _VD_FML_PER_INFRARED_REMOTE_CONTROL_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "kit_driver_bsp_config.h"
//#include "system_config.h"
//#include "vd_fml_config.h"
//#include "system_config.h"

#if 1 //(DRIVER_FML_INFRARED_EN == ON)
//#include "kit_driver_bsp"


/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
#define DRIVER_FML_INFRARED_TEST_EN OFF
/* ======================================================================================
 * types
 */

typedef enum
{	
	VD_FML_PER_INFRARED_POWER,//电源
	VD_FML_PER_INFRARED_GET_WATER,	//取水
	VD_FML_PER_INFRARED_HEAT,	//加热
	VD_FML_PER_INFRARED_ATTEMPERATION,	//调温/制冷
	VD_FML_PER_INFRARED_KEEP_WARM,	//保温
	VD_FML_PER_INFRARED_GET_COLD_WATER,	//取冷水
}vd_fml_per_infrared_key_type_t;

/* ======================================================================================
 * declaration
 */
extern vd_fml_per_infrared_key_type_t vd_fml_per_infrared_key;
extern vd_uint8_t vd_fml_per_infrared_remote_control_refresh_flag;
/* ======================================================================================
 * extern
 */ 
vd_fml_per_infrared_key_type_t *fml_infrared_key_info_get(vd_void_t);
/**
 * @brief : 红外模块初始化
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_infrared_remote_control_init(vd_void_t);

/**
 * @brief : 红外模块遥控器数据获取
 *
 * @return vd_bool_t
 */
vd_void_t vd_fml_per_infrared_remote_control_data_get(vd_void_t);

/*
infrared module
*/
vd_void_t vd_fml_per_infrared_remote_control_loop(vd_void_t);
/*
infrared module测试代码
*/
#if (DRIVER_FML_INFRARED_TEST_EN == ON)
vd_uint8_t vd_fml_per_infrared_remote_control_test(vd_void_t);
#endif

__vd_extern_c_enter__;
#endif
#endif // _HRD_INFRARED_MODULE_H
