#include "bsp_dwt.h"

/*
************************************************************
*	函数名称：	DWT_Dalay_Init
*	函数功能：	DWT延时初始化函数
*	入口参数：	无
*	返回参数：	无
************************************************************
*/
void bsp_dwt_dalay_init(void)
{
	/* 使能DWT外设 */
	DEM_CR |= (uint32_t)DEM_CR_TRCENA;
	/* DWT CYCCNT寄存器计数清0 */
	DWT_CYCCNT = (uint32_t)0u;
	/* 使能Cortex-M3 DWT CYCCNT寄存器 */
	DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}

/*
************************************************************
*	函数名称：	bsp_dwt_delay_us
*	函数功能：	US延时函数
*	入口参数：	无
*	返回参数：	延时时间	
************************************************************
*/
void bsp_dwt_delay_us(volatile uint32_t us)
{
    uint32_t clk_cycle_start = (uint32_t)DWT_CYCCNT;  // 获得当前次数
	us -= 1;  //补偿值
    us *= 72; // 72MHZ时，1us的时钟脉冲为72，1us计数72次，nus则是nus *= 72

    while(((uint32_t)DWT_CYCCNT - clk_cycle_start < us));  // 当计数次数达到预期时，跳出循环
}

/*
************************************************************
*	函数名称：	bsp_dwt_delay_ms
*	函数功能：	US延时函数
*	入口参数：	无
*	返回参数：	延时时间	
************************************************************
*/
void bsp_dwt_delay_ms(volatile uint32_t ms)
{
	while(ms--){
		bsp_dwt_delay_us(1000);
	}
}


/*
************************************************************
*	函数名称：	bsp_dwt_delay_s
*	函数功能：	US延时函数
*	入口参数：	无
*	返回参数：	延时时间	
************************************************************
*/
void bsp_dwt_delay_s(volatile uint32_t s)
{
	while(s--){
		bsp_dwt_delay_ms(1000);
	}
}


