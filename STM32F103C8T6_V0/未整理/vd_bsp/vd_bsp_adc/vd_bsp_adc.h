/*
 * Project: MCU_kit
 * Module: DRV_ADC
 * File: vd_bsp_adc.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_adcͷ�ļ�
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
#define ADC_CHANNEL_NUM	8		//ADCͨ������

/* ======================================================================================
 * types
 */
/* adc�ο��ź�ѡ�� */
typedef enum{
    PLATFORM_ADC_REF_IN,		/* �ڲ��ο��ź� */
    PLATFORM_ADC_REF_OUT,		/* �ⲿ�ο��ź� */
		PLATFORM_ADC_REF_MAX,
}adc_reference_e;

/* adc�ο���ѹֵѡ�� */
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

/* adc�ֱ���ѡ�� */
typedef enum{
    PLATFORM_ADC_RESOLUTION_8BIT = 8 ,
    PLATFORM_ADC_RESOLUTION_10BIT = 10 ,
    PLATFORM_ADC_RESOLUTION_12BIT = 12 ,
    PLATFORM_ADC_RESOLUTION_16BIT = 16 ,
}adc_resolution_e;

/* adc�����ṹ�� */
typedef struct {
    vd_bsp_port_e 				id;               //adc����id
	//���²���ͨ������ʵ�֣�ƽ̨����Ҫ�Ĳ����ɸ���,��conf������
		vd_uint8_t 						module;				   	//ģ��/��������id
    vd_bsp_port_e 				pin;              //ʹ�õ�pin��
		vd_uint8_t 						channel;          //ʹ�õ�ͨ����
    adc_reference_e 			ref_type;        	//�ο���ѹ����
    adc_reference_volt_e 	ref_volt;         //�ο���ѹֵ����λ����
    adc_resolution_e 			resolution;       //����λ�ֱ���
    vd_bool_t 						use_dma;          //�Ƿ�ʹ��DMA
} vd_bsp_adc_t;


/* ����ADCƽ̨�ӿ� */
typedef struct{
	vd_bool_t (*init)(vd_bsp_adc_t *adc);/* ��ʼ�� */
	vd_int32_t (*read)(vd_bsp_adc_t *adc);/* �� */
	vd_bool_t (*open)(vd_bsp_adc_t *adc);/* �� */
	vd_bool_t (*close)(vd_bsp_adc_t *adc);/* �ر� */
}adc_ops_t;

/* ƽ̨ע��ӿ� */
typedef vd_bool_t (*adc_plat_register_t)(adc_ops_t *ops);

/* ======================================================================================
 * declaration
 */

/**
 * @brief ��ʼ��ADC
 *
 * @param 	adc_id  	ADC ��ʼ����ӦID��
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_init(vd_bsp_port_e adc_id);

/**
 * @brief  ��ȡADC����
 *
 * @param 	adc_id 		ADC����
 * @return 	�Ǹ�ֵ��	��ȡ���
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_read(vd_bsp_port_e adc_id);

/**
 * @brief  ��ȡADC��ѹֵ
 *
 * @param 	adc_id 		ADC����
 * @return 	�Ǹ�ֵ��	��ȡ���
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_read_mv(vd_bsp_port_e adc_id);

/**
 * @brief  ��ADC
 *
 * @param 	adc_id 		ADC����
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_open(vd_bsp_port_e adc_id);

/**
 * @brief  �ر�ADC
 *
 * @param 	adc_id 		ADC����
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_adc_close(vd_bsp_port_e adc_id);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // _DRV_ADC_H
