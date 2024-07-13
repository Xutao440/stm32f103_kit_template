/*
 * Project: kit_sdk
 * Module: ntc
 * File: vd_fml_per_ntc.c
 * Created Date: 2022-11-05 14:53:10
 * Author: YangDingYuan
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022, Inc
 */

/* ======================================================================================
 * includes
 */
#include "vd_fml_per_ntc.h"
#include "control_uart.h"
/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */
static pid_calibration calibration = {.kp = 7, .ki = 0, .kd = 5};
static pid_state state = {.target = 45, .time_delta = 1, .previous_error = 0};
static vd_bool_t prd_enable = false;
/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
extern querystatus_cmd_t control_info;
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/**
 * @brief
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_ntc_init(void)
{
#if 0
	adc_InitParameter_t ADC_InitParam;

	system_set_port_mux(GPIO_PORT_D, GPIO_BIT_7, PORTD7_FUNC_SARADC7);

	system_regs->clk_gate.auxadc_clk_en = 1;
	system_regs->misc.saradc_apb_clk_en = 1;
	
	ADC_InitParam.ADC_CLK_DIV          = 5;
	ADC_InitParam.ADC_SetupDelay       = 80;
	ADC_InitParam.ADC_Reference        = ADC_REF_LDOIO;
	ADC_InitParam.FIFO_Enable          = FIFO_DISABLE;
	ADC_InitParam.FIFO_AlmostFullLevel = 0;

	adc_init(ADC_InitParam);

    adc_Channel_ConvertConfig(ADC_CHANNEL_0);

	adc_convert_enable();
	return true;
#endif
}
#if 0
/**
 * @brief �ײ��ȡadc
 * 
 * @return vd_uint16_t 
 */
vd_uint16_t adc_value_get(void){
    return adc_get_channel_data(0);
}
/**
 * @brief ��ȡtimes��adcֵ�����򣬱����쳣����Ӱ�죬������Ҫ����һЩʱ��
 * 
 * @param times 
 * @return vd_uint16_t 
 */
vd_uint16_t adc_average_get(vd_uint8_t times)
{
	unsigned char ii=0,nn=0;
	vd_uint16_t adc_temp = 0;
	vd_uint16_t ad_temp[100] = {0};
	 for(ii=0;ii<times;ii++)
   {
	  ad_temp[ii] =adc_value_get();
   }
	    for(ii=0;ii<times;ii++)
   {
	     for(nn=0;nn<times-ii;nn++)
	     {
	       if(ad_temp[nn]>ad_temp[nn+1])
	         {
		        adc_temp = ad_temp[nn];
		        ad_temp[nn]  = ad_temp[nn+1];
		        ad_temp[nn+1]= adc_temp;
	         }		  
	     }
   }
   	adc_temp = ad_temp[times/2];
  	return adc_temp;
}
/**
 * @brief ��ȡʵ���¶�
 * 
 * @return vd_uint8_t 
 */
int vd_fml_per_ntc_temp_get(void){
	int temp;
	vd_uint16_t Temperature_receive1;
	Temperature_receive1 = 1024-adc_average_get(10); //��Ϊntc����gdd����������������ȡadc�ɼ�����
	temp = 1000; //��ֵ���״ν�������ж�ѭ��
	// co_printf("Temperature_receive1=%d\n",Temperature_receive1);
	/************�ж�ntc�Ƿ�����***********/
	if(Temperature_receive1 > 10)
	{
		co_printf("Temperature_receive1=%d\n",Temperature_receive1);
		//ȡʵ���¶���ֵ
		//��ΪӲ�����⣬�¶Ƚϵ�ʱ�в��죬�˴�Ŀ�����Ż��˲��죬����ʹ���ۺ�ʵ���¶�ƥ��
		if(read_vd_fml_per_ntc_temperature(Temperature_receive1)<=38&&read_vd_fml_per_ntc_temperature(Temperature_receive1)>=35)
			temp=read_vd_fml_per_ntc_temperature(Temperature_receive1)-(38-read_vd_fml_per_ntc_temperature(Temperature_receive1))*2;
		else if(read_vd_fml_per_ntc_temperature(Temperature_receive1)>38)
			temp = read_vd_fml_per_ntc_temperature(Temperature_receive1);
		else 
			temp = read_vd_fml_per_ntc_temperature(Temperature_receive1)-6;
		if(temp == 1000)//�����ʾ����/����
		{
			co_printf("ADC = %d\n", Temperature_receive1);
			return 1;
		}
		else
		{
			return temp;
		}
	
	}
	else{
		return 2;
		co_printf("vd_fml_per_ntc connect?\n");
	}
}
#endif
/**
 * @brief pid������
 *
 * @param target
 * @param kp
 * @param ki
 * @param kd
 */
void vd_fml_per_ntc_target_set(vd_uint8_t target, vd_uint8_t kp, vd_uint8_t ki, vd_uint8_t kd)
{
	state.target = target;
	calibration.kp = kp;
	calibration.ki = ki;
	calibration.kd = kd;
}

/**
 * @brief pid����
 *
 */
void vd_fml_per_ntc_loop(void)
{

	if (prd_enable == true)
	{
		// ����Ϊpid�����¶Ȳ���
		//		state.actual=vd_fml_per_ntc_temp_get();//��ȡ��ǰ�¶�
		querystatus_reply_t *mcu_info = machine_tb001_get_querystatus();
		state.actual = mcu_info->mcu_kettle_temperature_value;
		// �¶ȵ���-10���϶Ȼ��߸���120���϶���ֹͣ����
		if ((mcu_info->mcu_kettle_temperature_type == 1 && mcu_info->mcu_kettle_temperature_value > 10) || (mcu_info->mcu_kettle_temperature_type == 0 && mcu_info->mcu_kettle_temperature_value > 120))
		{
			return;
		}
		vd_fml_per_ntc_temp_set(calibration, state); // ����pid����
		// ִ�з���
		if (state.actual < state.target)
		{
			//			drv_control_on(PLATFORM_CONTROL_HEAT);
			// �ṹ����ˮ������λ��Ϊ1
			control_info.device_kettle_heating_switch = 1;
			co_printf("device_kettle_heating_switch on\n");
		}
		else
		{
			//			drv_control_off(PLATFORM_CONTROL_HEAT);
			// �ṹ����ˮ������λ��Ϊ0
			control_info.device_kettle_heating_switch = 0;
			co_printf("device_kettle_heating_switch off\n");
		}
		// log
		//		co_printf("adc_value=%d\n",adc_average_get(10));
		//		co_printf("vd_fml_per_ntc_temp=%d\n",vd_fml_per_ntc_temp_get());
		co_printf("state.target=%d,calibration.kp=%d,calibration.ki=%d,calibration.kd=%d\n", state.target, calibration.kp, calibration.ki, calibration.kd);
	}
}
/**
 * @brief pid����ʹ�� trueΪʹ�� falseΪʧ��
 * 			target ������Ŀ���¶�
 *
 * @param enable
 * @param target
 */
void pid_adjusting_enable(vd_bool_t enable, vd_uint8_t target)
{
	prd_enable = enable;
	state.target = target;
}
///**
// * @brief �ߵ��¼�� 0������ 1�������쳣 2�������쳣
// *
// * @return vd_uint8_t
// */
// vd_uint8_t temp_error_check(){
//	vd_uint16_t adc_value=adc_average_get(10);
//	if(adc_value<96)
//		return 1;
//	else if(adc_value>837)
//		return 2;
//	else return 0;
//}
/* ======================================================================================
 * implementation
 */
