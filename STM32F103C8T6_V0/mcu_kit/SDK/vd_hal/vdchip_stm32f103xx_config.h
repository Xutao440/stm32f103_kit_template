/*
 * Project: dt_Project
 * Module: ƽ̨������
 * File: platform_config.h
 * Created Date: 2022-03-22 16:25:07
 * Author: meizhen
 * Description: ƽ̨��ص�����
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _VDCHIP4005_CONFIG_TEST_DEMO_H
#define _VDCHIP4005_CONFIG_TEST_DEMO_H

/* ======================================================================================
 * includes
 */
#include "base.h"
/* ======================================================================================
 * extern
 */
//__vd_extern_c_enter__;
/* ======================================================================================
 * types
 */

/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * һ��ƽ̨���������������
 */

// 1.1 GPIO����
#define PLATFORM_GPIO_EN 						ON

// 1.2 Uart ����
#define PLATFORM_UART0_EN 					ON
#define PLATFORM_UART0_TX 					1 // ���㷽�� PORTx*8+ PIN������PA0=0*0+0���������Ÿ��ù��ܲ��
#define PLATFORM_UART0_RX 					0
#define PLATFORM_UART0_BAUDEATE 		UART_BAUD_115200
#define PLATFORM_UART0_MODE 				UART_MODE_TXRX
#define PLATFORM_UART0_DATA_BITS 		UART_DATA_BITS_8
#define PLATFORM_UART0_STOP_BITS 		UART_STOP_BITS_1
#define PLATFORM_UART0_PARITY 			UART_PARITY_BIT_NONE

#define PLATFORM_UART1_EN 					ON
#define PLATFORM_UART1_TX 					27 // PD3
#define PLATFORM_UART1_RX 					26 // PD2
#define PLATFORM_UART1_BAUDEATE 		UART_BAUD_115200
#define PLATFORM_UART1_MODE 				UART_MODE_TXRX
#define PLATFORM_UART1_DATA_BITS 		UART_DATA_BITS_8
#define PLATFORM_UART1_STOP_BITS 		UART_STOP_BITS_1
#define PLATFORM_UART1_PARITY 			UART_PARITY_BIT_NONE

#define PLATFORM_UART2_EN OFF
#define PLATFORM_UART2_TX -1
#define PLATFORM_UART2_RX -1
#define PLATFORM_UART2_BAUDEATE 115200

// 1.3 I2C ����
#define PLATFORM_I2C1_EN 					ON
#define PLATFORM_I2C1_SCL 				2	//PA2
#define PLATFORM_I2C1_SDA 				3	//PA3
#define PLATFORM_I2C1_MODE				0	//������7λ��ַ
#define PLATFORM_I2C1_SLAVEADDR		0	//�ӻ���ַ���ӻ�ģʽ����Ч��

//#define PLATFORM_I2C1_EN OFF
//#define PLATFORM_I2C1_SCL -1
//#define PLATFORM_I2C1_SDA -1
//#define PLATFORM_I2C1_SPEED DRV_I2C_SPEED_STAND           // 0,100khz;1,400khz
//#define PLATFORM_I2C1_ARRD_BIT DRV_I2C_SLAVE_ADDRESS_7BIT // 0,7bit;1,10bit

// 1.4 SPIM ����
#define PLATFORM_SPIM0_EN ON
#define PLATFORM_SPIM0_SCLK_PIN 8     // PB0         	SPI0ʱ�ӽ�
#define PLATFORM_SPIM0_CS_PIN 	9     // PB1					SPI0Ƭѡ��
#define PLATFORM_SPIM0_MOSI_PIN 10     // PB2		SPI0 MOSI��/QSPI IO0��
#define PLATFORM_SPIM0_MISO_PIN 11     // PB3		SPI0 MISO��/QSPI IO1��
#define PLATFORM_SPIM0_MODE			0			// ģʽ0  ���е�ƽ�͵�ƽ����һ�����ز���
#define PLATFORM_SPIM0_CS_MODE	1			// Ӳ��Ƭѡ
#define PLATFORM_SPIM0_DATA_BIT 8     // ����λ
#define PLATFORM_SPIM0_SPEED 		4800000 		// SPEED ���Ϊ48MHz
#define PLATFORM_SPIM0_DMA_EN 	OFF 	// DMAʹ��
#define PLATFORM_SPIM0_DMA_CHANNEL 		0 		// DMAͨ��

