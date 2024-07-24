#include "vd_bsp_pwm.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"

vd_pwm_t vd_g_register_pwm_obj_arry[PWM_MAX] = {0};   //已注册pwm对象
static vd_uint16_t pwm_obj_register_mask = 0;

vd_pwm_port_map_t vd_g_pwm_port_map[PWM_MAP_MAX] = {
		{GPIO_PA0,  TIM2, TIM_Channel_1}, 
		{GPIO_PA1,  TIM2, TIM_Channel_2}, 
		{GPIO_PA2,  TIM2, TIM_Channel_3}, 
		{GPIO_PA3,  TIM2, TIM_Channel_4}, 
		{GPIO_PA8,  TIM1, TIM_Channel_1}, 
		{GPIO_PA9,  TIM1, TIM_Channel_2}, 
		{GPIO_PA10, TIM1, TIM_Channel_3}, 
		{GPIO_PA11, TIM1, TIM_Channel_4}, 
};

/*
************************************************************
*	函数名称：	vd_bsp_pwm_find_map_id
*	函数功能：	找到对应port——map 的id号
*	入口参数：	gpio_port：自定义端口号
*	返回参数：	成功返回map的id， 失败返回<0	
************************************************************
*/
static vd_int8_t vd_bsp_pwm_find_map_id(GPIO_PORT_E gpio_port)
{
	vd_int8_t index = -1, ret_value = -1;

	vd_check_return_val_error(gpio_port > GPIO_MAX, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: EINVAL \r\n",__FUNCTION__ );
	for(index = 0; index < PWM_MAP_MAX; index++){
		if(vd_g_pwm_port_map[index].gpio_port == gpio_port){
			ret_value = index;
			break;
		}
	}
	vd_check_return_val_error(index >= PWM_MAP_MAX, -VD_BSP_ERR_SDK, "[ %s ]: EINVAL \r\n",__FUNCTION__ );
	
	return ret_value;
}

/*
************************************************************
*	函数名称：	vd_bsp_TIM_OC1Init
*	函数功能：	pwm初始化
*	入口参数：	gpio_port：自定义端口号
*	返回参数：	成功返回map的id， 失败返回<0	
************************************************************
*/
static vd_int32_t vd_bsp_TIM_OC1Init(GPIO_PORT_E gpio_port, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
	vd_int8_t  pwm_port_map_id = -1;
	vd_pwm_port_map_t* pwm_port_map = NULL;
	
	vd_check_return_val(TIM_OCInitStruct == vd_null, -VD_BSP_ERR_PORT_INVALID);
	
	pwm_port_map_id = vd_bsp_pwm_find_map_id(gpio_port);
	vd_check_return_val(pwm_port_map_id < 0, -VD_BSP_ERR_SDK);
	pwm_port_map = &vd_g_pwm_port_map[pwm_port_map_id];
	
	log_info("TIM_OC1Init\r\n");
	if(pwm_port_map->chnanle == pwm_channel_1){
			TIM_OC1Init(pwm_port_map->timer, TIM_OCInitStruct);			
		}else if(pwm_port_map->chnanle == pwm_channel_2){		
			TIM_OC2Init(TIM2, TIM_OCInitStruct);
			log_info("TIM_OC2Init!!!!!!!!!!\r\n");
		}else if(pwm_port_map->chnanle == pwm_channel_3){
			TIM_OC3Init(pwm_port_map->timer, TIM_OCInitStruct);
		}else if(pwm_port_map->chnanle == pwm_channel_4){
			TIM_OC4Init(pwm_port_map->timer, TIM_OCInitStruct);
	}
		
	return VD_BSP_NOERROR;
}

/*
************************************************************
*	函数名称：	vd_bsp_TIM_SetCompare
*	函数功能：	设置占空比
*	入口参数：	pwm_obj_id：pwm 对象， duty：占空比
*	返回参数：	成功返回0， 失败返回<0	
************************************************************
*/
 static vd_int32_t vd_bsp_TIM_SetCompare(vd_int32_t pwm_obj_id, vd_uint32_t duty)
 {
	 vd_int32_t ret_value = VD_BSP_NOERROR;
	 vd_int8_t  pwm_port_map_id = -1;
	 vd_pwm_port_map_t* pwm_port_map = NULL;
	 
	vd_check_return_val(pwm_obj_id > PWM_MAX || pwm_obj_id < 0, -VD_BSP_ERR_PARAMETER);
	 
	pwm_port_map_id = vd_bsp_pwm_find_map_id(vd_g_register_pwm_obj_arry[pwm_obj_id].gpio_port);
	vd_check_return_val(pwm_port_map_id < 0, -VD_BSP_ERR_SDK);
	pwm_port_map = &vd_g_pwm_port_map[pwm_port_map_id];
	 
	switch(pwm_port_map->chnanle){
		case pwm_channel_1:  TIM_SetCompare1(TIM2, duty);  break;
		case pwm_channel_2:  TIM_SetCompare2(TIM2, duty);  break;
		case pwm_channel_3:  TIM_SetCompare3(TIM2, duty);  break;
		case pwm_channel_4:  TIM_SetCompare4(TIM2, duty);  break;	
		default: 
			ret_value = -VD_BSP_ERR_SDK;  break;
	}
	return ret_value;
 }
 
/*
************************************************************
*	函数名称：	vd_bsp_pwm_init
*	函数功能：	pwm初始化
*	入口参数：	timer_id：定时器id, pwm_fre_hz：频率(hz), pwm_per_duty:占空比 10%则10
*	返回参数：	成功返回pwm对象id， 失败返回<0	
************************************************************
*/
vd_int32_t vd_bsp_pwm_init(vd_pwm_t* pwm)
{
	vd_uint32_t gpio_ahb_periph = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint32_t group_adrr = 0;
	GPIO_TypeDef* group = NULL;
	uint16_t pin = 0, index = 0;
	vd_int32_t ret_value = 0;
	vd_int8_t  pwm_port_map_id = -1;
	vd_pwm_port_map_t* pwm_port_map = NULL;
	
	vd_check_return_val(pwm == vd_null, -VD_BSP_ERR_PORT_INVALID);
	
	pwm_port_map_id = vd_bsp_pwm_find_map_id(pwm->gpio_port);
	vd_check_return_val(pwm_port_map_id < 0, -VD_BSP_ERR_SDK);
	pwm_port_map = &vd_g_pwm_port_map[pwm_port_map_id];	
	
	/*1.gpio get pin*/
	ret_value = vd_bsp_get_group_and_pin(pwm->gpio_port, &group_adrr, &pin);
	group = (GPIO_TypeDef*) group_adrr;
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: get pin fail! \r\n",__FUNCTION__ );
	
	/*GPIO时钟*/
	gpio_ahb_periph = vd_bsp_gpio_get_PeriphClock(group);
	RCC_APB2PeriphClockCmd(gpio_ahb_periph, ENABLE);         //GPIO时钟	
	if(pwm_port_map->timer == TIM1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //设置定时器时钟 		 
	}else if(pwm_port_map->timer == TIM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);     //设置定时器时钟
	}
	
	/*GPIO初始化*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(group, &GPIO_InitStructure);
	
	//通用定时器初始化	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = pwm->period - 1;		//ARR	
	if((pwm->fre_offset) > 0){
		TIM_TimeBaseInitStructure.TIM_Prescaler = ((SystemCoreClock / 100000 + pwm->fre_offset) * (1000 / pwm->pwm_fre_hz)) - 1 ; 		
	}else{
		TIM_TimeBaseInitStructure.TIM_Prescaler = ((SystemCoreClock / 100000 - vd_abs(pwm->fre_offset)) * (1000 / pwm->pwm_fre_hz)) - 1 ; 	
	}
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_InternalClockConfig(pwm_port_map->timer);
	TIM_TimeBaseInit(pwm_port_map->timer, &TIM_TimeBaseInitStructure);
	
	/*pwm初始化*/
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	if(pwm->gpio_default_state == VD_GPIO_HIGH){
		TIM_OCInitStructure.TIM_Pulse = 100;		//占空比100%
	}else{
		TIM_OCInitStructure.TIM_Pulse = 0;		    //占空比0%
	}	
	ret_value = vd_bsp_TIM_OC1Init(pwm->gpio_port, &TIM_OCInitStructure);
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_PARAMETER, "[ %s ]: TIM OC1Init fail! \r\n",__FUNCTION__ );
	
	TIM_Cmd(pwm_port_map->timer, ENABLE);		
	
	for(index = 0; index < PWM_MAX; index++){		
		if(((pwm_obj_register_mask >> index) & 0x01) == 0){
			vd_g_register_pwm_obj_arry[index].fre_offset = pwm->fre_offset;
			vd_g_register_pwm_obj_arry[index].gpio_port = pwm->gpio_port;
			vd_g_register_pwm_obj_arry[index].period = pwm->period;
			vd_g_register_pwm_obj_arry[index].pwm_duty = pwm->pwm_duty;
			vd_g_register_pwm_obj_arry[index].pwm_fre_hz = pwm->pwm_fre_hz;
			vd_g_register_pwm_obj_arry[index].gpio_default_state = pwm->gpio_default_state;
			ret_value = index;
			pwm_obj_register_mask |= 0x0001 << index;
			break;
		}
	}
	vd_check_return_val_error(index >= PWM_MAX, -VD_BSP_ERR_SDK, "[ %s ]: no index register\r\n",__FUNCTION__);
	
	
    return ret_value;
}


