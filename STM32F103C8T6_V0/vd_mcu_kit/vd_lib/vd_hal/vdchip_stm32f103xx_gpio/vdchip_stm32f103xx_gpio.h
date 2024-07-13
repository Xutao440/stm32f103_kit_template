//#ifndef _VDCHIP_STM32F103XX_GPIO_H
//#define _VDCHIP_STM32F103XX_GPIO_H

//#include "board_config.h"
//#include "base_type.h"
//#include "vd_bsp_gpio.h"

//#define GPIO_IRQn_ERRO       0xff    //错误的中断号
//#define GPIO_EXTIn_MAX       15      //GPIO外部中断最大个数

///* GPIOx Index */
//typedef enum{
//    GPIO_A,
//    GPIO_B,
//    GPIO_C,
//    GPIO_D,
//    GPIO_E,
//	
//}enum_GPIOx_t;


///* GPIOx Index */
//typedef enum{
//    VD_EXTI0 = 0,
//    VD_EXTI1,
//    VD_EXTI2,
//    VD_EXTI3,
//    VD_EXTI4,
//	VD_EXTI9_5,
//	VD_EXTI15_10,
//	
//}VD_EXTI_E;

///*******gpio information**********/
//typedef struct{
//    vd_int16_t type_gpio_num;    //自定义GPIO号码
//    GPIO_TypeDef* group;          //GPIOA...
//    vd_uint32_t   pin;            //GPIO_Pin_0...
//	
//}stm32f103_gpio_inf_t;

///*************irq map information***********/
//typedef struct{
//    vd_int16_t type_gpio_num;  //自定义GPIO号码
//    vd_uint32_t exti_line;      //中断线
//}pin_irq_map_t;

// /**
// * @brief irq callback function
// * 
// */
//typedef struct{
//	vd_int16_t     index;                  //范围0-15,数组中位置
//    vd_uint16_t    mode;                   //中断模式	
//    vd_uint32_t    line;                   //中断线
//    vd_void_t      (*hdr)(vd_void_t *args);//回调函数
//    vd_void_t      *args;                  //回调函数参数
//}vd_gpio_irq_hdr_t;


//vd_bool_t vd_stm32f103_gpio_register(vd_gpio_ops_t *ops);

//#endif

