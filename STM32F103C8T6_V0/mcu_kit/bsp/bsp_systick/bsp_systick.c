#include "bsp_systick.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"

volatile uint32_t systick_init_us = 0;  //ʱ��ϵ��
volatile uint32_t systick_count = 0;    //ʱ��ms����
volatile uint32_t systick_s_count = 0;  //ʱ��s����
volatile uint32_t systick_ms = 0;       //msʱ��Ƭ
volatile uint32_t systick_s = 0;        //sʱ��Ƭ

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
*	�������ƣ�	bsp_get_now_time
*	�������ܣ�	����ʱ��Ƭ����
*	��ڲ�����	tick_type��ʱ��Ƭ����
*	���ز�����	��ǰʱ��	
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
*	�������ƣ�	bsp_time_differ
*	�������ܣ�	��ȡʱ����
*	��ڲ�����	tick_type��ʱ��Ƭ���ͣ� now_time����ǰʱ��Ƭ
*	���ز�����	ʱ���ֵ	
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
*	�������ƣ�	SysTick_Handler
*	�������ܣ�	�δ�ʱ���жϴ�����
*	��ڲ�����	��
*	���ز�����	��	
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
