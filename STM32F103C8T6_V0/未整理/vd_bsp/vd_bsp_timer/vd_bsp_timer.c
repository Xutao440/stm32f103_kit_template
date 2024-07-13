/*
 * Project: mcu-kit
 * Module: �δ�ʱ������
 * File: vd_bsp_timer.c
 * Created Date: 2022��6��20�� 10:58:57
 * Author: gwx
 * Description: ���ڱ�ʾmcuϵͳʱ��
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
/* timer�������� */
typedef struct{
	timer_plat_register_t 	plat_register;		/* timerƽ̨ע��ӿ� */
	timer_ops_t 						ops;							/* timerƽ̨�ӿ� */
}timer_drv_t;

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
/* ����timer�������� */
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
 * @brief ����ID������������
 *
 * @param timer_id timer��Ӧ��ID��
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
 * @brief timer��ʼ��
 *
 * @param timer_id  				timer��ʼ����ӦID��
 * @param tim_irq_callback  timer�ص�����ָ��
 * @param arg  							�ص�����
 * @return vd_bool_t				��ʼ�����
 */
vd_int32_t vd_bsp_timer_init(vd_bsp_port_e timer_id, void (*tim_irq_callback)(void *), void *arg)
{
	vd_bool_t   ret = vd_false;
	vd_bsp_timer_t* tim = vd_null;

	/* ƽ̨ע�� */
	vd_check_return_val_error(timer_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "timer platform not bound\r\n");
	ret = timer_drv.plat_register(&timer_drv.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "timer platform register fail\r\n");

	/* ����У�� */
	vd_check_return_val_error(timer_id < VD_PORT_TIM0 || timer_id >= VD_PORT_TIM_MAX, -VD_BSP_ERR_PORT_INVALID, "timer_id = %d, error\r\n", timer_id);
	
	/* ����timer�����б� */
	tim = vd_bsp_timer_find(timer_id);
	vd_check_return_val_error(tim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "timer[%d] not found\r\n", timer_id - VD_PORT_TIM0);
	vd_check_return_val_error(tim->period == 0, -VD_BSP_ERR_PARAMETER, "timer[%d] period is 0, func = %d, line = %d\r\n", timer_id - VD_PORT_TIM0, __func__, __LINE__);
	
	/* timer��ʼ�� */
	vd_check_return_val_error(timer_drv.ops.init == vd_null, -VD_BSP_ERR_SDK, "timer init port is null\r\n");
	ret = timer_drv.ops.init(tim, tim_irq_callback, arg);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform timer[%d] init error\r\n", timer_id - VD_PORT_TIM0);

  return VD_BSP_NOERROR;
}

/**
 * @brief timer��
 *
 * @param timer_id  		timer ID��	
 * @return vd_bool_t		�򿪽��
 */
vd_int32_t vd_bsp_timer_open(vd_bsp_port_e timer_id)
{
	vd_bool_t   ret = vd_false;
	vd_bsp_timer_t* tim = vd_null;

	/* ����У�� */
	vd_check_return_val_error(timer_id < VD_PORT_TIM0 || timer_id >= VD_PORT_TIM_MAX, -VD_BSP_ERR_PORT_INVALID, "timer_id = %d, error\r\n", timer_id);
	
	/* ����timer�����б� */
	tim = vd_bsp_timer_find(timer_id);
	vd_check_return_val_error(tim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "timer[%d] not found\r\n", timer_id - VD_PORT_TIM0);
	
	/* timer�� */
	vd_check_return_val_error(timer_drv.ops.open == vd_null, -VD_BSP_ERR_SDK, "timer open port is null\r\n");
	ret = timer_drv.ops.open(tim);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform timer[%d] open error\r\n", timer_id - VD_PORT_TIM0);

  return VD_BSP_NOERROR;
}

/**
 * @brief timer�ر�
 *
 * @param timer_id  		timer ID��	
 * @return vd_bool_t		�رս��
 */
vd_int32_t vd_bsp_timer_close(vd_bsp_port_e timer_id)
{
	vd_bool_t   ret = vd_false;
	vd_bsp_timer_t* tim = vd_null;

	/* ����У�� */
	vd_check_return_val_error(timer_id < VD_PORT_TIM0 || timer_id >= VD_PORT_TIM_MAX, -VD_BSP_ERR_PORT_INVALID, "timer_id = %d, error\r\n", timer_id);
	
	/* ����timer�����б� */
	tim = vd_bsp_timer_find(timer_id);
	vd_check_return_val_error(tim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "timer[%d] not found\r\n", timer_id - VD_PORT_TIM0);
	
	/* timer�ر� */
	vd_check_return_val_error(timer_drv.ops.close == vd_null, -VD_BSP_ERR_SDK, "timer close port is null\r\n");
	ret = timer_drv.ops.close(tim);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform timer[%d] close error\r\n", timer_id - VD_PORT_TIM0);

  return VD_BSP_NOERROR;
}





