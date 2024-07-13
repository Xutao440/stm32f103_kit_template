/*
 * Project: rf01
 * Module: module_name
 * File: vd_bsp_pwm.c
 * Created Date: 2022-05-05 10:57:01
 * Author: lijianzhong
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#include "vdchip_40xx_50xx_pwm/vdchip_40xx_50xx_pwm.h"
#include "vdchip40xx_50xx_gpio/vdchip40xx_50xx_gpio.h"

#include "driver_pwm.h"
#include "driver_gpio.h"
/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (0)


/**
 * @brief pwm init 
 * 
 * @param pwm_dev 
 * @return ** vd_bool_t 
 */
vd_bool_t vd_fr8000_pwm_init(vd_bsp_pwm_t *pwm, vd_uint32_t pwm_frequency,vd_uint32_t pwm_duty)
{
    vd_check_return_val(pwm == vd_null, vd_false);
    GPIO_InitTypeDef  VD_GPIO_Handle;
    struct_PWM_Config_t VD_PWM_Config;

    //open clk
     __SYSTEM_PWM_CLK_ENABLE();  
    // pin 
    VD_GPIO_Handle.Pin = (1<<(pwm->pin%8));
    VD_GPIO_Handle.Mode = GPIO_MODE_AF_PP;
    VD_GPIO_Handle.Pull = GPIO_PULLUP;
    VD_GPIO_Handle.Alternate = GPIO_FUNCTION_6;
    gpio_init(pwm->pin/8,&VD_GPIO_Handle);

    VD_PWM_Config.Prescale = system_get_clock_config();
    VD_PWM_Config.Period = (1000000/pwm_frequency);
    VD_PWM_Config.Posedge =0;
    VD_PWM_Config.Negedge =pwm_duty;
    pwm_config(1<<(pwm->channel), VD_PWM_Config);    
	
    return vd_true;
}

/**
 * @brief :pwm使用接口
 *
 * @param pwm_id ：id号
 * @param vd_bsp_pwm_freq ：输出频率
 * @param vd_bsp_pwm_duty ：占空比
 * @return vd_int_t
 */
vd_bool_t vd_fr8000_pwm_start(vd_bsp_pwm_t *pwm)
{
   vd_check_return_val(pwm==vd_null, vd_false);
    pwm_output_enable(1<<(pwm->channel));
    return vd_true;
     
}


/**
 * @brief :pwm停止
 *
 * @param pwm_id ：id号
 * @param vd_bsp_pwm_freq ：输出频率
 * @param vd_bsp_pwm_duty ：占空比
 * @return vd_int_t
 */
vd_bool_t vd_fr8000_pwm_stop(vd_bsp_pwm_t *pwm)
{
   vd_check_return_val(pwm==vd_null, vd_false);
    pwm_output_disable(1<<(pwm->channel));
    return vd_true;
}

/**
 * @brief :pwm设置
 *
 * @param pwm_id ：id号
 * @param vd_bsp_pwm_freq ：输出频率
 * @param vd_bsp_pwm_duty ：占空比
 * @return vd_int_t
 */
vd_bool_t vd_fr8000_pwm_set(vd_bsp_pwm_t *pwm, vd_uint32_t pwm_frequency, vd_uint32_t pwm_duty)
{
  vd_check_return_val(pwm==vd_null, vd_false);
    
	struct_PWM_Config_t VD_PWM_Config;

	VD_PWM_Config.Prescale = system_get_clock_config();
	VD_PWM_Config.Period = (1000000/pwm_frequency);
	VD_PWM_Config.Posedge =0;
	VD_PWM_Config.Negedge =pwm_duty;
	pwm_config(1<<(pwm->channel), VD_PWM_Config); 
	
  return vd_true;
}


vd_bool_t vd_fr8000_pwm_register(vd_pwm_ops_t *ops)
{
    ops->init = vd_fr8000_pwm_init;
    ops->start = vd_fr8000_pwm_start;
    ops->stop = vd_fr8000_pwm_stop;
		ops->set = vd_fr8000_pwm_set;
}
