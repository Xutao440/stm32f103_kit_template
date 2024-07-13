/*
 * Project: MCU_kit
 * Module: DRV_ADC
 * File: vd_bsp_adc.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_adc头文件
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_ADC_H
#define _DRV_ADC_H

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
 * macros
 */
#define ADC_CHANNEL_NUM	8		//ADC通道数量

/* ======================================================================================
 * types
 */
/* adc参考信号选择 */
typedef enum{
    PLATFORM_ADC_REF_IN,		/* 内部参考信号 */
    PLATFORM_ADC_REF_OUT,		/* 外部参考信号 */
		PLATFORM_ADC_REF_MAX,
}adc_reference_e;

/* adc参考电压值选择 */
typedef enum{
    PLATFORM_ADC_REF_1800MV = 1800,
    PLATFORM_ADC_REF_2200MV = 2200,
    PLATFORM_ADC_REF_2500MV = 2500,
    PLATFORM_ADC_REF_2800MV = 2800,
    PLATFORM_ADC_REF_3000MV = 3000,
    PLATFORM_ADC_REF_3300MV = 3300,
    PLATFORM_ADC_REF_3500MV = 3500,
    PLATFORM_ADC_REF_3700MV = 3700,
    PLATFORM_ADC_REF_4000MV = 4000,
    PLATFORM_ADC_REF_4200MV = 4200,
    PLATFORM_ADC_REF_5000MV = 5000,
}adc_reference_volt_e;

/* adc分辨率选择 */
typedef enum{
    PLATFORM_ADC_RESOLUTION_8BIT = 8 ,
    PLATFORM_ADC_RESOLUTION_10BIT = 10 ,
    PLATFORM_ADC_RESOLUTION_12BIT = 12 ,
    PLATFORM_ADC_RESOLUTION_16BIT = 16 ,
}adc_resolution_e;

/* adc参数结构体 */
typedef struct {
    vd_bsp_port_e 				id;               //adc索引id
	//以下参数通过配置实现，平台不需要的参数可赋空,由conf下配置
		vd_uint8_t 						module;				   	//模块/控制器的id
    vd_bsp_port_e 				pin;              //使用的pin脚
		vd_uint8_t 						channel;          //使用的通道号
    adc_reference_e 			ref_type;        	//参考电压类型
    adc_reference_volt_e 	ref_volt;         //参考电压值，单位毫伏
    adc_resolution_e 			resolution;       //多少位分辨率
    vd_bool_t 						use_dma;          //是否使用DMA
} vd_bsp_adc_t;


/* 定义ADC平台接口 */
typedef struct{
	vd_bool_t (*init)(vd_bsp_adc_t *adc);/* 初始化 */
	vd_int32_t (*read)(vd_bsp_adc_t *adc);/* 读 */
	vd_bool_t (*open)(vd_bsp_adc_t *adc);/* 打开 */
	vd_bool_t (*close)(vd_bsp_adc_t *adc);/* 关闭 */
}adc_ops_t;

/* 平台注册接口 */
typedef vd_bool_t (*adc_plat_register_t)(adc_ops_t *ops);

/* ======================================================================================
 * declaration
 */

/**
 * @brief 初始化ADC
 *
 * @param 	adc_id  	ADC 初始化对应ID号
 * @return 	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_init(vd_bsp_port_e adc_id);

/**
 * @brief  读取ADC数据
 *
 * @param 	adc_id 		ADC索引
 * @return 	非负值：	读取结果
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_read(vd_bsp_port_e adc_id);

/**
 * @brief  读取ADC电压值
 *
 * @param 	adc_id 		ADC索引
 * @return 	非负值：	读取结果
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_read_mv(vd_bsp_port_e adc_id);

/**
 * @brief  打开ADC
 *
 * @param 	adc_id 		ADC索引
 * @return 	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_open(vd_bsp_port_e adc_id);

/**
 * @brief  关闭ADC
 *
 * @param 	adc_id 		ADC索引
 * @return 	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_close(vd_bsp_port_e adc_id);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // _DRV_ADC_H
