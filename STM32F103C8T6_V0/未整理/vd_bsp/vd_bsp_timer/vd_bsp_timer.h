/*
 * Project: mcu-kit
 * Module:滴答定时器驱动
 * File: vd_bsp_timer.h
 * Created Date: 2022年6月20日20:58:14
 * Author: gwx
 * Description: 用于表示mcu系统时间
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_TIMER_H
#define _DRV_TIMER_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "vd_bsp_type.h"

/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;
/* ======================================================================================
 * types
 */

/* timer参数结构体 */
typedef struct {
    vd_bsp_port_e  	timer_id;		/* 定时器id */
		vd_uint32_t			period;			/* 周期，us */
		vd_bool_t				auto_load;	/* 自动重装载 */
} vd_bsp_timer_t;

/* 定义timer平台接口 */
typedef struct{
	vd_bool_t (*init)(vd_bsp_timer_t *tim, void (*tim_irq_callback)(void *), void *arg);/* 初始化 */
	vd_bool_t (*open)(vd_bsp_timer_t *tim);/* 开 */
	vd_bool_t (*close)(vd_bsp_timer_t *tim);/* 关 */
}timer_ops_t;

/* 平台注册接口 */
typedef vd_bool_t (*timer_plat_register_t)(timer_ops_t *ops);

/* ======================================================================================
 * declaration
 */

/**
 * @brief timer初始化
 *
 * @param timer_id  				timer初始化对应ID号
 * @param tim_irq_callback  timer回调函数指针
 * @param arg  							回调参数
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_timer_init(vd_bsp_port_e timer_id, void (*tim_irq_callback)(void *), void *arg);
/**
 * @brief timer打开
 *
 * @param timer_id  		timer ID号	
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_timer_open(vd_bsp_port_e timer_id);
/**
 * @brief timer关闭
 *
 * @param timer_id  		timer ID号	
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_timer_close(vd_bsp_port_e timer_id);

#endif // _DRV_TIMER_H
