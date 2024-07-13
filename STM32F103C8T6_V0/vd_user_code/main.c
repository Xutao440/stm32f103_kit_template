#include "vd_bsp_uart.h"
#include "vd_bsp_gpio.h"
#include "vd_bsp_timer.h"
#include "vd_bsp_pwm.h"
#include "vd_bsp_dwt.h"
#include "vd_bsp_adc.h"
#include "vd_bsp_systick.h"
#include "vd_bsp_soft_i2c.h"
#include "vd_bsp_soft_spi.h"

#include "log.h"
#include "vd_fml_per_led_module.h"
#include "vd_fml_per_beep_module.h"

vd_int32_t led_obj_id = -1,led_obj_id1 = -1,beep_obj_id = -1;

/*
************************************************************
*	函数名称：	vd_hardware_init
*	函数功能：	硬件初始化
*	入口参数：	无
*	返回参数：	无	
************************************************************
*/
void vd_hardware_init(void)
{	
	log_init(VD_USART1);;        //log初始化函数
	vd_bsp_dwt_dalay_init();     //dwt延时函数初始化
	vd_bsp_systick_init(1000);
	log_info("hardware init succeed!\r\n");
}



vd_void_t gpio_exti_test(vd_void_t *args)
{
	log_info("gpio_exti is succeed!\r\n");
}


void timer1_1_test(void *args)
{
	log_info("timer1_1_test\r\n");
}

void timer2_1_test(void *args)
{
	log_info("timer2_1_test\r\n");
}

//led灯初始化
void led_init(void)
{
	vd_fml_per_led_init_t led;
	vd_pwm_t led_pwm;
	
	led.flick_time_ms = 500;
	led.gpio_port = GPIO_PA1;
	led.led_gpio_init_state = VD_GPIO_HIGH;
	led.breath_speed = 5;
	
	led_pwm.gpio_port = GPIO_PA1;
	led_pwm.period = 100;
	led_pwm.pwm_duty = 50;
	led_pwm.pwm_fre_hz = 50;
	led_pwm.fre_offset = 7;
	led_pwm.gpio_default_state = VD_GPIO_LOW;
	led_obj_id = vd_fml_per_led_init(&led, &led_pwm);
	
	led.gpio_port = GPIO_PA2;
	led.led_gpio_init_state = VD_GPIO_LOW;
	led.breath_speed = 15;
	led_obj_id1 = vd_fml_per_led_init(&led, NULL);
}


//beep灯初始化
void beep_init(void)
{
	vd_pwm_t beep_pwm;
	vd_fml_per_beep_init_t  beep_init;             //beep初始化结构体
	beep_init.beep_gpio_init_state = VD_GPIO_LOW;
	beep_init.gpio_port = GPIO_PA2;
	beep_init.vd_beep_long_on_ms = 500;
	beep_init.vd_beep_short_on_ms = 200;
	beep_init.vd_beep_off_ms = 1000;
	
	beep_pwm.fre_offset = 0;
	beep_pwm.gpio_default_state = VD_GPIO_LOW;
	beep_pwm.gpio_port = GPIO_PA2;
	beep_pwm.period = 100;
	beep_pwm.pwm_duty = 50;
	beep_pwm.pwm_fre_hz = 50;
	beep_obj_id = vd_fml_per_beep_init(&beep_init, &beep_pwm);	
}
/*
************************************************************
*	函数名称：	main
*	函数功能：	主函数入库
*	入口参数：	无
*	返回参数：	无	
************************************************************
*/
#define	SLAVE_ADDR   0x25

#define test_pin     GPIO_PC13
#define test_IRQ_pin GPIO_PC13
vd_uint8_t buff[4] = {0x11, 0x12, 0x13, 0x14}, test_vlaue = 0x55;
vd_uint32_t main_timer_s = 0;

