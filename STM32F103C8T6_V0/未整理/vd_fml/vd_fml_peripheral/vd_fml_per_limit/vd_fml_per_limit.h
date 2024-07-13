/*
 * Project: ble_freertos_demo_pr008_9_cJson
 * Module: nfc
 * File: drv_nfc.h
 * Created Date: 2023-03-02 11:21:41
 * Author: liaozewei
 * Description: nfc
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2023 - vDiscovery, Inc
 */
#ifndef DRV_LIMIT_H
#define DRV_LIMIT_H

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

/* ======================================================================================
 * types
 */
enum {
    FACTORY_XI_TE = 1, 
		FACTORY_FENG_HUANG 
};

/* ======================================================================================
 * declaration
 */

/**
 * @brief ���ٵ��ýӿ�
* @param  limit_mode��0x01������״̬  0x02����״̬    send_cnt�����Ͳ��δ�������ǰĬ��10��
 * @return vd_void_t
 */
/*****************************���ٽӿ�*************************************/
vd_void_t drv_limit_mode(vd_uint8_t customer ,vd_uint8_t limit_mode,vd_uint8_t limit_valus,vd_uint8_t send_cnt);



/**
 * @brief ��������
* @param  
 * @return vd_void_t
 */
/************************��������********************************/
vd_void_t mcu_limit_drive(void);


/**
 * @brief ���ٵ��ýӿ�
* @param  limit_mode��0x01������״̬  0x02����״̬  limit_valus:���͵���ֵ  send_cnt�����Ͳ��δ�������ǰĬ��10��
 * @return vd_void_t
 */
/*****************************���ٽӿ�*************************************/
vd_uint8_t drv_get_limit_state(vd_void_t);
/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__

#endif