#define PLATFORM_SPIM1_EN OFF
#define PLATFORM_SPIM1_SCLK_PIN 24    // SPI1ʱ�ӽ�  //3*8+0=PD0
#define PLATFORM_SPIM1_CS_PIN 25      // SPI1Ƭѡ��
#define PLATFORM_SPIM1_CMD_PIN 19     // SPI1��������ѡ���
#define PLATFORM_SPIM1_IO0_PIN 18     // SPI1 MOSI��/QSPI IO0��
#define PLATFORM_SPIM1_IO1_PIN -1     // SPI1 MISO��/QSPI IO1��
#define PLATFORM_SPIM1_IO2_PIN -1     // QSPI IO2��
#define PLATFORM_SPIM1_IO3_PIN -1     // QSPI IO3��
#define PLATFORM_SPIM1_SPEED 48000000 // SPEED ���Ϊ48MHz

// 1.5 SPIS����  // maximum frequency is 2MHz
#define PLATFORM_SPIS_EN OFF
#define PLATFORM_SPIS_CS_PIN 3

// 1.6 QSPI ����
#define PLATFORM_QSPIM0_EN 			ON
#define PLATFORM_QSPIM0_SCLK_PIN 8     	// PB0    QSPIM0ʱ�ӽ�
#define PLATFORM_QSPIM0_CS_PIN 	9     	// PB1		QSPIM0Ƭѡ��
#define PLATFORM_QSPIM0_IO0_PIN 10     	// PB2		QSPIM0 IO0��
#define PLATFORM_QSPIM0_IO1_PIN 11     	// PB3		QSPIM0 IO1��
#define PLATFORM_QSPIM0_IO2_PIN 12     	// PB4		QSPIM0 IO2��
#define PLATFORM_QSPIM0_IO3_PIN 13     	// PB5		QSPIM0 IO3��
#define PLATFORM_QSPIM0_MODE			0				// ģʽ0  ���е�ƽ�͵�ƽ����һ�����ز���
#define PLATFORM_QSPIM0_CS_MODE		0				// ���Ƭѡ
#define PLATFORM_QSPIM0_DATA_BIT 	8     	// ����λ
#define PLATFORM_QSPIM0_SPEED 		48000000 		// SPEED ���Ϊ48MHz
#define PLATFORM_QSPIM0_DMA_EN 	ON 		// DMAʹ��
#define PLATFORM_QSPIM0_DMA_CHANNEL 		0 		// DMAͨ��

// 1.7 PWM����
#define PLATFORM_PWM0_EN 						OFF
#define PLATFORM_PWM0_MODULE        0
#define PLATFORM_PWM0_PIN 					28
#define PLATFORM_PWM0_CHANNEL       0

#define PLATFORM_PWM1_EN 						OFF
#define PLATFORM_PWM1_MODULE        0
#define PLATFORM_PWM1_PIN 					29
#define PLATFORM_PWM1_CHANNEL       1

#define PLATFORM_PWM2_EN 						OFF
#define PLATFORM_PWM2_MODULE        0
#define PLATFORM_PWM2_PIN 					29
#define PLATFORM_PWM2_CHANNEL       2

#define PLATFORM_PWM3_EN 						OFF
#define PLATFORM_PWM3_MODULE        0
#define PLATFORM_PWM3_PIN 					5
#define PLATFORM_PWM3_CHANNEL       3

#define PLATFORM_PWM4_EN 						ON
#define PLATFORM_PWM4_MODULE        0
#define PLATFORM_PWM4_PIN 					28
#define PLATFORM_PWM4_CHANNEL       4

#define PLATFORM_PWM5_EN 						ON
#define PLATFORM_PWM5_MODULE        0
#define PLATFORM_PWM5_PIN 					29
#define PLATFORM_PWM5_CHANNEL       5

#define PLATFORM_PWM6_EN 						OFF
#define PLATFORM_PWM6_MODULE        0
#define PLATFORM_PWM6_PIN 					30
#define PLATFORM_PWM6_CHANNEL       6

