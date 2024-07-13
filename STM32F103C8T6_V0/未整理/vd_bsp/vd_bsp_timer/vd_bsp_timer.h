/*
 * Project: mcu-kit
 * Module:�δ�ʱ������
 * File: vd_bsp_timer.h
 * Created Date: 2022��6��20��20:58:14
 * Author: gwx
 * Description: ���ڱ�ʾmcuϵͳʱ��
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

/* timer�����ṹ�� */
typedef struct {
    vd_bsp_port_e  	timer_id;		/* ��ʱ��id */
		vd_uint32_t			period;			/* ���ڣ�us */
		vd_bool_t				auto_load;	/* �Զ���װ�� */
} vd_bsp_timer_t;

/* ����timerƽ̨�ӿ� */
typedef struct{
	vd_bool_t (*init)(vd_bsp_timer_t *tim, void (*tim_irq_callback)(void *), void *arg);/* ��ʼ�� */
	vd_bool_t (*open)(vd_bsp_timer_t *tim);/* �� */
	vd_bool_t (*close)(vd_bsp_timer_t *tim);/* �� */
}timer_ops_t;

/* ƽ̨ע��ӿ� */
typedef vd_bool_t (*timer_plat_register_t)(timer_ops_t *ops);

/* ======================================================================================
 * declaration
 */

/**
 * @brief timer��ʼ��
 *
 * @param timer_id  				timer��ʼ����ӦID��
 * @param tim_irq_callback  timer�ص�����ָ��
 * @param arg  							�ص�����
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_timer_init(vd_bsp_port_e timer_id, void (*tim_irq_callback)(void *), void *arg);
/**
 * @brief timer��
 *
 * @param timer_id  		timer ID��	
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_timer_open(vd_bsp_port_e timer_id);
/**
 * @brief timer�ر�
 *
 * @param timer_id  		timer ID��	
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_timer_close(vd_bsp_port_e timer_id);

#endif // _DRV_TIMER_H
