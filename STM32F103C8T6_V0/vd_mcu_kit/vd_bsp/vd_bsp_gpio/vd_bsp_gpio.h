#ifndef _VD_BSP_GPIO_H
#define _VD_BSP_GPIO_H

#include "base_type.h"
#include "vd_bsp_type.h"

#define GPIO_EXTIn_MAX       15
#define GPIO_IRQn_ERRO       0xff    //错误的中断号

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


//引脚状态
typedef enum{
	VD_GPIO_LOW,
	VD_GPIO_HIGH,
	VD_GPIO_STAT_MAX,
}GPIO_STATE_E;

//引脚模式
typedef enum{
	VD_GPIO_MODE_AIN = 0x00,            //模拟输入
	VD_GPIO_MODE_IN_FLOATING = 0x04,    //浮空输入
	VD_GPIO_MODE_IPD = 0x28,            //输入下拉
	VD_GPIO_MODE_IPU = 0x48,           //输入上拉
	VD_GPIO_MODE_OUT_OD = 0x14,         //开漏输出
	VD_GPIO_MODE_OUT_PP = 0x10,         //推挽输出
	VD_GPIO_MODE_AF_OD = 0x1C,         //复用开漏输出
	VD_GPIO_MODE_AF_PP = 0x18,         //复用推挽输出
	VD_GPIO_MODE_MAX = 0xff,
	
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
vd_int32_t vd_bsp_gpio_init(GPIO_PORT_E gpio_port, GPIO_MODE_E mode); //初始化函数

vd_int32_t vd_bsp_gpio_write( GPIO_PORT_E gpio_port, GPIO_STATE_E value); //gpio写函数

vd_int32_t vd_bsp_gpio_read(GPIO_PORT_E gpio_port); //gpio读函数

vd_int32_t vd_bsp_gpio_register_irq(GPIO_PORT_E gpio_port, GPIO_MODE_E gpio_mode, GPIO_IRQ_MODE_E mode, vd_void_t (*hdr)(vd_void_t *args), vd_void_t  *args); //gpio外部中断注册函数

vd_int32_t vd_bsp_gpio_unregister_irq(GPIO_PORT_E gpio_port); //gpio外部中断取消注册函数

vd_int32_t vd_bsp_gpio_irq_switch(GPIO_PORT_E gpio_port, GPIO_IRQ_STATE_E state); //gpio外部函数使能函数

uint32_t vd_bsp_gpio_get_PeriphClock(GPIO_TypeDef* gpio); //获取GPIO时钟

vd_int32_t vd_bsp_get_group_and_pin(GPIO_PORT_E gpio_port, uint32_t* group_addr, uint16_t* pin);  //获取group 和 pin
#endif
