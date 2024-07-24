#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "base_type.h"

#define GPIO_EXTIn_MAX       15
#define GPIO_IRQn_ERRO       0xff    //������жϺ�

#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC             ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD             ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE             ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF             ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG             ((uint32_t)0x00000100)

//����port
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

/*����ģʽ����4λ������/�������4λ��ģʽ*/
typedef enum{
	VD_GPIO_AIN         = 0x00,  //ģ������
	VD_GPIO_IN_FLOATING = 0x10,  //��������
	VD_GPIO_IPU         = 0x30,  //��������
	VD_GPIO_IPD         = 0x20,  //��������
	VD_GPIO_OUT_OD      = 0x13,  //��©���
	VD_GPIO_OUT_PP      = 0x03,  //�������
	VD_GPIO_AF_OD       = 0x33,  //���ÿ�©���
	VD_GPIO_AF_PP       = 0x23,  //�����������
	
}GPIO_MODE_E;

//�ж�ģʽ
typedef enum{
	VD_GPIO_IRQ_MODE_RISING,        // �����ش���
	VD_GPIO_IRQ_MODE_FALLING,        //�½��ش���
	VD_GPIO_IRQ_MODE_RISING_FALLING,  //�������½���ͬʱ����
//	VD_GPIO_IRQ_MODE_HIGH_LEVEL,
//	VD_GPIO_IRQ_MODE_LOW_LEVEL,
	VD_GPIO_IRQ_MODE_MAX,
	
}GPIO_IRQ_MODE_E;

//�ж�ʹ��
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
	vd_int16_t     index;                  //��Χ0-15,������λ��
	vd_int8_t      gpio_mode;              //gpioģʽ              
    vd_uint16_t    mode;                   //�ж�ģʽ	
    vd_uint32_t    line;                   //�ж���
    vd_void_t      (*hdr)(vd_void_t *args);//�ص�����
    vd_void_t      *args;                  //�ص���������
}vd_gpio_irq_hdr_t;


/********************************************* �������� ******************************************************/
vd_int32_t vd_bsp_gpio_init(GPIO_PORT_E gpio_port, GPIO_MODE_E mode);     //��ʼ������
vd_int32_t vd_bsp_gpio_write( GPIO_PORT_E gpio_port, vd_bool_t value); //gpioд����
vd_int32_t vd_bsp_gpio_read(GPIO_PORT_E gpio_port);                       //gpio������

#endif
