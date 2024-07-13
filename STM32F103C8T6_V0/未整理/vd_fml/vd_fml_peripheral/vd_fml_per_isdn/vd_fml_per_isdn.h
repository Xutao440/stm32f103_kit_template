/*
 * Project: ble_freertos_demo_pr008_9_cJson
 * Module: isdn
 * File: drv_isdn.h
 * Created Date: 2022-07-23 11:21:41
 * Author: baoye
 * Description: 一线通通讯
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
    LOW = 0,  //低电平
    HIGH      //高点平
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
    plat_isdn_id_e  isdn_id;   //几路一线通
    vd_uint8_t      pin_num;   //输入信号的pin脚
    vd_uint8_t pin_mode;  //配置pin脚模式
    cmd_queue_t     rx_queue;  //发送的队列
} drv_isdn_t;

typedef struct {
    vd_uint8_t  dev_code;   //设备编码
    vd_uint8_t  seq_code;   //流水号
    vd_uint8_t  err1_code;  //故障信号1
    vd_uint8_t  err2_code;  //故障信号2
    vd_uint8_t  run_code;   //车辆运行信号
    vd_uint8_t  pro_code;   //车辆保护信号
    vd_uint8_t  bat_code;   //运行电流
    vd_uint16_t spd_code;   //车辆速度
    vd_uint8_t  cell_code;  //电池电量
    vd_uint8_t  run_bat;    //运行电流比例
    vd_uint8_t  check_sum;  //校验和
} proctocl_isdn_t;
/* ======================================================================================
 * declaration
 */
/**
 * @brief : 一线通初始化内部函数
 * @return vd_bool_t
 */
vd_bool_t isdn_inside_init(void);
/**
 * @brief : 一线通初始化接口函数
 * @param : isdn_id
 * @return vd_bool_t
 */
vd_bool_t drv_isdn_init(vd_uint8_t isdn_id);
/**
 * @brief : 字节组合函数
 * @param : bit_value
 * @return vd_bool_t
 */
vd_bool_t drv_isdn_interaction(vd_uint8_t bit_value);


/**
 * @brief : 一线通初始化函数
 *
 * @param void 
 * @return 
 */

vd_bool_t drv_isdn_init_t(vd_void_t);

/**
 * @brief : 一线通解析处理函数
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