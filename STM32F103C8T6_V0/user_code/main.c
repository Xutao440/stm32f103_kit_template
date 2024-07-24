#include "bsp_cfg.h"
#include "bsp_uart.h"
/*
************************************************************
*	�������ƣ�	main
*	�������ܣ�	���������
*	��ڲ�����	��
*	���ز�����	��	
************************************************************
*/

int main(void)
{
	Stm32_Clock_Init(9);	//ϵͳʱ������
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
