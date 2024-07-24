#include "bsp_cfg.h"
#include "bsp_uart.h"
/*
************************************************************
*	函数名称：	main
*	函数功能：	主函数入库
*	入口参数：	无
*	返回参数：	无	
************************************************************
*/

int main(void)
{
	Stm32_Clock_Init(9);	//系统时钟设置
	bsp_dwt_dalay_init();
	vd_bsp_gpio_init(GPIO_PB15, VD_GPIO_OUT_PP); 	
	//log_init(VD_USART1);
	vd_bsp_usart_test_init(VD_USART1);

	while (1)
	{	
	vd_bsp_gpio_write(GPIO_PB15,vd_false);
	bsp_dwt_delay_s(1);
	vd_bsp_gpio_write(GPIO_PB15,vd_true);
	bsp_dwt_delay_s(1);
		
	vd_bsp_usart_test();
	}
}
