/**
 * @file vdchip40xx_50xx_adc.c
 * @author liyongshaung
 * @brief 
 * @version 0.1
 * @date 2023-06-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* ======================================================================================
 * log
 */


/* ======================================================================================
 * includes
 */
#include "vdchip40xx_50xx_adc/vdchip40xx_50xx_adc.h"
#include "driver_adc.h"
#include "vd_log.h"

/* ======================================================================================
 * macros
 */


/* ======================================================================================
 * types
 */

/* 定义adc驱动对象 */
typedef struct{
	adc_InitParameter_t 			handle;		/* adc句柄 */
}vdchip_40xx_50xx_adc_t;


/* ======================================================================================
 * globals
 */
vdchip_40xx_50xx_adc_t adc0 = {0};/* adc0驱动对象 */

/* ======================================================================================
 * helper
 */

/* 平台接口*/
static vd_bool_t vdchip_40xx_50xx_adc_init(vd_bsp_adc_t *adc);/* 初始化 */
static vd_bool_t vdchip_40xx_50xx_adc_open(vd_bsp_adc_t *adc);/* 打开 */
static vd_bool_t vdchip_40xx_50xx_adc_close(vd_bsp_adc_t *adc);/* 关闭 */
static vd_int32_t vdchip_40xx_50xx_adc_read(vd_bsp_adc_t *adc);/* 读 */


/* ======================================================================================
 * implementation
 */
/**
 * @brief adc平台注册
 *
 * @param ops		adc平台接口
 * @return vd_bool_t			注册结果
 */
vd_bool_t vdchip_40xx_50xx_adc_register(adc_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/* 接口注册 */
	ops->init = vdchip_40xx_50xx_adc_init;
	ops->read = vdchip_40xx_50xx_adc_read;
	ops->open = vdchip_40xx_50xx_adc_open;
	ops->close = vdchip_40xx_50xx_adc_close;
	
	return vd_true;
}

/**
 * @brief adc初始化
 *
 * @param adc  					adc参数
 * @return vd_bool_t		初始化结果
 */
static vd_bool_t vdchip_40xx_50xx_adc_init(vd_bsp_adc_t *adc)
{
	vdchip_40xx_50xx_adc_t *adcx = vd_null;
	
	/* 检查adc iD */
	if(adc->id == VD_PORT_ADC0){
		adcx = &adc0;
	}
	
	vd_check_return_val_error(adcx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	vd_check_return_val_error(adc->channel > 7, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	__SYSTEM_ADC_CLK_ENABLE();
	
	/* 配置初始化参数 */
	adcx->handle.ADC_CLK_DIV          = 1;
	adcx->handle.ADC_SetupDelay       = 80;
	adcx->handle.ADC_Reference = (adc->ref_type == PLATFORM_ADC_REF_IN) ? ADC_REF_1P2V : ADC_REF_LDOIO;
	adcx->handle.FIFO_Enable          = FIFO_DISABLE;
	adcx->handle.FIFO_AlmostFullLevel = 0;

	/* adc初始化 */
	adc_init(adcx->handle);

  adc_Channel_ConvertConfig(0x01 << adc->channel);
	
	/*ADC引脚初始化*/
//	for(vd_uint8_t i = 0; i < PLATFORM_ADC_CHANNEL_MAX; i++){
//		if((0x01 << i) & adc->channel){/* 该通道已启用 */
//			system_set_port_mux(adc->pin[i] / 8, adc->pin[i] % 8, 0x08);//PORTD7_FUNC_SARADC7
//		}
//	}
	system_set_port_mux(adc->pin / 8, adc->pin % 8, 0x08);//PORTD7_FUNC_SARADC7
	
  return vd_true;
}

/**
 * @brief adc打开
 *
 * @param adc  					adc参数	
 * @return vd_bool_t		打开结果
 */
static vd_bool_t vdchip_40xx_50xx_adc_open(vd_bsp_adc_t *adc)
{
	vdchip_40xx_50xx_adc_t *adcx = vd_null;
	
	/* 检查adc iD */
	if(adc->id == VD_PORT_ADC0){
		adcx = &adc0;
	}
	vd_check_return_val_error(adcx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	adc_convert_enable();
	
	return vd_true;
}

/**
 * @brief adc关闭
 *
 * @param adc  					adc参数	
 * @return vd_bool_t		关闭结果
 */
static vd_bool_t vdchip_40xx_50xx_adc_close(vd_bsp_adc_t *adc)
{
	vdchip_40xx_50xx_adc_t *adcx = vd_null;
	
	/* 检查adc iD */
	if(adc->id == VD_PORT_ADC0){
		adcx = &adc0;
	}
	vd_check_return_val_error(adcx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	adc_convert_disable();
	
	return vd_true;
}

/**
 * @brief adc读
 *
 * @param adc  					adc参数	
 * @return vd_bool_t		读取结果
 */
static vd_int32_t vdchip_40xx_50xx_adc_read(vd_bsp_adc_t *adc)
{
	vd_int32_t ret = 0;
	vdchip_40xx_50xx_adc_t *adcx = vd_null;
	
	/* 检查adc iD */
	if(adc->id == VD_PORT_ADC0){
		adcx = &adc0;
	}
	vd_check_return_val_error(adcx == vd_null, -1, "in func:%s, line:%d", __func__, __LINE__);
	vd_check_return_val_error(adc->channel > 7, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/* adc读 */
	ret = adc_get_channel_data(adc->channel);
	
  return ret;
}




