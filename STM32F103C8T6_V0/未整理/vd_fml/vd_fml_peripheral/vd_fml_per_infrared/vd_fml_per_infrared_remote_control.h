/*
* Project: mcu-kit
 * Module: module_name
 * File: hrd_infrared_module.h
 * Created Date: 2022��7��13��10:33:06
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
	VD_FML_PER_INFRARED_POWER,//��Դ
	VD_FML_PER_INFRARED_GET_WATER,	//ȡˮ
	VD_FML_PER_INFRARED_HEAT,	//����
	VD_FML_PER_INFRARED_ATTEMPERATION,	//����/����
	VD_FML_PER_INFRARED_KEEP_WARM,	//����
	VD_FML_PER_INFRARED_GET_COLD_WATER,	//ȡ��ˮ
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
 * @brief : ����ģ���ʼ��
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_infrared_remote_control_init(vd_void_t);

/**
 * @brief : ����ģ��ң�������ݻ�ȡ
 *
 * @return vd_bool_t
 */
vd_void_t vd_fml_per_infrared_remote_control_data_get(vd_void_t);

/*
infrared module
*/
vd_void_t vd_fml_per_infrared_remote_control_loop(vd_void_t);
/*
infrared module���Դ���
*/
#if (DRIVER_FML_INFRARED_TEST_EN == ON)
vd_uint8_t vd_fml_per_infrared_remote_control_test(vd_void_t);
#endif

__vd_extern_c_enter__;
#endif
#endif // _HRD_INFRARED_MODULE_H
