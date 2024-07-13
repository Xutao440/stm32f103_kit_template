#include "drv_MR88FX02_BSP.h"
#include "kit_driver_bsp.h"
//#include "driver_config.h"

#if (DRIVER_FML_EN==ON)
extern vd_void_t kit_hrd_drivers_io_init(vd_void_t);

extern vd_void_t kit_hrd_init( vd_void_t );

#endif



#if (PROJECT_OPEN_SYS_QUEUE==ON)
mq_check_s sys_event_mq;
vd_void_t sys_queue_init(vd_void_t){
	sys_event_mq.init_flag = 0;
	sys_event_mq.init_msg_num = 4;
	sys_event_mq.init_msg_size = 15;
	sys_event_mq.mqd_msg = mq_queue_init(sys_event_mq);//4条消息，每条消息15个字节
}
#endif


vd_void_t kit_drv_hrd_init( vd_void_t )//底层驱动初始化
{
#if (DRIVER_BSP_GPIO_EN == ON)
	drv_gpio_t drv_gpio;
	memset(&drv_gpio, 0, sizeof(drv_gpio_t));
	drv_gpio.gpio_init_type = DRV_GPIO_INIT_ALL;//初始化所有GPIO
#endif
	drv_start_init();//关闭中断
	drv_bsp_init();//系统初始化，无需修改，但是如果有新的中断使用，需要添加中断初始化
	
	/*********所有GPIO初始化**********/
	#if (DRIVER_BSP_GPIO_EN == ON)
	drv_gpio_init(&drv_gpio);
	#if PROJECT_NOW == PROJECT_IS_PR011
	drv_gpio_write(DRV_GPIO_C,7,DRV_GPIO_OUTPUT_HIGH);
	#endif
	//模块对应IO初始化
	#if (DRIVER_FML_EN == ON)
	kit_hrd_drivers_io_init();
	#endif
	#endif
	
	/*********UART**********/
	/********log*UART**********/
	#if (DRIVER_BSP_UART_EN == ON)
	#if (DRIVER_BSP_UART0_EN==ON)
	drv_uart_init(DRIVER_BSP_UART0);
	#endif
	#if (DRIVER_BSP_UART1_EN==ON)
	drv_uart_init(DRIVER_BSP_UART1);
	#endif
	#if (DRIVER_BSP_UART2_EN==ON)
	drv_uart_init(DRIVER_BSP_UART2);
	#if PROJECT_NOW == PROJECT_IS_VC001
	drv_uart_control_init();
	drv_uart_rx_receive_en(DRIVER_BSP_UART2);//默认接收模式
	#endif
	#endif
	#endif
	/*********ADC**********/
	#if (DRIVER_BSP_ADC_EN == ON)
	#if PROJECT_NOW == PROJECT_IS_PR011
	drv_adc_init(7);
	//drv_adc_init(19);
	#elif PROJECT_NOW == PROJECT_IS_VC001
	drv_adc_init(16);
//	drv_adc_init(18);
//	drv_adc_init(19);
//	drv_adc_init(25);
	#elif PROJECT_NOW == PROJECT_IS_PR018
	drv_adc_init(25);
	#endif
	#endif
	/*********timer**********/

	#if (DRIVER_BSP_TIMER_GTIMER0_EN==ON)
	drv_timer_init(DRIVER_BSP_TIMER_GTIMER0);
	#endif
	#if (DRIVER_BSP_TIMER_GTIMER1_EN==ON)
	drv_timer_init(DRIVER_BSP_TIMER_GTIMER1);
	#endif
	#if (DRIVER_BSP_TIMER_GTIMER2_EN==ON)
	drv_timer_init(DRIVER_BSP_TIMER_GTIMER2);
	#endif
	/*********PWM**********/
	#if (DRIVER_BSP_PWM_EN == ON)
	#if (DRIVER_BSP_PWM_BSTIMER0_EN==ON)
	drv_pwm_init(DRIVER_BSP_PWM_BEEP0);
	//drv_pwm_write_duty(DRIVER_BSP_PWM_BEEP0,40);
	#endif
	#if (DRIVER_BSP_PWM_BSTIMER1_EN==ON)
	drv_pwm_init(DRIVER_BSP_PWM_BEEP1);
//	drv_pwm_write_duty(DRIVER_BSP_PWM_BEEP1,40);
	#endif
	#endif

	/*********WDT**********/
	#if (DRIVER_FML_WDT_EN == ON)
	drv_watchdog_init(DRV_WDT_4S);
	#endif
	/*********SPIM**********/
	#if (DRIVER_BSP_SPIM_EN == ON)
	drv_spim_init(0);
	
	#endif
	drv_exti_init();//IO口中断触发配置
	//mcu_log("drv_uart_rx_receive_en4\r\n");
	drv_init_end();//打开中断
	/********sys moudle init*******/
	#if (PROJECT_OPEN_SYS_QUEUE==ON)
	sys_queue_init();//系统通信队列初始化
	#endif
	/*********Module INIT**********/
	#if (DRIVER_FML_EN==ON)
	kit_hrd_init();
	#endif
}







