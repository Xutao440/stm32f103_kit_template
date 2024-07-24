#ifndef _DRV_ADC_H
#define _DRV_ADC_H

#include "base_type.h"
#include "vd_bsp_type.h"
#include "vd_bsp_gpio.h"

#define ADC_OBJ_MAX	    8     //注册最大adc对象个数

//ADC 参考电压选择
typedef enum{
    ADC_REF_IN,		/* 内部参考信号 */
    ADC_REF_OUT,	/* 外部参考信号 */
	ADC_REF_MAX,
	
}ADC_REF_E;

//ADC 参考电压值选择
typedef enum{
    ADC_REF_1800MV = 1800,
    ADC_REF_2200MV = 2200,
    ADC_REF_2500MV = 2500,
    ADC_REF_2800MV = 2800,
    ADC_REF_3000MV = 3000,
    ADC_REF_3300MV = 3300,
    ADC_REF_3500MV = 3500,
    ADC_REF_3700MV = 3700,
    ADC_REF_4000MV = 4000,
    ADC_REF_4200MV = 4200,
    ADC_REF_5000MV = 5000,
	
}ADC_REF_V_E;

//ADC 分辨率选择
typedef enum{
    ADC_RES_8BIT = 8,
    ADC_RES_10BIT = 10 ,
    ADC_RES_12BIT = 12 ,
    ADC_RES_16BIT = 16 ,
	
}ADC_RES_E;

//ADC 数据结构体
//((ADC_TypeDef *) ADC1_BASE)
typedef struct {
    GPIO_PORT_E 	gpio_port;         //自定义引脚号
	ADC_TypeDef *   adc;               //ADC句柄
	vd_uint8_t 		channel;           //使用的通道号
    ADC_REF_E 	    ref_type;          //参考电压类型
    ADC_REF_V_E 	ref_volt;          //参考电压值，单位毫伏
    ADC_RES_E 		resolution;        //多少位分辨率
    vd_bool_t 		use_dma;           //是否使用DMA
	
} vd_adc_t;


vd_int32_t vd_bsp_adc_init(vd_adc_t* adc);

vd_uint16_t vd_bsp_adc_read_value(vd_int32_t adc_obj_id) ;
vd_uint32_t vd_bsp_adc_read_mv(vd_int32_t adc_obj_id);

#endif  // _DRV_ADC_H