/*
************************************************************
*	函数名称：	vd_bsp_pwm_start
*	函数功能：	开启pwm
*	入口参数：	pwm_obj_id： pwm对象id
*	返回参数：	成功返回0， 失败返回<0
************************************************************
*/
vd_int32_t vd_bsp_pwm_start(vd_int32_t pwm_obj_id)
{
    vd_int32_t ret_value = VD_BSP_NOERROR;
	vd_uint16_t      pwm_duty;     //占空比
	
    vd_check_return_val(pwm_obj_id > PWM_MAX || pwm_obj_id < 0, -VD_BSP_ERR_PARAMETER);
	
	pwm_duty = vd_g_register_pwm_obj_arry[pwm_obj_id].pwm_duty;
	ret_value = vd_bsp_TIM_SetCompare(pwm_obj_id, pwm_duty);

    return ret_value;     
}


/*
************************************************************
*	函数名称：	vd_bsp_pwm_stop
*	函数功能：	停止pwm
*	入口参数：	pwm_obj_id：pwm对象id
*	返回参数：	成功返回0， 失败返回<0
************************************************************
*/
vd_int32_t vd_bsp_pwm_stop(vd_int32_t pwm_obj_id)
{
    vd_int32_t ret_value = VD_BSP_NOERROR;
	
    vd_check_return_val(pwm_obj_id > PWM_MAX || pwm_obj_id < 0, -VD_BSP_ERR_PARAMETER);
	
	if(vd_g_register_pwm_obj_arry[pwm_obj_id].gpio_default_state == VD_GPIO_HIGH){
		ret_value = vd_bsp_TIM_SetCompare(pwm_obj_id, 100);			
	}else{
		ret_value = vd_bsp_TIM_SetCompare(pwm_obj_id, 0);	
	}
		
    return ret_value;     
}

