#include "bsp_gpio.h"
#include "base_check.h"
#include "base_utils.h"

/*
************************************************************
*	函数名称：	vd_bsp_gpio_mode_cfg
*	函数功能：	gpio mode配置函数
*	入口参数：	GPIOx：group地址，  pin：引脚号，mode：模式
*	返回参数：	无
************************************************************
*/
static void vd_bsp_gpio_mode_cfg(GPIO_TypeDef* GPIOx, uint16_t pin, GPIO_MODE_E mode)
{
	vd_uint8_t GPIOx_cnfy = 0, GPIOx_modey = 0;
	vd_uint32_t GPIOx_cr = 0;
	vd_uint32_t temp = 0;

	GPIOx_modey = (vd_uint8_t)mode & 0x0f;
	GPIOx_cnfy = (vd_uint8_t)mode >> 4;
	
	if(GPIOx_modey == 0){ //输入
		
		if(GPIOx_cnfy == 3){//上拉
			GPIOx->BSRR = (vd_uint32_t)(0x0001 << pin);
			GPIOx_cnfy = 2;
		}else if(GPIOx_cnfy == 2){ //下拉
			GPIOx->BRR = (vd_uint32_t)(0x0001 << pin);
		}	
	}
	GPIOx_cr |= GPIOx_modey;
	GPIOx_cr |= GPIOx_cnfy << 2;
	
	if(pin <= 7){
	  GPIOx->CRL &= ~(0x0f << (pin * 4));
	  GPIOx->CRL |= GPIOx_cr << (pin * 4);
		
	}else if(pin <= 15){
	  GPIOx->CRH &= ~(0x0f << ((pin - 8) * 4));	
	  GPIOx->CRH |= GPIOx_cr << ((pin - 8) * 4);
	}
}

/*
************************************************************
*	函数名称：	vd_bsp_gpio_init
*	函数功能：	gpio初始化
*	入口参数：	gpio_port：port， mode：引脚模式
*	返回参数：	无
************************************************************
*/
vd_int32_t vd_bsp_gpio_init(GPIO_PORT_E gpio_port, GPIO_MODE_E mode)
{		
	vd_int32_t ret_value = 0;
	uint8_t group = 0, pin = 0;
	
	vd_check_return_val(gpio_port > GPIO_MAX, -1);

	group = gpio_port >> 4;
	pin = gpio_port & 0x0f;
	
	switch(group)
	{
		case 0: //GPIOA
			//时钟
			RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
			vd_bsp_gpio_mode_cfg(GPIOA, pin, mode);
			break;
		
		case 1: //GPIOB
			//时钟
			RCC->APB2ENR |= RCC_APB2Periph_GPIOB;
			vd_bsp_gpio_mode_cfg(GPIOB, pin, mode);
			break;
		
		case 2: //GPIOC
			//时钟
			RCC->APB2ENR |= RCC_APB2Periph_GPIOC;
			vd_bsp_gpio_mode_cfg(GPIOC, pin, mode);
			break;
		
		case 3: //GPIOD
			//时钟
			RCC->APB2ENR |= RCC_APB2Periph_GPIOD;
			vd_bsp_gpio_mode_cfg(GPIOD, pin, mode);
			break;
		
		default:
			break;
	}
	
    return ret_value;
}


/*
************************************************************
*	函数名称：	vd_bsp_gpio_write
*	函数功能：	gpio写
*	入口参数：	gpio_port：port， value：设置值
*	返回参数：	成功返回0
************************************************************
*/
vd_int32_t vd_bsp_gpio_write( GPIO_PORT_E gpio_port, vd_bool_t value)
{   
	uint8_t group = 0, pin = 0;
	vd_int32_t ret_value = 0;

	vd_check_return_val(gpio_port > GPIO_MAX, -1);

	group = gpio_port >> 4;
	pin = gpio_port & 0x0f;
	switch(group)
	{
		case 0: //GPIOA
			if(value == vd_true){
				GPIOA->BSRR |= 1U << pin;
			}else if(value == vd_false){
				GPIOA->BRR |= (1U << pin);
			}
			break;
		
		case 1: //GPIOB
			if(value == vd_true){
				GPIOB->BSRR |= 1U << pin;
			}else if(value == vd_false){
				GPIOB->BRR |= (1U << pin);
			}
			break;
		
		case 2: //GPIOC
			if(value == vd_true){
				GPIOC->BSRR |= 1U << pin;
			}else if(value == vd_false){
				GPIOC->BRR |= (1U << pin);
			}
			break;
		
		case 3: //GPIOD
			if(value == vd_true){
				GPIOD->BSRR |= 1U << pin;
			}else if(value == vd_false){
				GPIOD->BRR |= (1U << pin);
			}
			break;
		
		default:
			break;
	}

    return ret_value;
}


/*
************************************************************
*	函数名称：	vd_bsp_gpio_read
*	函数功能：	gpio读
*	入口参数：	gpio_port：port
*	返回参数：	成功返回0
************************************************************
*/
vd_int32_t vd_bsp_gpio_read(GPIO_PORT_E gpio_port)
{  
	uint8_t group = 0, pin = 0;
	vd_int32_t ret_value = 0;

	vd_check_return_val(gpio_port > GPIO_MAX, -1);

	group = gpio_port >> 4;
	pin = gpio_port & 0x0f;
	switch(group)
	{
		case 0: //GPIOA
			if(GPIOA->ODR & (1U << pin)){
				ret_value = 1;
			}
			break;
		
		case 1: //GPIOB
			if(GPIOB->ODR & (1U << pin)){
				ret_value = 1;
			}
			break;
		
		case 2: //GPIOC
			if(GPIOC->ODR & (1U << pin)){
				ret_value = 1;
			}
			break;
		
		case 3: //GPIOD
			if(GPIOD->ODR & (1U << pin)){
				ret_value = 1;
			}
			break;
		
		default:
			break;
	}

    return ret_value;
}



