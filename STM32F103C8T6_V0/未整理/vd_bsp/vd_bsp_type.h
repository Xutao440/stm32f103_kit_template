#ifndef __VD_BSP_TYPE_H
#define __VD_BSP_TYPE_H

/* ======================================================================================
 * 一、外设/协议端口定义
 * GPIO引脚编号范围：0~0xff
 */
typedef enum{
	VD_PORT_GPIO_NONE = -1,
	VD_PORT_GPIO_MAX = 0x00ff,	/* GPIO pin值限定 */
	VD_PORT_UART0,
	VD_PORT_UART1,
	VD_PORT_UART2,
	VD_PORT_UART3,
	VD_PORT_UART4,
	VD_PORT_UART5,
	VD_PORT_UART_MAX,
	VD_PORT_I2C0,
	VD_PORT_I2C1,
	VD_PORT_I2C2,
	VD_PORT_I2C_MAX,
	VD_PORT_SPIM0,
	VD_PORT_SPIM1,
	VD_PORT_SPIM2,
	VD_PORT_SPIM_MAX,
	VD_PORT_SPIS0,
	VD_PORT_SPIS1,
	VD_PORT_SPIS2,
	VD_PORT_SPIS_MAX,
	VD_PORT_QSPIM0,
	VD_PORT_QSPIM1,
	VD_PORT_QSPIM2,
	VD_PORT_QSPIM_MAX,
	VD_PORT_QSPIS0,
	VD_PORT_QSPIS1,
	VD_PORT_QSPIS2,
	VD_PORT_QSPIS_MAX,
	VD_PORT_TIM0,
	VD_PORT_TIM1,
	VD_PORT_TIM2,
	VD_PORT_TIM_MAX,
	VD_PORT_ADC0,
	VD_PORT_ADC1,
	VD_PORT_ADC2,
	VD_PORT_ADC3,
	VD_PORT_ADC4,
	VD_PORT_ADC5,
	VD_PORT_ADC6,
	VD_PORT_ADC7,
	VD_PORT_ADC8,
	VD_PORT_ADC_MAX,
	VD_PORT_PWM0,
	VD_PORT_PWM1,
	VD_PORT_PWM2,
	VD_PORT_PWM3,
	VD_PORT_PWM4,
	VD_PORT_PWM5,
	VD_PORT_PWM6,
	VD_PORT_PWM7,
	VD_PORT_PWM8,
	VD_PORT_PWM_MAX,
	VD_PORT_ISDN,
	VD_PORT_BLE,
}vd_bsp_port_e;

/* ======================================================================================
 * 二、错误码
 */
typedef enum{
	VD_BSP_NOERROR = 0,						//没有错误
	VD_BSP_ERR_REGISTER,					//平台注册失败
	VD_BSP_ERR_PORT_INVALID,			//无效端口号
	VD_BSP_ERR_PORT_INACTIVE,			//端口未激活
	VD_BSP_ERR_PARAMETER,					//参数错误
	VD_BSP_ERR_MALLOC,						//内存分配错误
	VD_BSP_ERR_SDK,								//SDK执行错误
	VD_BSP_ERR_OVER_SIZE,					//大小溢出
}vd_bsp_error_e;

#endif