/*
************************************************************
*	函数名称：	vd_bsp_pwm_fake_gpio_set_value
*	函数功能：	模拟gpio设置value
*	入口参数：	pwm_obj_id：pwm对象id
*	返回参数：	成功返回0， 失败返回<0
************************************************************
*/
vd_int32_t vd_bsp_pwm_fake_gpio_set_value(vd_int32_t pwm_obj_id, GPIO_STATE_E value)
{
	vd_int32_t ret_value = VD_BSP_NOERROR;
	
	vd_check_return_val(pwm_obj_id > PWM_MAX || pwm_obj_id < 0, -VD_BSP_ERR_PARAMETER);
	
	ret_value = vd_bsp_TIM_SetCompare(pwm_obj_id, value == VD_GPIO_HIGH? 100 : 0);
		
	return ret_value;
}

/*
************************************************************
*	函数名称：	vd_bsp_pwm_set_duty
*	函数功能：	设置占空比
*	入口参数：	pwm_obj_id：pwm对象id， pwm_duty：占空比
*	返回参数：	成功返回0， 失败返回<0
************************************************************
*/
vd_int32_t vd_bsp_pwm_set_duty(vd_int32_t pwm_obj_id, vd_uint32_t pwm_duty) 
{
	vd_int32_t ret_value = VD_BSP_NOERROR;
	vd_check_return_val(pwm_obj_id > PWM_MAX || pwm_obj_id < 0, -VD_BSP_ERR_PARAMETER);	
	
	ret_value = vd_bsp_TIM_SetCompare(pwm_obj_id, pwm_duty);
	
	return ret_value;
}

