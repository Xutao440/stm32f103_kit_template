#include "bsp_systick.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"

volatile uint32_t systick_init_us = 0;  //时间系数
volatile uint32_t systick_count = 0;    //时间ms计数
volatile uint32_t systick_s_count = 0;  //时间s计数
volatile uint32_t systick_ms = 0;       //ms时间片
volatile uint32_t systick_s = 0;        //s时间片

/**
* @brief    : This function will be used to initialize SysTick.
* @param    : ticks
* SystemFrequency / 1000    1ms raise interrupt
* SystemFrequency / 100000	 10us raise interrupt
* SystemFrequency / 1000000 1us raise interrupt
* @retval   : None
*/
void bsp_systick_init(uint32_t time_us)
{
    SysTick_Config(((SystemCoreClock/1000000) * time_us));
	/** Initialization Systick interrupt */
    NVIC_SetPriority(SysTick_IRQn, 1);	
	systick_init_us = time_us;

}
void Clear_Over_Flag(void)
{
    SysTick->CTRL;
}

/*
************************************************************
*	函数名称：	bsp_get_now_time
*	函数功能：	更新时间片函数
*	入口参数：	tick_type：时间片类型
*	返回参数：	当前时间	
************************************************************
*/
uint32_t bsp_get_now_time(tick_type_e tick_type)
{
	uint32_t ret_tick = 0;
	
	if(tick_type == TICK_TYEP_MS){
		ret_tick = systick_ms;
	}else if(tick_type == TICK_TYEP_S){
		ret_tick = systick_s;
	}

	return 	ret_tick;
}


/*
************************************************************
*	函数名称：	bsp_time_differ
*	函数功能：	获取时间差函数
*	入口参数：	tick_type：时间片类型， now_time：当前时间片
*	返回参数：	时间差值	
************************************************************
*/
uint32_t bsp_time_differ(tick_type_e tick_type, uint32_t now_time)
{
	uint32_t ret_tick = 0;
	
	if(tick_type == TICK_TYEP_MS){
		ret_tick = systick_ms - now_time;
		
	}else if(tick_type == TICK_TYEP_S){
		ret_tick = systick_s - now_time;
	}
	
	return ret_tick;
}

/*
************************************************************
*	函数名称：	SysTick_Handler
*	函数功能：	滴答定时器中断处理函数
*	入口参数：	无
*	返回参数：	无	
************************************************************
*/
void SysTick_Handler(void)
{	
    Clear_Over_Flag();
	systick_count++;
	if(systick_count * systick_init_us >= 1000){
		systick_count = 0;
		systick_ms++;
		systick_s_count++;
		if(systick_s_count >= 1000){
			systick_s_count = 0;
			systick_s++;		
		}
	}
}
