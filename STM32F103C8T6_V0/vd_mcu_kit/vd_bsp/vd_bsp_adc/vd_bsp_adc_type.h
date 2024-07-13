#ifndef __VD_BSP_ADC_TYPE_H
#define __VD_BSP_ADC_TYPE_H

#include "base.h"
#include "vd_bsp_type.h"


/* adc参考信号选择 */
typedef enum{
    PLATFORM_ADC_REF_IN,		/* 内部参考信号 */
    PLATFORM_ADC_REF_OUT,		/* 外部参考信号 */
		PLATFORM_ADC_REF_MAX,
}adc_reference_e;

/* adc通道选择 */
typedef enum{
    PLATFORM_ADC_CHANNEL0 = 0x01,		/* 通道0 */
    PLATFORM_ADC_CHANNEL1 = 0x02,		/* 通道1 */
	  PLATFORM_ADC_CHANNEL2 = 0x04,		/* 通道2 */
    PLATFORM_ADC_CHANNEL3 = 0x08,		/* 通道3 */
    PLATFORM_ADC_CHANNEL4 = 0x10,		/* 通道4 */
    PLATFORM_ADC_CHANNEL5 = 0x20,		/* 通道5 */
	  PLATFORM_ADC_CHANNEL6 = 0x40,		/* 通道6 */
    PLATFORM_ADC_CHANNEL7 = 0x80,		/* 通道7 */
		PLATFORM_ADC_CHANNEL_MAX = 0x08,
}adc_channel_e;

/* adc参数结构体 */
typedef struct {
    vd_bsp_port_e  		id;																				//adc索引
		vd_bool_t					use_dma;																	//使用dma
	
		adc_reference_e		reference;																//参考信号选择	值 adc_reference_e
		vd_bsp_port_e  		collect_pin[PLATFORM_ADC_CHANNEL_MAX];		//采集引脚
		vd_uint8_t				channel;																	//通道		每bit对应一个通道   值 adc_channel_e
} vd_bsp_adc_t;


/* 定义ADC平台接口 */
typedef struct{
	vd_bool_t (*init)(vd_bsp_adc_t *adc);/* 初始化 */
	vd_int32_t (*read)(vd_bsp_adc_t *adc, vd_uint8_t channel);/* 读 */
	vd_bool_t (*open)(vd_bsp_adc_t *adc);/* 打开 */
	vd_bool_t (*close)(vd_bsp_adc_t *adc);/* 关闭 */
}adc_plat_port_t;

/* 平台注册接口 */
typedef vd_bool_t (*adc_plat_register_t)(adc_plat_port_t *adc_plat_port);

#endif
