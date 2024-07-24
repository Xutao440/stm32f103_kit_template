#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "base_type.h"

#define GPIO_EXTIn_MAX       15
#define GPIO_IRQn_ERRO       0xff    //错误的中断号

#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC             ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD             ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE             ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF             ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG             ((uint32_t)0x00000100)

//引脚port
typedef enum{
	
	GPIO_PA0 = 0x00,
	GPIO_PA1,
	GPIO_PA2,
	GPIO_PA3,
	GPIO_PA4,
	GPIO_PA5,
	GPIO_PA6,
	GPIO_PA7,
	GPIO_PA8,
	GPIO_PA9,
	GPIO_PA10,
	GPIO_PA11,
	GPIO_PA12,
	GPIO_PA13,
	GPIO_PA14,
	GPIO_PA15,
	GPIO_PB0 = 0x10,
	GPIO_PB1,
	GPIO_PB2,
	GPIO_PB3,
	GPIO_PB4,
	GPIO_PB5,
	GPIO_PB6,
	GPIO_PB7,
	GPIO_PB8,
	GPIO_PB9,
	GPIO_PB10,
	GPIO_PB11,
	GPIO_PB12,
	GPIO_PB13,
	GPIO_PB14,
	GPIO_PB15,
	GPIO_PC0 = 0x20,
	GPIO_PC1,
	GPIO_PC2,
	GPIO_PC3,
	GPIO_PC4,
	GPIO_PC5,
	GPIO_PC6,
	GPIO_PC7,
	GPIO_PC8,
	GPIO_PC9,
	GPIO_PC10,
	GPIO_PC11,
	GPIO_PC12,
	GPIO_PC13,
	GPIO_PC14,
	GPIO_PC15,	
	GPIO_MAX,
	
}GPIO_PORT_E;

/*引脚模式，低4位：输入/输出，高4位：模式*/
typedef enum{
	VD_GPIO_AIN         = 0x00,  //模拟输入
	VD_GPIO_IN_FLOATING = 0x10,  //浮空输入
	VD_GPIO_IPU         = 0x30,  //输入上拉
	VD_GPIO_IPD         = 0x20,  //输入下拉
	VD_GPIO_OUT_OD      = 0x13,  //开漏输出
	VD_GPIO_OUT_PP      = 0x03,  //推挽输出
	VD_GPIO_AF_OD       = 0x33,  //复用开漏输出
	VD_GPIO_AF_PP       = 0x23,  //复用推挽输出
	
}GPIO_MODE_E;

//中断模式
typedef enum{
	VD_GPIO_IRQ_MODE_RISING,        // 上升沿触发
	VD_GPIO_IRQ_MODE_FALLING,        //下降沿触发
	VD_GPIO_IRQ_MODE_RISING_FALLING,  //上升和下降沿同时触发
//	VD_GPIO_IRQ_MODE_HIGH_LEVEL,
//	VD_GPIO_IRQ_MODE_LOW_LEVEL,
	VD_GPIO_IRQ_MODE_MAX,
	
}GPIO_IRQ_MODE_E;

//中断使能
typedef enum{
	VD_GPIO_IRQ_DISABLE,
	VD_GPIO_IRQ_ENABLE,
	VD_GPIO_IRQ_ENABLE_MAX,
}GPIO_IRQ_STATE_E;

/* GPIOx Index */
typedef enum{
    VD_EXTI0 = 0,
    VD_EXTI1,
    VD_EXTI2,
    VD_EXTI3,
    VD_EXTI4,
	VD_EXTI9_5,
	VD_EXTI15_10,
	
}VD_EXTI_E;

typedef struct{
	vd_int16_t     index;                  //范围0-15,数组中位置
	vd_int8_t      gpio_mode;              //gpio模式              
    vd_uint16_t    mode;                   //中断模式	
    vd_uint32_t    line;                   //中断线
    vd_void_t      (*hdr)(vd_void_t *args);//回调函数
    vd_void_t      *args;                  //回调函数参数
}vd_gpio_irq_hdr_t;


/********************************************* 函数声明 ******************************************************/
vd_int32_t vd_bsp_gpio_init(GPIO_PORT_E gpio_port, GPIO_MODE_E mode);     //初始化函数
vd_int32_t vd_bsp_gpio_write( GPIO_PORT_E gpio_port, vd_bool_t value); //gpio写函数
vd_int32_t vd_bsp_gpio_read(GPIO_PORT_E gpio_port);                       //gpio读函数

#endif
