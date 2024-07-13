/*
 * Project: mcu-kit
 * Module: 滴答定时器驱动
 * File: vd_bsp_timer.c
 * Created Date: 2022年6月20日 10:58:57
 * Author: gwx
 * Description: 用于表示mcu系统时间
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)

/* ======================================================================================
 * includes
 */
#include "vd_bsp_timer/vd_bsp_timer.h"
#include "platform_config.h"
#include "vd_log.h"

/* ======================================================================================
 * macros
 */
#ifndef PLATFORM_TIMER_REGISTER
#define PLATFORM_TIMER_REGISTER vd_null
#endif

/* ======================================================================================
 * types
 */
/* timer驱动对象 */
typedef struct{
	timer_plat_register_t 	plat_register;		/* timer平台注册接口 */
	timer_ops_t 						ops;							/* timer平台接口 */
}timer_drv_t;

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
/* 定义timer驱动对象 */
static timer_drv_t timer_drv = {
	.plat_register = PLATFORM_TIMER_REGISTER,
	.ops = {0},
};

static vd_bsp_timer_t g_vd_bsp_timer[] = {
#if (PLATFORM_TIME0_EN == ON)
	{VD_PORT_TIM0, PLATFORM_TIME0_PERIOD, PLATFORM_TIME0_AUTOLOAD},
#endif
#if (PLATFORM_TIME1_EN == ON)
	{VD_PORT_TIM1, PLATFORM_TIME1_PERIOD, PLATFORM_TIME1_AUTOLOAD},
#endif
	{0},
};
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */


/* ======================================================================================
 * implementation
 */
 
 /**
 * @brief 根据ID索引查找驱动
 *
 * @param timer_id timer对应的ID号
 * @return vd_bsp_timer_t*
 */
static vd_bsp_timer_t* vd_bsp_timer_find(vd_bsp_port_e timer_id) {
    for (int i = 0; i < vd_array_size(g_vd_bsp_timer) - 1; i++) {
        if (g_vd_bsp_timer[i].timer_id == timer_id) {
            return &g_vd_bsp_timer[i];
        }
    }
    return vd_null;
}
 
/**
 * @brief timer初始化
 *
 * @param timer_id  				timer初始化对应ID号
 * @param tim_irq_callback  timer回调函数指针
 * @param arg  							回调参数
 * @return vd_bool_t				初始化结果
 */
vd_int32_t vd_bsp_timer_init(vd_bsp_port_e timer_id, void (*tim_irq_callback)(void *), void *arg)
{
	vd_bool_t   ret = vd_false;
	vd_bsp_timer_t* tim = vd_null;

	/* 平台注册 */
	vd_check_return_val_error(timer_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "timer platform not bound\r\n");
	ret = timer_drv.plat_register(&timer_drv.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "timer platform register fail\r\n");

	/* 参数校验 */
	vd_check_return_val_error(timer_id < VD_PORT_TIM0 || timer_id >= VD_PORT_TIM_MAX, -VD_BSP_ERR_PORT_INVALID, "timer_id = %d, error\r\n", timer_id);
	
	/* 遍历timer配置列表 */
	tim = vd_bsp_timer_find(timer_id);
	vd_check_return_val_error(tim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "timer[%d] not found\r\n", timer_id - VD_PORT_TIM0);
	vd_check_return_val_error(tim->period == 0, -VD_BSP_ERR_PARAMETER, "timer[%d] period is 0, func = %d, line = %d\r\n", timer_id - VD_PORT_TIM0, __func__, __LINE__);
	
	/* timer初始化 */
	vd_check_return_val_error(timer_drv.ops.init == vd_null, -VD_BSP_ERR_SDK, "timer init port is null\r\n");
	ret = timer_drv.ops.init(tim, tim_irq_callback, arg);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform timer[%d] init error\r\n", timer_id - VD_PORT_TIM0);

  return VD_BSP_NOERROR;
}

/**
 * @brief timer打开
 *
 * @param timer_id  		timer ID号	
 * @return vd_bool_t		打开结果
 */
vd_int32_t vd_bsp_timer_open(vd_bsp_port_e timer_id)
{
	vd_bool_t   ret = vd_false;
	vd_bsp_timer_t* tim = vd_null;

	/* 参数校验 */
	vd_check_return_val_error(timer_id < VD_PORT_TIM0 || timer_id >= VD_PORT_TIM_MAX, -VD_BSP_ERR_PORT_INVALID, "timer_id = %d, error\r\n", timer_id);
	
	/* 遍历timer配置列表 */
	tim = vd_bsp_timer_find(timer_id);
	vd_check_return_val_error(tim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "timer[%d] not found\r\n", timer_id - VD_PORT_TIM0);
	
	/* timer打开 */
	vd_check_return_val_error(timer_drv.ops.open == vd_null, -VD_BSP_ERR_SDK, "timer open port is null\r\n");
	ret = timer_drv.ops.open(tim);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform timer[%d] open error\r\n", timer_id - VD_PORT_TIM0);

  return VD_BSP_NOERROR;
}

/**
 * @brief timer关闭
 *
 * @param timer_id  		timer ID号	
 * @return vd_bool_t		关闭结果
 */
vd_int32_t vd_bsp_timer_close(vd_bsp_port_e timer_id)
{
	vd_bool_t   ret = vd_false;
	vd_bsp_timer_t* tim = vd_null;

	/* 参数校验 */
	vd_check_return_val_error(timer_id < VD_PORT_TIM0 || timer_id >= VD_PORT_TIM_MAX, -VD_BSP_ERR_PORT_INVALID, "timer_id = %d, error\r\n", timer_id);
	
	/* 遍历timer配置列表 */
	tim = vd_bsp_timer_find(timer_id);
	vd_check_return_val_error(tim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "timer[%d] not found\r\n", timer_id - VD_PORT_TIM0);
	
	/* timer关闭 */
	vd_check_return_val_error(timer_drv.ops.close == vd_null, -VD_BSP_ERR_SDK, "timer close port is null\r\n");
	ret = timer_drv.ops.close(tim);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform timer[%d] close error\r\n", timer_id - VD_PORT_TIM0);

  return VD_BSP_NOERROR;
}





