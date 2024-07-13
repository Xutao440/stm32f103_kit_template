#ifndef __VD_BSP_CONFIG_H
#define __VD_BSP_CONFIG_H

/******************* pwm *******************/
#define PWM_PORT_MAP  {                   \
		{GPIO_PA0,  TIM2, TIM_Channel_1}, \
		{GPIO_PA1,  TIM2, TIM_Channel_2}, \
		{GPIO_PA2,  TIM2, TIM_Channel_3}, \
		{GPIO_PA3,  TIM2, TIM_Channel_4}, \
		{GPIO_PA8,  TIM1, TIM_Channel_1}, \
		{GPIO_PA9,  TIM1, TIM_Channel_2}, \
		{GPIO_PA10, TIM1, TIM_Channel_3}, \
		{GPIO_PA11, TIM1, TIM_Channel_4}, \
	} 

#include "vd_bsp_dwt.h"
#include "vd_bsp_pwm.h"
#include "vd_bsp_systick.h"
#include "vd_bsp_uart.h"
#include "vd_bsp_adc.h"


#endif
