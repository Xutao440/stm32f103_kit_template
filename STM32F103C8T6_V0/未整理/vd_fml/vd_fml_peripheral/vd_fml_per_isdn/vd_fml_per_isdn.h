/*
 * Project: ble_freertos_demo_pr008_9_cJson
 * Module: isdn
 * File: drv_isdn.h
 * Created Date: 2022-07-23 11:21:41
 * Author: baoye
 * Description: һ��ͨͨѶ
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef VD_FML_PER_ISDN_H
#define VD_FML_PER_ISDN_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "vd_bsp_gpio/vd_bsp_gpio.h"
//#include "kit_lib.h"
#include "cmd_queue.h"
#include "mq_queue.h"
#include "vd_log.h"

#include "project_config.h"
#include "protocol_driver.h"
/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
typedef enum {
    LOW = 0,  //�͵�ƽ
    HIGH      //�ߵ�ƽ
} high_low_e;

typedef enum {
    PLATFORM_ISDN0 = 4,  //
    PLATFORM_ISDN1,
    PLATFORM_ISDN2,
    PLATFORM_ISDNMAX
} plat_isdn_id_e;
/* ======================================================================================
 * types
 */
typedef struct {
    plat_isdn_id_e  isdn_id;   //��·һ��ͨ
    vd_uint8_t      pin_num;   //�����źŵ�pin��
    vd_uint8_t pin_mode;  //����pin��ģʽ
    cmd_queue_t     rx_queue;  //���͵Ķ���
} drv_isdn_t;

typedef struct {
    vd_uint8_t  dev_code;   //�豸����
    vd_uint8_t  seq_code;   //��ˮ��
    vd_uint8_t  err1_code;  //�����ź�1
    vd_uint8_t  err2_code;  //�����ź�2
    vd_uint8_t  run_code;   //���������ź�
    vd_uint8_t  pro_code;   //���������ź�
    vd_uint8_t  bat_code;   //���е���
    vd_uint16_t spd_code;   //�����ٶ�
    vd_uint8_t  cell_code;  //��ص���
    vd_uint8_t  run_bat;    //���е�������
    vd_uint8_t  check_sum;  //У���
} proctocl_isdn_t;
/* ======================================================================================
 * declaration
 */
/**
 * @brief : һ��ͨ��ʼ���ڲ�����
 * @return vd_bool_t
 */
vd_bool_t isdn_inside_init(void);
/**
 * @brief : һ��ͨ��ʼ���ӿں���
 * @param : isdn_id
 * @return vd_bool_t
 */
vd_bool_t drv_isdn_init(vd_uint8_t isdn_id);
/**
 * @brief : �ֽ���Ϻ���
 * @param : bit_value
 * @return vd_bool_t
 */
vd_bool_t drv_isdn_interaction(vd_uint8_t bit_value);


/**
 * @brief : һ��ͨ��ʼ������
 *
 * @param void 
 * @return 
 */

vd_bool_t drv_isdn_init_t(vd_void_t);

/**
 * @brief : һ��ͨ����������
 *
 * @param void 
 * @return 
 */

vd_void_t drv_isdn_loop(vd_void_t);



/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // isdn_DRV_ISDN_H