/*
 * Project: MCU_kit
 * Module: vd_bsp_adc.c
 * File: vd_bsp_adc.c
 * Created Date: 2022-03-24 15:19:11
 * Author: GanWenXu
 * Description: ADC����
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */

/* ======================================================================================
 * includes
 */
#include "vd_bsp_adc/vd_bsp_adc.h"
#include "platform_config.h"
#include "vd_log.h"

/* ======================================================================================
 * macros
 */
#ifndef PLATFORM_ADC_REGISTER
#define PLATFORM_ADC_REGISTER vd_null
#endif

/* ======================================================================================
 * types
 */

/* ADC �������� */
typedef struct{
	adc_plat_register_t 	plat_register;		/* ADC ƽ̨ע��ӿ� */
	adc_ops_t 						ops;				/* ADC ƽ̨�ӿ� */
}adc_drv_t;

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
 

/* ���� ADC �������� */
static adc_drv_t adc_drv = {
	.plat_register = PLATFORM_ADC_REGISTER,
	.ops = {0},
};

/* ADC �����б� */
static vd_bsp_adc_t g_vd_bsp_adc[] = {
	#if (PLATFORM_ADC0_EN == ON)
		{VD_PORT_ADC0, PLATFORM_ADC0_MODULE, PLATFORM_ADC0_PIN, PLATFORM_ADC0_CHANNEL, PLATFORM_ADC0_REF, PLATFORM_ADC0_REF_VOLT, PLATFORM_ADC0_RES_BIT, PLATFORM_ADC0_DMA_EN},
	#endif
	#if (PLATFORM_ADC1_EN == ON)
		{VD_PORT_ADC1, PLATFORM_ADC1_MODULE, PLATFORM_ADC1_PIN, PLATFORM_ADC1_CHANNEL, PLATFORM_ADC1_REF, PLATFORM_ADC1_REF_VOLT, PLATFORM_ADC1_RES_BIT, PLATFORM_ADC1_DMA_EN},
	#endif
	{0},
};
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */





/**
 * @brief ����ID������������
 *
 * @param adc_id 		ADC��Ӧ��ID��
 * @return vd_bsp_adc_t*
 */
static vd_bsp_adc_t* vd_bsp_adc_find(vd_bsp_port_e adc_id) {
    for (int i = 0; i < vd_array_size(g_vd_bsp_adc) - 1; i++) {
        if (g_vd_bsp_adc[i].id == adc_id) {
            return &g_vd_bsp_adc[i];
        }
    }
    return vd_null;
}

/* ======================================================================================
 * implementation
 */

/**
 * @brief ��ʼ��ADC
 *
 * @param 	adc_id  	ADC ��ʼ����ӦID��
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_init(vd_bsp_port_e adc_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;

	/* ƽ̨ע�� */
	vd_check_return_val_error(adc_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "platform adc not bound\r\n");
	ret = adc_drv.plat_register(&adc_drv.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "platform adc register fail\r\n");

	/* ����У�� */
	vd_check_return_val_error(adc_id < VD_PORT_ADC0 || adc_id >= VD_PORT_ADC_MAX, -VD_BSP_ERR_PORT_INVALID, "adc_id = %d, error\r\n", adc_id);
	
	/* ���������б� */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "adc[%d] not found\r\n", adc_id - VD_PORT_ADC0);
	
	log_debug("adc module = %d\n", adc->module);
	log_debug("adc channel = %d\n", adc->channel);
	log_debug("adc pin = %d\n", adc->pin);
	log_debug("adc ref_type = %d\n", adc->ref_type);
	log_debug("adc ref_volt = %d\n", adc->ref_volt);
	log_debug("adc resolution = %d\n", adc->resolution);
	log_debug("adc use dma = %d\n", adc->use_dma);
	
	/* ����У�� */
	vd_check_return_val_error(adc->ref_type >= PLATFORM_ADC_REF_MAX, -VD_BSP_ERR_PARAMETER, "in func:%s, line:%d\r\n", __func__, __LINE__);
	
	/* ��ʼ�� */
	vd_check_return_val_error(adc_drv.ops.init == vd_null,  -VD_BSP_ERR_SDK, "adc init ops is null\r\n");
	ret = adc_drv.ops.init(adc);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform adc[%d] init error\r\n", adc_id - VD_PORT_ADC0);

  return VD_BSP_NOERROR;
}

/**
 * @brief  ��ADC
 *
 * @param 	adc_id 		ADC����
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_open(vd_bsp_port_e adc_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(adc_id < VD_PORT_ADC0 || adc_id >= VD_PORT_ADC_MAX, -VD_BSP_ERR_PORT_INVALID, "adc_id = %d, error\r\n", adc_id);
	
	/* ���������б� */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No adc[%d]\r\n", adc_id - VD_PORT_ADC0);

	/* �� */
	vd_check_return_val_error(adc_drv.ops.open == vd_null,  -VD_BSP_ERR_SDK, "adc open ops is null\r\n");
	ret = adc_drv.ops.open(adc);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform adc[%d] open error\r\n", adc_id - VD_PORT_ADC0);

	return VD_BSP_NOERROR;
}

/**
 * @brief  �ر�ADC
 *
 * @param 	adc_id 		ADC����
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_close(vd_bsp_port_e adc_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(adc_id < VD_PORT_ADC0 || adc_id >= VD_PORT_ADC_MAX, -VD_BSP_ERR_PORT_INVALID, "adc_id = %d, error\r\n", adc_id);
	
	/* ���������б� */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No adc[%d]\r\n", adc_id - VD_PORT_ADC0);

	/* �� */
	vd_check_return_val_error(adc_drv.ops.close == vd_null,  -VD_BSP_ERR_SDK, "adc close ops is null\r\n");
	ret = adc_drv.ops.close(adc);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform adc[%d] close error\r\n", adc_id - VD_PORT_ADC0);

	return VD_BSP_NOERROR;
}

/**
 * @brief  ��ȡADC����
 *
 * @param 	adc_id 		ADC����
 * @return 	�Ǹ�ֵ��	��ȡ���
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_read(vd_bsp_port_e adc_id) 
{
	vd_int32_t   ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(adc_id < VD_PORT_ADC0 || adc_id >= VD_PORT_ADC_MAX, vd_false, "adc_id = %d, error\r\n", adc_id);
	
	/* ���������б� */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No adc[%d]\r\n", adc_id - VD_PORT_ADC0);

	/* ��ȡ */
	vd_check_return_val_error(adc_drv.ops.read == vd_null,  -VD_BSP_ERR_SDK, "adc read ops is null\r\n");
	ret = adc_drv.ops.read(adc);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform adc[%d] read error\r\n", adc_id - VD_PORT_ADC0);

	return ret;
}

/**
 * @brief  ��ȡADC����
 *
 * @param 	adc_id 		ADC����
 * @return 	�Ǹ�ֵ��	��ȡ���
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_read_mv(vd_bsp_port_e adc_id)
{
	vd_int32_t   	ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;
	
	/* ���������б� */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No adc[%d]\r\n", adc_id - VD_PORT_ADC0);
	
	/* ��ȡ */
	vd_check_return_val_error(adc_drv.ops.read == vd_null,  -VD_BSP_ERR_SDK, "adc read ops is null\r\n");
	ret = adc_drv.ops.read(adc);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform adc[%d] read error, func = %s, line = %d\r\n", adc_id - VD_PORT_ADC0, __func__, __LINE__);
	
	return (ret * adc->ref_volt) >> adc->resolution;
}

