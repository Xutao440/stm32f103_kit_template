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

/* ����adc�������� */
typedef struct{
	adc_InitParameter_t 			handle;		/* adc��� */
}vdchip_40xx_50xx_adc_t;


/* ======================================================================================
 * globals
 */
vdchip_40xx_50xx_adc_t adc0 = {0};/* adc0�������� */

/* ======================================================================================
 * helper
 */

/* ƽ̨�ӿ�*/
static vd_bool_t vdchip_40xx_50xx_adc_init(vd_bsp_adc_t *adc);/* ��ʼ�� */
static vd_bool_t vdchip_40xx_50xx_adc_open(vd_bsp_adc_t *adc);/* �� */
static vd_bool_t vdchip_40xx_50xx_adc_close(vd_bsp_adc_t *adc);/* �ر� */
static vd_int32_t vdchip_40xx_50xx_adc_read(vd_bsp_adc_t *adc);/* �� */


/* ======================================================================================
 * implementation
 */
/**
 * @brief adcƽ̨ע��
 *
 * @param ops		adcƽ̨�ӿ�
 * @return vd_bool_t			ע����
 */
vd_bool_t vdchip_40xx_50xx_adc_register(adc_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/* �ӿ�ע�� */
	ops->init = vdchip_40xx_50xx_adc_init;
	ops->read = vdchip_40xx_50xx_adc_read;
	ops->open = vdchip_40xx_50xx_adc_open;
	ops->close = vdchip_40xx_50xx_adc_close;
	
	return vd_true;
}

/**
 * @brief adc��ʼ��
 *
 * @param adc  					adc����
 * @return vd_bool_t		��ʼ�����
 */
static vd_bool_t vdchip_40xx_50xx_adc_init(vd_bsp_adc_t *adc)
{
	vdchip_40xx_50xx_adc_t *adcx = vd_null;
	
	/* ���adc iD */
	if(adc->id == VD_PORT_ADC0){
		adcx = &adc0;
	}
	
	vd_check_return_val_error(adcx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	vd_check_return_val_error(adc->channel > 7, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	__SYSTEM_ADC_CLK_ENABLE();
	
	/* ���ó�ʼ������ */
	adcx->handle.ADC_CLK_DIV          = 1;
	adcx->handle.ADC_SetupDelay       = 80;
	adcx->handle.ADC_Reference = (adc->ref_type == PLATFORM_ADC_REF_IN) ? ADC_REF_1P2V : ADC_REF_LDOIO;
	adcx->handle.FIFO_Enable          = FIFO_DISABLE;
	adcx->handle.FIFO_AlmostFullLevel = 0;

	/* adc��ʼ�� */
	adc_init(adcx->handle);

  adc_Channel_ConvertConfig(0x01 << adc->channel);
	
	/*ADC���ų�ʼ��*/
//	for(vd_uint8_t i = 0; i < PLATFORM_ADC_CHANNEL_MAX; i++){
//		if((0x01 << i) & adc->channel){/* ��ͨ�������� */
//			system_set_port_mux(adc->pin[i] / 8, adc->pin[i] % 8, 0x08);//PORTD7_FUNC_SARADC7
//		}
//	}
	system_set_port_mux(adc->pin / 8, adc->pin % 8, 0x08);//PORTD7_FUNC_SARADC7
	
  return vd_true;
}

/**
 * @brief adc��
 *
 * @param adc  					adc����	
 * @return vd_bool_t		�򿪽��
 */
static vd_bool_t vdchip_40xx_50xx_adc_open(vd_bsp_adc_t *adc)
{
	vdchip_40xx_50xx_adc_t *adcx = vd_null;
	
	/* ���adc iD */
	if(adc->id == VD_PORT_ADC0){
		adcx = &adc0;
	}
	vd_check_return_val_error(adcx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	adc_convert_enable();
	
	return vd_true;
}

/**
 * @brief adc�ر�
 *
 * @param adc  					adc����	
 * @return vd_bool_t		�رս��
 */
static vd_bool_t vdchip_40xx_50xx_adc_close(vd_bsp_adc_t *adc)
{
	vdchip_40xx_50xx_adc_t *adcx = vd_null;
	
	/* ���adc iD */
	if(adc->id == VD_PORT_ADC0){
		adcx = &adc0;
	}
	vd_check_return_val_error(adcx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	adc_convert_disable();
	
	return vd_true;
}

/**
 * @brief adc��
 *
 * @param adc  					adc����	
 * @return vd_bool_t		��ȡ���
 */
static vd_int32_t vdchip_40xx_50xx_adc_read(vd_bsp_adc_t *adc)
{
	vd_int32_t ret = 0;
	vdchip_40xx_50xx_adc_t *adcx = vd_null;
	
	/* ���adc iD */
	if(adc->id == VD_PORT_ADC0){
		adcx = &adc0;
	}
	vd_check_return_val_error(adcx == vd_null, -1, "in func:%s, line:%d", __func__, __LINE__);
	vd_check_return_val_error(adc->channel > 7, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/* adc�� */
	ret = adc_get_channel_data(adc->channel);
	
  return ret;
}




