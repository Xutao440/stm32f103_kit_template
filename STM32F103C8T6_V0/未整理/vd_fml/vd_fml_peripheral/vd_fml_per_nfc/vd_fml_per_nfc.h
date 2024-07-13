/*
 * Project: fr8000
 * Module: nfc
 * File: drv_nfc.h
 * Created Date: 2022-10-18 21:16:14
 * Author: liaozewei
 * Description: description
 * -----
 * todo: modified 
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef DRV_NFC_H
#define DRV_NFC_H

/* ======================================================================================
 * includes
 */
#include "base.h"

/* ======================================================================================
 * extern
 */
// 外部定义变量或函数位置




typedef struct {
    vd_uint8_t nfc_info_state;
    vd_uint8_t nfc_id_len0;       // nfc卡号长度
    vd_uint8_t nfc_id_buff0[10];  // nfc卡号内容
    vd_uint8_t nfc_id_len1;       // nfc卡号长度
    vd_uint8_t nfc_id_buff1[10];  // nfc卡号内容

    vd_uint8_t nfc_id_len2;       // nfc卡号长度
    vd_uint8_t nfc_id_buff2[10];  // nfc卡号内容
    vd_uint8_t nfc_id_len3;       // nfc卡号长度
    vd_uint8_t nfc_id_buff3[10];  // nfc卡号内容
    vd_uint8_t nfc_id_len4;       // nfc卡号长度
    vd_uint8_t nfc_id_buff4[10];  // nfc卡号内容

} nfc_config_t;  // NFC结构体

typedef struct {
    vd_uint8_t user_chose_info[20];

} user_chose_info_t;  // 客制化选项

typedef struct {
    //    isdn_config_t isdn;
    nfc_config_t      nfc;
    user_chose_info_t user_chose_info_;
} config_table_t;  //配置表相关结构体










__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
// 宏定义位置

/* ======================================================================================
 * types
 */
// 类型定义位置


typedef struct {
    vd_uint8_t nfc_type;   
	  vd_uint8_t nfc_id_len;  // 
	  vd_uint8_t nfc_id[10];  //
} nfc_event_t;


typedef struct {
    vd_uint8_t nfc_state;   //
	  vd_uint8_t nfc_id_len;  //
		vd_uint8_t nfc_factory_flag;  
	  vd_uint8_t nfc_id_buff[10];  //
	  vd_uint8_t nfc_factory_buff[16];  //
  
} nfc_data_t;

/* ======================================================================================
 * declaration
 */
// 函数定义位置


/**
 * @brief : nfc初始化
 * @param : 
 * @return vd_bool_t
 */
//vd_bool_t drv_isdn_init(void);
vd_bool_t drv_nfc_init(void) ;


/**
 * @brief : nfc id获取函数
 * @return vd_bool_t
 */
//vd_bool_t drv_isdn_interaction(vd_uint8_t bit_value);
vd_void_t  drv_nfc_loop(void);



/**
 * @brief : nfc 业务处理
 * @return vd_bool_t
 */
vd_void_t drv_nfc_deal(vd_void_t);




/* ======================================================================================
 * extern
 */ 
__vd_extern_c_leave__
#endif // module_name_DRV_NFC_H









