#include "vd_bsp_adc.h"
#include "vd_bsp_config.h"
#include "base_check.h"
#include "log.h"

vd_adc_t vd_g_register_adc_obj_arry[ADC_OBJ_MAX] = {0};   //已注册adc对象


/*
************************************************************
*	函数名称：	vd_bsp_adc_init
*	函数功能：	adc初始化
*	入口参数：	adc：adc对象
*	返回参数：	成功返回adc对象id， 失败返回<0	
************************************************************
*/
vd_int32_t vd_bsp_adc_init(vd_adc_t* adc) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	vd_int32_t  ret_adc_obj = -1,ret_value = -1;;
	uint32_t group_adrr = 0,gpio_ahb_periph = 0;;
	GPIO_TypeDef* group = NULL;
	uint16_t pin = 0, index = 0;

	vd_check_return_val_error(adc == vd_null, -VD_BSP_ERR_PARAMETER, "vd bsp adc init fail!\r\n");
	
	/*gpio get pin*/
	ret_value = vd_bsp_get_group_and_pin(adc->gpio_port, &group_adrr, &pin);
	group = (GPIO_TypeDef*) group_adrr;
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: get pin fail! \r\n",__FUNCTION__ );
	
	/*时钟初始化*/
	gpio_ahb_periph = vd_bsp_gpio_get_PeriphClock(group);
	RCC_APB2PeriphClockCmd(gpio_ahb_periph, ENABLE);         //GPIO时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	/*GPIO初始化*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(group, &GPIO_InitStructure);
		
	/*ADC初始化*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(adc->adc, &ADC_InitStructure);
	ADC_Cmd(adc->adc, ENABLE);
	
	ADC_TempSensorVrefintCmd(ENABLE);//打开内部参照电压
	/*ADC 校准*/
	ADC_ResetCalibration(adc->adc);
	while (ADC_GetResetCalibrationStatus(adc->adc) == SET) {};
	ADC_StartCalibration(adc->adc);
	while (ADC_GetCalibrationStatus(adc->adc) == SET) {};

	for(index = 0; index < ADC_OBJ_MAX; index++){		
		if(vd_g_register_adc_obj_arry[index].adc == NULL){
			vd_g_register_adc_obj_arry[index].adc = adc->adc;
			vd_g_register_adc_obj_arry[index].channel = adc->channel;
			vd_g_register_adc_obj_arry[index].gpio_port = adc->gpio_port;
			vd_g_register_adc_obj_arry[index].ref_type = adc->ref_type;
			vd_g_register_adc_obj_arry[index].ref_volt = adc->ref_volt;
			vd_g_register_adc_obj_arry[index].resolution = adc->resolution;
			vd_g_register_adc_obj_arry[index].use_dma = adc->use_dma;
			
			ret_value = index;
			break;
		}
	}
	
    return ret_value;
}

/*
************************************************************
*	函数名称：	vd_bsp_adc_read_value
*	函数功能：	读取adc值
*	入口参数：	adc_obj_id：adc对象id
*	返回参数：	成功返回adc值， 失败返回0
************************************************************
*/
vd_uint16_t vd_bsp_adc_read_value(vd_int32_t adc_obj_id) 
{
	vd_uint16_t   ret_value = 0;
	ADC_TypeDef * adc;               //ADC句柄
	vd_uint8_t    adc_channel;           //使用的通道号
	vd_int32_t    timer_out = 5000;
	
	vd_check_return_val_error(adc_obj_id < 0 || adc_obj_id > ADC_OBJ_MAX, ret_value, "parameter error!\r\n");
	
	adc = vd_g_register_adc_obj_arry[adc_obj_id].adc;
	adc_channel =  vd_g_register_adc_obj_arry[adc_obj_id].channel;
	
	ADC_RegularChannelConfig(adc, adc_channel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(adc, ENABLE);
	
	while(ADC_GetFlagStatus(adc, ADC_FLAG_EOC) == RESET){
		timer_out--;
		vd_check_return_val_error(timer_out == 0, ret_value, "read adc value fail!\r\n");    
	}
	ret_value = ADC_GetConversionValue(adc);	
	
	return ret_value;
	
}


/*
************************************************************
*	函数名称：	vd_bsp_adc_read_mv
*	函数功能：	读取电压值
*	入口参数：	adc_obj_id：adc对象id
*	返回参数：	成功返回电压值mv， 失败返回<0
************************************************************
*/
vd_uint32_t vd_bsp_adc_read_mv(vd_int32_t adc_obj_id)
{
	vd_uint32_t    ret_value = 0;
	vd_uint16_t   adc_value = 0;
	
	vd_check_return_val_error(adc_obj_id < 0 || adc_obj_id > ADC_OBJ_MAX, ret_value, "parameter error!\r\n");
	adc_value = vd_bsp_adc_read_value(adc_obj_id);
	
	ret_value = (adc_value * vd_g_register_adc_obj_arry[adc_obj_id].ref_volt) >> vd_g_register_adc_obj_arry[adc_obj_id].resolution;
	
	return ret_value;
}

