/*
 * Project: MCU_kit
 * Module: vd_bsp_adc.c
 * File: vd_bsp_adc.c
 * Created Date: 2022-03-24 15:19:11
 * Author: GanWenXu
 * Description: ADC驱动
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

/* ADC 驱动对象 */
typedef struct{
	adc_plat_register_t 	plat_register;		/* ADC 平台注册接口 */
	adc_ops_t 						ops;				/* ADC 平台接口 */
}adc_drv_t;

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
 

/* 定义 ADC 驱动对象 */
static adc_drv_t adc_drv = {
	.plat_register = PLATFORM_ADC_REGISTER,
	.ops = {0},
};

/* ADC 配置列表 */
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
 * @brief 根据ID索引查找驱动
 *
 * @param adc_id 		ADC对应的ID号
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
 * @brief 初始化ADC
 *
 * @param 	adc_id  	ADC 初始化对应ID号
 * @return 	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_init(vd_bsp_port_e adc_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;

	/* 平台注册 */
	vd_check_return_val_error(adc_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "platform adc not bound\r\n");
	ret = adc_drv.plat_register(&adc_drv.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "platform adc register fail\r\n");

	/* 参数校验 */
	vd_check_return_val_error(adc_id < VD_PORT_ADC0 || adc_id >= VD_PORT_ADC_MAX, -VD_BSP_ERR_PORT_INVALID, "adc_id = %d, error\r\n", adc_id);
	
	/* 遍历配置列表 */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "adc[%d] not found\r\n", adc_id - VD_PORT_ADC0);
	
	log_debug("adc module = %d\n", adc->module);
	log_debug("adc channel = %d\n", adc->channel);
	log_debug("adc pin = %d\n", adc->pin);
	log_debug("adc ref_type = %d\n", adc->ref_type);
	log_debug("adc ref_volt = %d\n", adc->ref_volt);
	log_debug("adc resolution = %d\n", adc->resolution);
	log_debug("adc use dma = %d\n", adc->use_dma);
	
	/* 参数校验 */
	vd_check_return_val_error(adc->ref_type >= PLATFORM_ADC_REF_MAX, -VD_BSP_ERR_PARAMETER, "in func:%s, line:%d\r\n", __func__, __LINE__);
	
	/* 初始化 */
	vd_check_return_val_error(adc_drv.ops.init == vd_null,  -VD_BSP_ERR_SDK, "adc init ops is null\r\n");
	ret = adc_drv.ops.init(adc);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform adc[%d] init error\r\n", adc_id - VD_PORT_ADC0);

  return VD_BSP_NOERROR;
}

/**
 * @brief  打开ADC
 *
 * @param 	adc_id 		ADC索引
 * @return 	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_open(vd_bsp_port_e adc_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;
	
	/* 参数校验 */
	vd_check_return_val_error(adc_id < VD_PORT_ADC0 || adc_id >= VD_PORT_ADC_MAX, -VD_BSP_ERR_PORT_INVALID, "adc_id = %d, error\r\n", adc_id);
	
	/* 遍历配置列表 */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No adc[%d]\r\n", adc_id - VD_PORT_ADC0);

	/* 打开 */
	vd_check_return_val_error(adc_drv.ops.open == vd_null,  -VD_BSP_ERR_SDK, "adc open ops is null\r\n");
	ret = adc_drv.ops.open(adc);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform adc[%d] open error\r\n", adc_id - VD_PORT_ADC0);

	return VD_BSP_NOERROR;
}

/**
 * @brief  关闭ADC
 *
 * @param 	adc_id 		ADC索引
 * @return 	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_close(vd_bsp_port_e adc_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;
	
	/* 参数校验 */
	vd_check_return_val_error(adc_id < VD_PORT_ADC0 || adc_id >= VD_PORT_ADC_MAX, -VD_BSP_ERR_PORT_INVALID, "adc_id = %d, error\r\n", adc_id);
	
	/* 遍历配置列表 */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No adc[%d]\r\n", adc_id - VD_PORT_ADC0);

	/* 打开 */
	vd_check_return_val_error(adc_drv.ops.close == vd_null,  -VD_BSP_ERR_SDK, "adc close ops is null\r\n");
	ret = adc_drv.ops.close(adc);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform adc[%d] close error\r\n", adc_id - VD_PORT_ADC0);

	return VD_BSP_NOERROR;
}

/**
 * @brief  读取ADC数据
 *
 * @param 	adc_id 		ADC索引
 * @return 	非负值：	读取结果
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_read(vd_bsp_port_e adc_id) 
{
	vd_int32_t   ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;
	
	/* 参数校验 */
	vd_check_return_val_error(adc_id < VD_PORT_ADC0 || adc_id >= VD_PORT_ADC_MAX, vd_false, "adc_id = %d, error\r\n", adc_id);
	
	/* 遍历配置列表 */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No adc[%d]\r\n", adc_id - VD_PORT_ADC0);

	/* 读取 */
	vd_check_return_val_error(adc_drv.ops.read == vd_null,  -VD_BSP_ERR_SDK, "adc read ops is null\r\n");
	ret = adc_drv.ops.read(adc);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform adc[%d] read error\r\n", adc_id - VD_PORT_ADC0);

	return ret;
}

/**
 * @brief  读取ADC数据
 *
 * @param 	adc_id 		ADC索引
 * @return 	非负值：	读取结果
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_read_mv(vd_bsp_port_e adc_id)
{
	vd_int32_t   	ret = vd_false;
	vd_bsp_adc_t* adc = vd_null;
	
	/* 遍历配置列表 */
	adc = vd_bsp_adc_find(adc_id);
	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No adc[%d]\r\n", adc_id - VD_PORT_ADC0);
	
	/* 读取 */
	vd_check_return_val_error(adc_drv.ops.read == vd_null,  -VD_BSP_ERR_SDK, "adc read ops is null\r\n");
	ret = adc_drv.ops.read(adc);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform adc[%d] read error, func = %s, line = %d\r\n", adc_id - VD_PORT_ADC0, __func__, __LINE__);
	
	return (ret * adc->ref_volt) >> adc->resolution;
}