int main(void)
{
	vd_soft_spi_t  master_spi;
	vd_uint8_t data = 0;
	vd_master_i2c_t soft_i2c;	
	vd_uint32_t timer_10s = 10;
	vd_int32_t adc_mv = 0;
	vd_uint16_t adc_value = 0;
	vd_int32_t pwm_obj_id = 0, pwm_obj_id1 = 0, adc_obj_id = 0;
	TIMERx_OBJ_ID_E timer_obj1 = VD_BSP_TIMERx_OBJ_NANE,timer_obj2 = VD_BSP_TIMERx_OBJ_NANE, i2c_id = 0, i2c_id1 = 0;
	vd_pwm_t pwm;
	vd_hardware_init();            //硬件初始化		
	vd_adc_t adc;

//GPIO测试
//  vd_bsp_gpio_init(test_pin, VD_GPIO_MODE_OUT_PP); //常规GPIO初始化
//	vd_bsp_gpio_register_irq(test_IRQ_pin, VD_GPIO_MODE_IPU, VD_GPIO_IRQ_MODE_RISING_FALLING, gpio_exti_test, NULL); //gpio外部中断注册函数
//	vd_bsp_gpio_irq_switch(test_IRQ_pin, VD_GPIO_IRQ_ENABLE); //gpio外部函数使能函数
		
//	timer_obj1 = vd_bsp_timer_init(VD_PORT_TIM1, 1000, timer1_1_test, NULL);
//	vd_bsp_timer_open(timer_obj1); //开启定时器
//	timer_obj2 = vd_bsp_timer_init(VD_PORT_TIM2, 2000, timer2_1_test, NULL);
//	log_info("!!!!!!!!!!timer_obj_id=%d\r\n",timer_obj2);
//	vd_bsp_timer_open(timer_obj2); //开启定时器
	
//pwm测试	
//	pwm.timer = TIM2;
//	pwm.chnanle = pwm_channel_2;
//	pwm.gpio_port = GPIO_PA1;
//	pwm.period = 100;
//	pwm.pwm_duty = 50;
//	pwm.pwm_fre_hz = 30;
//	pwm.fre_offset = 7;
//	pwm.gpio_default_state = VD_GPIO_LOW;
//	pwm_obj_id = vd_bsp_pwm_init(&pwm);
//	
//	pwm.timer = TIM2;
//	pwm.chnanle = pwm_channel_3;
//	pwm.gpio_port = GPIO_PA2;
//	pwm.period = 100;
//	pwm.pwm_duty = 50;
//	pwm.pwm_fre_hz = 30;
//	pwm.fre_offset = 7;
//	pwm.gpio_default_state = VD_GPIO_LOW;
//	pwm_obj_id1 = vd_bsp_pwm_init(&pwm);
//	
//	vd_bsp_pwm_start(pwm_obj_id);
//	vd_bsp_pwm_start(pwm_obj_id1);
//	vd_bsp_dwt_delay_s(10);
//	vd_bsp_pwm_stop(pwm_obj_id);
//	vd_bsp_pwm_stop(pwm_obj_id1);

//	//adc 测试
//	adc.adc = ADC1;
//	adc.channel = 1;
//	adc.gpio_port= GPIO_PA1;
//	adc.ref_type = ADC_REF_OUT;
//	adc.ref_volt = ADC_REF_3300MV;
//	adc.resolution = ADC_RES_12BIT;
//	adc_obj_id = vd_bsp_adc_init(&adc);
//	adc_mv = vd_bsp_adc_read_mv(adc_obj_id);
//	adc_value = vd_bsp_adc_read_value(adc_obj_id); 		
//	log_info("!!!!!!!!!!adc_value=%d, adc_mv = %d\r\n", adc_value, adc_mv);	

	//led_init();
	//vd_fml_per_led_on(led_obj_id);
////	vd_fml_per_led_flick(led_obj_id1, NULL);
////	vd_fml_per_led_breathe(led_obj_id, NULL);
//	蜂鸣器
//	beep_init();
//	vd_fml_pre_beep_control(beep_obj_id, VD_BEEP_SHORT, 1);

//软件模拟IIC	
//	soft_i2c.reg_bit = I2C_REG_BIT_0BIT;
//	soft_i2c.scl_prot = GPIO_PB1;
//	soft_i2c.sda_prot = GPIO_PB0;
//	soft_i2c.speed = I2C_SPEED_100K;
//	vd_bsp_soft_i2c_init(&soft_i2c);

	master_spi.sck_port = GPIO_PB11;
	master_spi.mosi_port = GPIO_PB10;
	master_spi.miso_port = GPIO_PB1;
	master_spi.ss_port = GPIO_PB0;
	master_spi.spi_mode = SPI_00_MODE;
	
	vd_bsp_soft_spi_init(&master_spi);
	
//	vd_slave_spi_t slave_dev;
//	vd_int32_t spi_dev_id = 0;
//	slave_dev.ss_prot = GPIO_PA7;
//	slave_dev.spi_mode = SPI_11_MODE;
//	spi_dev_id = vd_bsp_soft_spi_add_slave_dev(&slave_dev);
	uint8_t rx_data = 0;
	while (1)
	{
		//软件SPI
		if(vd_bsp_time_differ(TICK_TYEP_S, main_timer_s) >= 1){
			main_timer_s = vd_bsp_get_now_time(TICK_TYEP_S);
			SPI_Start(0);
			rx_data = vd_bsp_spi_send_byte(test_vlaue)	;						
			log_info("soft spi write bytes rx_data=%X!\r\n",rx_data);
	
			SPI_Stop(0);			
		}
				
////		软件IIC
//		if(vd_bsp_time_differ(TICK_TYEP_S, main_timer_s) >= 1){
//			main_timer_s = vd_bsp_get_now_time(TICK_TYEP_S); 
//			if(vd_bsp_soft_i2c_write_bytes(SLAVE_ADDR, 0, buff, 4) == IIC_OK){
//				log_info("soft i2c write bytes !\r\n");
//			}		
//		}
//		if(vd_bsp_soft_i2c_write_byte(0, 0x12) == IIC_OK){	
//			log_info("soft i2c write byte !\r\n");
//		}
//		if(vd_bsp_soft_i2c_read_byte(0, &data) == IIC_OK){	
//			log_info("soft i2c read byte=%d\r\n",data);
//		}
		//vd_fml_per_beep_loop(); 
		//vd_fml_per_led_loop();
		
//常规gpio测试
//		vd_bsp_gpio_write(test_pin, VD_GPIO_HIGH);
//		vd_bsp_dwt_delay_s(1);
//		vd_bsp_gpio_write(test_pin, VD_GPIO_LOW);
//		vd_bsp_dwt_delay_s(1);
	
//定时器测试		
//		vd_bsp_dwt_delay_s(10);
//		vd_bsp_timer_close(timer_obj1); //开启定时器
//		vd_bsp_timer_close(timer_obj2); //开启定时器
//		log_info("!!!!!!!!!!\r\n");
//		vd_bsp_dwt_delay_s(5);
//		vd_bsp_timer_open(timer_obj1);  //开启定时器
//		vd_bsp_timer_open(timer_obj2);  //开启定时器		
	}
}
