#ifndef __VD_BSP_ADC_TYPE_H
#define __VD_BSP_ADC_TYPE_H

#include "base.h"
#include "vd_bsp_type.h"


/* adc�ο��ź�ѡ�� */
typedef enum{
    PLATFORM_ADC_REF_IN,		/* �ڲ��ο��ź� */
    PLATFORM_ADC_REF_OUT,		/* �ⲿ�ο��ź� */
		PLATFORM_ADC_REF_MAX,
}adc_reference_e;

/* adcͨ��ѡ�� */
typedef enum{
    PLATFORM_ADC_CHANNEL0 = 0x01,		/* ͨ��0 */
    PLATFORM_ADC_CHANNEL1 = 0x02,		/* ͨ��1 */
	  PLATFORM_ADC_CHANNEL2 = 0x04,		/* ͨ��2 */
    PLATFORM_ADC_CHANNEL3 = 0x08,		/* ͨ��3 */
    PLATFORM_ADC_CHANNEL4 = 0x10,		/* ͨ��4 */
    PLATFORM_ADC_CHANNEL5 = 0x20,		/* ͨ��5 */
	  PLATFORM_ADC_CHANNEL6 = 0x40,		/* ͨ��6 */
    PLATFORM_ADC_CHANNEL7 = 0x80,		/* ͨ��7 */
		PLATFORM_ADC_CHANNEL_MAX = 0x08,
}adc_channel_e;

/* adc�����ṹ�� */
typedef struct {
    vd_bsp_port_e  		id;																				//adc����
		vd_bool_t					use_dma;																	//ʹ��dma
	
		adc_reference_e		reference;																//�ο��ź�ѡ��	ֵ adc_reference_e
		vd_bsp_port_e  		collect_pin[PLATFORM_ADC_CHANNEL_MAX];		//�ɼ�����
		vd_uint8_t				channel;																	//ͨ��		ÿbit��Ӧһ��ͨ��   ֵ adc_channel_e
} vd_bsp_adc_t;


/* ����ADCƽ̨�ӿ� */
typedef struct{
	vd_bool_t (*init)(vd_bsp_adc_t *adc);/* ��ʼ�� */
	vd_int32_t (*read)(vd_bsp_adc_t *adc, vd_uint8_t channel);/* �� */
	vd_bool_t (*open)(vd_bsp_adc_t *adc);/* �� */
	vd_bool_t (*close)(vd_bsp_adc_t *adc);/* �ر� */
}adc_plat_port_t;

/* ƽ̨ע��ӿ� */
typedef vd_bool_t (*adc_plat_register_t)(adc_plat_port_t *adc_plat_port);

#endif
