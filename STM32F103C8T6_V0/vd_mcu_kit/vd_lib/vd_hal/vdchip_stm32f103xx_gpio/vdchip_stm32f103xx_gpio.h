//#ifndef _VDCHIP_STM32F103XX_GPIO_H
//#define _VDCHIP_STM32F103XX_GPIO_H

//#include "board_config.h"
//#include "base_type.h"
//#include "vd_bsp_gpio.h"

//#define GPIO_IRQn_ERRO       0xff    //������жϺ�
//#define GPIO_EXTIn_MAX       15      //GPIO�ⲿ�ж�������

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
//    vd_int16_t type_gpio_num;    //�Զ���GPIO����
//    GPIO_TypeDef* group;          //GPIOA...
//    vd_uint32_t   pin;            //GPIO_Pin_0...
//	
//}stm32f103_gpio_inf_t;

///*************irq map information***********/
//typedef struct{
//    vd_int16_t type_gpio_num;  //�Զ���GPIO����
//    vd_uint32_t exti_line;      //�ж���
//}pin_irq_map_t;

// /**
// * @brief irq callback function
// * 
// */
//typedef struct{
//	vd_int16_t     index;                  //��Χ0-15,������λ��
//    vd_uint16_t    mode;                   //�ж�ģʽ	
//    vd_uint32_t    line;                   //�ж���
//    vd_void_t      (*hdr)(vd_void_t *args);//�ص�����
//    vd_void_t      *args;                  //�ص���������
//}vd_gpio_irq_hdr_t;


//vd_bool_t vd_stm32f103_gpio_register(vd_gpio_ops_t *ops);

//#endif