#define PLATFORM_PWM7_EN 						OFF
#define PLATFORM_PWM7_MODULE        0
#define PLATFORM_PWM7_PIN 					4
#define PLATFORM_PWM7_CHANNEL       7

// 1.8 ADC����
#define PLATFORM_ADC0_EN            ON
#define PLATFORM_ADC0_MODULE        0
#define PLATFORM_ADC0_PIN           25
#define PLATFORM_ADC0_CHANNEL       1
#define PLATFORM_ADC0_REF           PLATFORM_ADC_REF_IN
#define PLATFORM_ADC0_REF_VOLT      PLATFORM_ADC_REF_3300MV
#define PLATFORM_ADC0_RES_BIT       PLATFORM_ADC_RESOLUTION_10BIT
#define PLATFORM_ADC0_DMA_EN        OFF

#define PLATFORM_ADC1_EN            OFF
#define PLATFORM_ADC1_MODULE        0
#define PLATFORM_ADC1_PIN           25
#define PLATFORM_ADC1_CHANNEL       1
#define PLATFORM_ADC1_REF           PLATFORM_ADC_REF_IN
#define PLATFORM_ADC1_REF_VOLT      PLATFORM_ADC_REF_3300MV
#define PLATFORM_ADC1_RES_BIT       PLATFORM_ADC_RESOLUTION_10BIT
#define PLATFORM_ADC1_DMA_EN        OFF

// 1.9 TIMER����
#define PLATFORM_TIME0_EN 				ON
#define PLATFORM_TIME0_PERIOD 			100		//100us
#define PLATFORM_TIME0_AUTOLOAD 		ON		//�Զ���װ��

#define PLATFORM_TIME1_EN 				ON
#define PLATFORM_TIME1_PERIOD 			1000// 1ms
#define PLATFORM_TIME1_AUTOLOAD 		ON		//�Զ���װ��

//1.10 DMA����
//��ʽ��{DMA ID��@ref vd_fr8000_dma_id_e���� ͨ���ţ�0~2���� ����ID��1~15��}
#define PLATFORM_DMA_INIT_TABLE {\
	{DMA_ID_SPIM0_TX, 0, 1, 32, 8},\
}

// 1.10 DEBUG����
#define PLATFORM_DEBUG_EN OFF

// 1.11 SYSTICK����
#define PLATFORM_SYSTICK_EN OFF

// 1.12 FLASH����
#define PLATFORM_FLASH_EN ON // Flash�����������������ö�ٶ�������

// 1.13 WATCH_DOG����
#define PLATFORM_WATCH_DOG_EN OFF

// 1.14 NTC����
#define PLATFORM_NTC_EN ON

// 1.16 �͹��Ĺ���
#define PLATFORM_PM_EN	ON

// 1.17 ȫ��int����
#define PLATFORM_GLOBAL_SWITCH_EN	ON

/* ======================================================================================
 * ����ƽ̨�����ӿ��������
 */
// 2.1 GPIO
#if (PLATFORM_GPIO_EN == ON)
	#include "vdchip40xx_50xx_gpio/vdchip40xx_50xx_gpio.h"
	#define PLATFORM_GPIO_REGISTER	vd_fr8000_gpio_register
#endif

// 2.2 UART
#if (PLATFORM_UART0_EN == ON || PLATFORM_UART1_EN == ON || PLATFORM_UART2_EN == ON)
	#include "vdchip40xx_50xx_uart/vdchip40xx_50xx_uart.h"
	#define PLATFORM_UART_REGISTER	vd_fr8000_uart_register
#endif

// 2.3 SPI
#if (PLATFORM_SPIM0_EN == ON || PLATFORM_SPIM1_EN == ON || PLATFORM_SPIS_EN == ON)
	#include "vdchip40xx_50xx_spi/vdchip40xx_50xx_spi.h"
	#define PLATFORM_SPI_REGISTER	vdchip_40xx_50xx_spi_register
#endif

// 2.4 QSPI
#if (PLATFORM_QSPIM0_EN == ON)
	#include "vdchip40xx_50xx_qspi/vdchip40xx_50xx_qspi.h"
	#define PLATFORM_QSPI_REGISTER	vdchip_40xx_50xx_qspi_register
