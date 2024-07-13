/*
 * Project: rf01
 * Module: module_name
 * File: vd_bsp_watch_dog.h
 * Created Date: 2022-06-01 09:42:30
 * Author: huangjunli
 * Description: description
 * -----
 * todo: modified 
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * includes
 */
#include "base.h"
#if  (KIT_PLATFORM == KIT_VDCHIP4005)
//#include "watch_dog.h"
#include "driver_wdt.h"
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

/**
 * @brief : watch dogʧ��
 *
 * @return vd_bool_t
 */
vd_bool_t vd_bsp_watch_dog_disable(vd_void_t);

/**
 * @brief : watch dog��ʼ��
 *
 * @param period:��λ����(ĿǰRF01��Ϊ1600)
 * @return vd_bool_t
 */
vd_bool_t vd_bsp_watch_dog_init(void);

/**
 * @brief : watch dog ι��
 * 
 * @return vd_bool_t
 */
vd_void_t vd_bsp_watch_dog_kick(vd_void_t);

__vd_extern_c_leave__
#endif // module_name_DRV_BATTERY_H