#endif

// 2.5 I2C
#if (PLATFORM_I2C0_EN == ON || PLATFORM_I2C1_EN == ON | PLATFORM_I2C2_EN == ON)
	#include "vdchip40xx_50xx_i2c/vdchip40xx_50xx_i2c.h"
	#define PLATFORM_I2C_REGISTER	vdchip_40xx_50xx_i2c_register
#endif

// 2.6 PWM
#if (PLATFORM_PWM0_EN == ON || PLATFORM_PWM1_EN == ON ||PLATFORM_PWM2_EN == ON ||PLATFORM_PWM3_EN == ON || \
    PLATFORM_PWM4_EN == ON || PLATFORM_PWM5_EN == ON || PLATFORM_PWM6_EN == ON || PLATFORM_PWM7_EN == ON)
	#include "vdchip_40xx_50xx_pwm/vdchip_40xx_50xx_pwm.h"
	#define PLATFORM_PWM_REGISTER	vd_fr8000_pwm_register
#endif

// 2.7 ADC
#if (PLATFORM_ADC0_EN == ON || PLATFROM_ADC1_EN == ON)
	#include "vdchip40xx_50xx_adc/vdchip40xx_50xx_adc.h"
	#define PLATFORM_ADC_REGISTER	vdchip_40xx_50xx_adc_register
#endif

// 2.8 TIMER
#if (PLATFORM_TIME0_EN == ON || PLATFORM_TIME1_EN == ON)
	#include "vdchip40xx_50xx_timer/vdchip40xx_50xx_timer.h"
	#define PLATFORM_TIMER_REGISTER	vdchip_40xx_50xx_timer_register
#endif

// 2.9 SYSTICK
#if (PLATFORM_SYSTICK_EN == ON)
#include "vd_bsp_systick/vd_bsp_systick.h"
#endif

// 2.10 FLASH
#if (PLATFORM_FLASH_EN == ON)
	#include "vdchip40xx_50xx_flash/vdchip40xx_50xx_flash.h"
	#define PLATFORM_FLASH_REGISTER	vdchip_40xx_50xx_flash_register
#endif

// 2.11 PM
#if (PLATFORM_PM_EN == ON)
	#include "vdchip40xx_50xx_pm/vdchip40xx_50xx_pm.h"
	#define PLATFORM_PM_REGISTER	fr8000_pm_register
#endif

// 2.12 ȫ���жϿ���
#if (PLATFORM_GLOBAL_SWITCH_EN == ON)
	#include "vdchip_40xx_50xx_global_switch/vdchip_40xx_50xx_global_switch.h"
	#define PLATFORM_GLOBAL_SWITCH_REGISTER	 fr8000_global_switch_register
#endif
/* ======================================================================================
 * ����ƽ̨���ù��߽ӿ��������
 */
//3.1 ��־��ӡ
#include "co_log.h"
#define PLATFORM_LOG_INIT()								log_init()
#define PLATFORM_LOGLN(format, ...) 			co_printf(format, ##__VA_ARGS__)
#define PLATFORM_LOG(format, ...) 				co_printf(format, ##__VA_ARGS__)

//3.2 �ӳٺ���
#include "sys_utils.h"
#define PLATFORM_DELAY_MS(time) 					co_delay_100us(time * 10)

//3.3 ʱ����ȡ
#define PLATFORM_GET_CURRENT_TIME_MS() 		system_get_curr_time()	//��ȡ����ʱ��

//3.4 �ڴ����
//#include "os_mem.h"
//#define PLATFORM_MALLOC(size)							os_malloc(size)
//#define PLATFORM_REALLOC(p, size)					os_realloc(p, size)
//#define PLATFORM_FREE(size)								os_free(size)
#include "malloc.h"
#define PLATFORM_MALLOC(size)							mymalloc(SRAMEX, size)
#define PLATFORM_REALLOC(p, size)					myrealloc(SRAMEX, p, size)
#define PLATFORM_FREE(p)									myfree(SRAMEX, p)
/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */

// __vd_extern_c_leave__

#endif // _PLATFORM_CONFIG_H
