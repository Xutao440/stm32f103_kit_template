/*
 * Project: Control project
 * Module: ƽ̨������
 * File: driver_config.h
 * Created Date: 2022��7��23��14:17:09
 * Author: gwx
 * Description: ƽ̨��ص�����
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRIVER_CONFIG_H
#define _DRIVER_CONFIG_H

/* ======================================================================================
 * includes
 */
#include "mcu_kit_config.h"
#include "project_config.h"
/* ======================================================================================
 * extern
 */
#define VD_MIN(a, b) (a) < (b) ? (a) : (b)
#define VD_MAX(a, b) (a) > (b) ? (a) : (b)
#define ON 1
#define OFF 0
/* ======================================================================================
 * types
 */
// mcu-kit����֧�ֵ�ƽ̨
#define DRIVER_HAL_MR88 0x01


#ifndef KIT_DRIVER_HAL
#define KIT_DRIVER_HAL DRIVER_HAL_MR88
#endif
/*
************************driver����������Ϣ*************************************************
*/
/**************************************
							BSP�⼰ʹ���ܿ���
**************************************/
#ifndef DRIVER_BSP_EN
#define DRIVER_BSP_EN ON
#endif

#if (DRIVER_BSP_EN == ON)
/**************************1.1 GPIO����*************************/ 
#ifndef DRIVER_BSP_GPIO_EN
#define DRIVER_BSP_GPIO_EN ON
#endif

/**************************1.2 Uart ����*************************/ 
#ifndef DRIVER_BSP_UART_EN
#define DRIVER_BSP_UART_EN ON
#endif


/**************************1.3 I2C ����*************************/ 
#ifndef DRIVER_BSP_I2C_EN
#define DRIVER_BSP_I2C_EN OFF
#endif

/**************************1.4 SPIM ����*************************/ 
#ifndef DRIVER_BSP_SPIM_EN
#define DRIVER_BSP_SPIM_EN OFF
#endif

/**************************1.5 SPIS ����*************************/ //MR88F102 maximum frequency is 2MHz ?
#ifndef DRIVER_BSP_SPIS_EN
#define DRIVER_BSP_SPIS_EN OFF
#endif

/**************************1.6 PWM ����*************************/ 
#ifndef DRIVER_BSP_PWM_EN
#define DRIVER_BSP_PWM_EN OFF //��ʹ��
#endif

/**************************1.7 TIMER ����*************************/ 
#ifndef DRIVER_BSP_TIMER_EN
#define DRIVER_BSP_TIMER_EN OFF
#endif

/**************************1.8 ADC ����*************************/ 
#ifndef DRIVER_BSP_ADC_EN
#define DRIVER_BSP_ADC_EN OFF
#endif

/**************************1.9 DEBUG ����*************************/ 
#ifndef DRIVER_BSP_DEBUG_EN
#define DRIVER_BSP_DEBUG_EN OFF
#endif

/**************************1.10 SYSTICK ����*************************/ 
#ifndef DRIVER_BSP_SYSTICK_EN
#define DRIVER_BSP_SYSTICK_EN OFF
#endif

/**************************1.11 �ڲ�FLASH ����*************************/ 
#ifndef DRIVER_BSP_SYSTEM_FLASH_EN
#define DRIVER_BSP_SYSTEM_FLASH_EN OFF // Ƭ��Flash�����������������ö�ٶ�������
#endif

/**************************1.12 ���FLASH ����*************************/ 
#ifndef DRIVER_BSP_NOR_FLASH_EN
#define DRIVER_BSP_NOR_FLASH_EN OFF // Ƭ��Flash�����������������ö�ٶ�������
#endif

/**************************1.xx XXX����*************************/ 

/**************************1.xx XXX����*************************/ 
#endif  //(DRIVER_BSP_EN == ON)

/**************************************
							FMLʹ���ܿ���
**************************************/
#ifndef DRIVER_FML_EN
#define DRIVER_FML_EN ON
#endif
#if (DRIVER_FML_EN==ON)

/**************************2.1 KEY����*************************/ 
#ifndef DRIVER_FML_KEY_EN
#define DRIVER_FML_KEY_EN OFF
#endif

/**************************2.1 KEY����*************************/ 
#ifndef DRIVER_FML_KNOB_EN
#define DRIVER_FML_KNOB_EN OFF
#endif

/**************************2.2 SENSOR����*************************/ 
#ifndef DRIVER_FML_SENSOR_EN
#define DRIVER_FML_SENSOR_EN OFF
#endif

/**************************2.3 ϵͳʱ��ģ��*************************/ 
#ifndef DRIVER_FML_TIME_EN
#define DRIVER_FML_TIME_EN OFF
#endif

/**************************2.4 POWER����*************************/ 
#ifndef DRIVER_FML_POWER_EN
#define DRIVER_FML_POWER_EN OFF
#endif

/**************************2.5 BATTERY����*************************/ 
#ifndef DRIVER_FML_BATTERY_EN
#define DRIVER_FML_BATTERY_EN OFF
#endif

/**************************2.6 ���Ź�����*************************/ 
#ifndef DRIVER_FML_WDT_EN
#define DRIVER_FML_WDT_EN OFF
#endif

/**************************��������ģ��*************************/ 
#ifndef DRIVER_FML_TEST_EN
#define DRIVER_FML_TEST_EN OFF
#endif

/**************************2.7 �Ƿ���Ҫʹ�ô���ͨ����Ϣ��������*************************/ 
#ifndef DRIVER_FML_UART_ME_QUEUE_EN
#define DRIVER_FML_UART_ME_QUEUE_EN OFF
#endif

/**************************2.8 ���ݱ��汸��ģ������*************************/ 
#ifndef DRIVER_FML_DATA_BACKUP_EN
#define DRIVER_FML_DATA_BACKUP_EN OFF 
#endif

/**************************2.9 SPI ͨ�ŵ�ذ�*************************/ 
#ifndef DRIVER_FML_BMS_SPI_EN
#define DRIVER_FML_BMS_SPI_EN OFF 
#endif

/**************************2.10 ����ͨ����ʾ����λ��*************************/ 
#ifndef DRIVER_FML_UART_DISPLAY_EN
#define DRIVER_FML_UART_DISPLAY_EN OFF 	
#endif

/**************************2.11 UART ͨ�ŵ�ذ�*************************/ 
#ifndef DRIVER_FML_BMS_UART_EN
#define DRIVER_FML_BMS_UART_EN OFF 	
#endif

/**************************2.12 led����ģ�� LED����*************************/ 
#ifndef DRIVER_FML_LED_EN
#define DRIVER_FML_LED_EN OFF
#endif

/**************************2.13 led��ʾ������ģ�� LED����*************************/ 
#ifndef DRIVER_FML_LED_DISPLAY_EN
#define DRIVER_FML_LED_DISPLAY_EN OFF
#endif

/**************************2.14 ���ģ������*************************/ 
#ifndef DRIVER_FML_MOTOR_EN
#define DRIVER_FML_MOTOR_EN ON  
#endif
#if (DRIVER_FML_MOTOR_EN == ON)
#define DRIVER_FML_MOTOR_TEST_EN OFF
//����BLDC���
#ifndef DRIVER_FML_MOTOR_TYPE_BLDC
#define DRIVER_FML_MOTOR_TYPE_BLDC OFF
#endif
/**************************2.16 ��ˢģ������*************************/ //����ֱ�����
#ifndef DRIVER_FML_MOTOR_TYPE_DC
#define DRIVER_FML_MOTOR_TYPE_DC OFF
#endif
//���ý���������� �����������
#ifndef DRIVER_FML_MOTOR_TYPE_BSAC
#define DRIVER_FML_MOTOR_TYPE_BSAC OFF
#endif
#endif


/**************************2.15 ����ģ������*************************/ 
#ifndef DRIVER_FML_INFRARED_EN
#define DRIVER_FML_INFRARED_EN OFF
#endif



/**************************2.17 �̵���ģ������*************************/ 
#ifndef DRIVER_FML_RELAY_EN
#define DRIVER_FML_RELAY_EN OFF	
#endif

/**************************2.18 ����ģ������*************************/ 
#ifndef DRIVER_FML_FAN_EN
#define DRIVER_FML_FAN_EN OFF	
#endif

/**************************2.19 ����ģ������*************************/ 
#ifndef DRIVER_FML_HEAT_EN
#define DRIVER_FML_HEAT_EN OFF	
#endif

/**************************2.20 �¶ȼ��ģ������*************************/ 
#ifndef DRIVER_FML_TEMP_EN
#define DRIVER_FML_TEMP_EN OFF	
#endif

/**************************2.21 ������ģ������*************************/ 
#ifndef DRIVER_FML_BEEP_EN
#define DRIVER_FML_BEEP_EN OFF	
#endif

/**************************2.22 ���ӳ�ģ�飨����ģ�飩����*************************/ 
#ifndef DRIVER_FML_UART_WEIGHING_EN
#define DRIVER_FML_UART_WEIGHING_EN OFF	
#endif

/**************************2.23 ������ģ������*************************/ 
#ifndef DRIVER_FML_ZERO_CROSSING_DETECTION_EN
#define DRIVER_FML_ZERO_CROSSING_DETECTION_EN OFF	
#endif

/**************************x.x.x XXX����*************************/ 

/**************************x.x.x XXX����*************************/ 
#endif  // (DRIVER_FML_EN==ON)/****************************driver���ý���********************************/







#if (DRIVER_BSP_GPIO_EN==ON)
#define DRIVER_BSP_GPIO_NUM_MAX (16 + 16 + 13 + 7)
#define DRIVER_BSP_GPIO_WAKE_UP_EN ON
#define DRIVER_BSP_GPIO_ALERT_EN ON
#define DRIVER_BSP_GPIO_CHECK_CHARGE_EN ON
#endif

// 1.2 Uart ����

#if (DRIVER_BSP_UART_EN == ON)//log
	#ifndef DRIVER_BSP_UART0_EN
	#define DRIVER_BSP_UART0_EN ON	//log
	#endif
	#ifndef DRIVER_BSP_UART1_EN
	#define DRIVER_BSP_UART1_EN OFF
	#endif
	#ifndef DRIVER_BSP_UART2_EN
	#define DRIVER_BSP_UART2_EN OFF  
	#endif
	
#if (DRIVER_BSP_UART0_EN==ON)
#define DRIVER_BSP_UART0_TEST_EN OFF
#define DRIVER_BSP_UART0_TX_PORT DRV_GPIO_C
#define DRIVER_BSP_UART0_TX_PIN 9
#define DRIVER_BSP_UART0_RX_PORT DRV_GPIO_C
#define DRIVER_BSP_UART0_RX_PIN 8
#define DRIVER_BSP_UART0_BAUDEATE 9600
#define DRIVER_BSP_UART0_CLOCK SYSTEM_CLOCK
#endif

#if (DRIVER_BSP_UART1_EN==ON)
#define DRIVER_BSP_UART1_TEST_EN OFF
#define DRIVER_BSP_UART1_TX 23
#define DRIVER_BSP_UART1_RX 22
#define DRIVER_BSP_UART1_BAUDEATE 115200
#define DRIVER_BSP_UART1_CLOCK SYSTEM_CLOCK
#endif

#if (DRIVER_BSP_UART2_EN==ON)

#define DRIVER_BSP_UART2_TEST_EN OFF

#define DRIVER_BSP_UART2_TX_PORT DRV_GPIO_B
#define DRIVER_BSP_UART2_TX_PIN 3
#define DRIVER_BSP_UART2_RX_PORT DRV_GPIO_B
#define DRIVER_BSP_UART2_RX_PIN 4
#define DRIVER_BSP_UART2_CLOCK SYSTEM_CLOCK
#ifndef DRIVER_BSP_UART2_BAUDEATE
#define DRIVER_BSP_UART2_BAUDEATE 9600
#endif
#endif
#endif 	//(DRIVER_BSP_UART_EN == ON)

// 1.3 I2C ����
#if (DRIVER_BSP_I2C_EN==ON)
typedef enum
{
    DRV_I2C_SPEED_STAND,   // 100khz
    DRV_I2C_SPEED_FAST,    // 400hz
    DRV_I2C_SPEED_HIGHEDT, // 3.4Mhz
} drv_i2c_speed;

typedef enum
{
    DRV_I2C_SLAVE_ADDRESS_7BIT,
    DRV_I2C_SLAVE_ADDRESS_10BIT,
} drv_i2c_addr_bit;

#ifndef DRIVER_BSP_I2C0_EN
#define DRIVER_BSP_I2C0_EN OFF
#endif

#ifndef DRIVER_BSP_I2C1_EN
#define DRIVER_BSP_I2C1_EN OFF
#endif

#if DRIVER_BSP_I2C0_EN==ON
#define DRIVER_BSP_I2C0_SCL 2
#define DRIVER_BSP_I2C0_SDA 3
#define DRIVER_BSP_I2C0_SPEED DRV_I2C_SPEED_STAND           // 0,100khz;1,400khz
#define DRIVER_BSP_I2C0_ARRD_BIT DRV_I2C_SLAVE_ADDRESS_7BIT // 0,7bit;1,10bit
#endif
#if DRIVER_BSP_I2C1_EN==ON
#define DRIVER_BSP_I2C1_SCL 2
#define DRIVER_BSP_I2C1_SDA 3
#define DRIVER_BSP_I2C1_SPEED DRV_I2C_SPEED_STAND           // 0,100khz;1,400khz
#define DRIVER_BSP_I2C1_ARRD_BIT DRV_I2C_SLAVE_ADDRESS_7BIT // 0,7bit;1,10bit
#endif
#endif

// 1.4 SPIM ����
#if (DRIVER_BSP_SPIM_EN==ON)
#define DRIVER_BSP_SPIM_TEST_EN ON

#ifndef DRIVER_BSP_SPIM0_EN
#define DRIVER_BSP_SPIM0_EN ON
#endif
#ifndef DRIVER_BSP_SPIM1_EN
#define DRIVER_BSP_SPIM1_EN OFF
#endif

#if DRIVER_BSP_SPIM0_EN==ON
#define DRIVER_BSP_SPIM0_CS_TYPE 1       //ѡ�� 0x00->�жϴ������շ����ݣ�0x01->����շ�����
#define DRIVER_BSP_SPIM0_CS_PIN PORT_B_2 // SPI0Ƭѡ��
#define DRIVER_BSP_SPIM0_CMD_PIN 2       // SPI0��������ѡ���
#define DRIVER_BSP_SPIM0_DATA_BIT 10     // SPI0�������ݳ���
#define DRIVER_BSP_SPIM0_SPEED 24000000  // maximum frequency is 24MHz
#endif

#if DRIVER_BSP_SPIM1_EN==ON
#define DRIVER_BSP_SPIM1_CS_TYPE 2      //ѡ�� 0x01->cs0 0x02->cs1
#define DRIVER_BSP_SPIM1_CS_PIN 7       // SPI1Ƭѡ��
#define DRIVER_BSP_SPIM1_CMD_PIN 2      // SPI1��������ѡ���
#define DRIVER_BSP_SPIM1_SPEED 24000000 // maximum frequency is 24MHz
#endif
#endif

// 1.5 SPIS����  // maximum frequency is 2MHz
#if (DRIVER_BSP_SPIS_EN==ON)
#define DRIVER_BSP_SPIS_CS_PIN 3
#endif

// 1.6 PWM����
#if (DRIVER_BSP_PWM_EN == ON)
#define DRIVER_BSP_PWM_TEST_EN OFF //����ʹ��

	// PWM0
	#ifndef DRIVER_BSP_PWM_BSTIMER0_EN
	#define DRIVER_BSP_PWM_BSTIMER0_EN OFF
	#endif
	// PWM1
	#ifndef DRIVER_BSP_PWM_BSTIMER1_EN
	#define DRIVER_BSP_PWM_BSTIMER1_EN OFF
	#endif

// PWM0//�������
#if (DRIVER_BSP_PWM_BSTIMER0_EN == ON)
#define DRIVER_BSP_PWM_BSTIMER0_PORT DRV_GPIO_B
#define DRIVER_BSP_PWM_BSTIMER0_PIN 13
#define DRIVER_BSP_PWM_BSTIMER0_CLOCK SYSTEM_CLOCK
#ifndef DRIVER_BSP_PWM_BSTIMER0_FREQUENCY
#define DRIVER_BSP_PWM_BSTIMER0_FREQUENCY 4000
#endif
#endif
// PWM1//��ˢ��Դ����
#if (DRIVER_BSP_PWM_BSTIMER1_EN == ON) 
#define DRIVER_BSP_PWM_BSTIMER1_PIN PORT_B_11
#define DRIVER_BSP_PWM_BSTIMER1_CLOCK SYSTEM_CLOCK
#ifndef DRIVER_BSP_PWM_BSTIMER0_FREQUENCY
#define DRIVER_BSP_PWM_BSTIMER1_FREQUENCY 1000
#endif

#endif

// PWM����
#define DRIVER_BSP_PWM_WATCHTIM_EN OFF
#endif
// 1.7 TIMER����
#if (DRIVER_BSP_TIMER_EN == ON)
#ifndef DRIVER_BSP_TIMER_GTIMER0_EN
#define DRIVER_BSP_TIMER_GTIMER0_EN ON // 1ms��ʱ������δʹ��	
#endif
#ifndef DRIVER_BSP_TIMER_GTIMER1_EN
#define DRIVER_BSP_TIMER_GTIMER1_EN ON // 100us��ʱ����ϵͳ��ʱ
#endif
#ifndef DRIVER_BSP_TIMER_GTIMER2_EN
#define DRIVER_BSP_TIMER_GTIMER2_EN OFF
#endif
#ifndef DRIVER_BSP_TIMER_BSTIMER0_EN
#define DRIVER_BSP_TIMER_BSTIMER0_EN OFF
#endif
#ifndef DRIVER_BSP_TIMER_BSTIMER1_EN
#define DRIVER_BSP_TIMER_BSTIMER1_EN OFF
#endif
#ifndef DRIVER_BSP_TIMER_WATCHTIM_EN
#define DRIVER_BSP_TIMER_WATCHTIM_EN OFF
#endif



#if (DRIVER_BSP_TIMER_GTIMER0_EN == ON)
#define DRIVER_BSP_TIMER_GTIMER0_CLOCK SYSTEM_CLOCK
#ifndef DRIVER_BSP_TIMER_GTIMER0_FREQUENCY
#define DRIVER_BSP_TIMER_GTIMER0_FREQUENCY 1000
#endif
#endif
#if (DRIVER_BSP_TIMER_GTIMER1_EN == ON)
#define DRIVER_BSP_TIMER_GTIMER1_CLOCK SYSTEM_CLOCK
#ifndef DRIVER_BSP_TIMER_GTIMER1_FREQUENCY
#define DRIVER_BSP_TIMER_GTIMER1_FREQUENCY 10000
#endif
#endif
#if (DRIVER_BSP_TIMER_GTIMER2_EN == ON)
#define DRIVER_BSP_TIMER_GTIMER2_CLOCK SYSTEM_CLOCK
#ifndef DRIVER_BSP_TIMER_GTIMER2_FREQUENCY
#define DRIVER_BSP_TIMER_GTIMER2_FREQUENCY 1000
#endif
#endif
#endif


// 1.8 ADC����
#if (DRIVER_BSP_ADC_EN == ON)
#define DRIVER_BSP_ADC_TEST_EN OFF

#ifndef DRIVER_BSP_ADC7_EN
#define DRIVER_BSP_ADC7_EN OFF	
#endif
#if DRIVER_BSP_ADC7_EN==ON
#define DRIVER_BSP_ADC7_PORT DRV_GPIO_B
#define DRIVER_BSP_ADC7_PIN 5 //�ŵ�ʱ���ŵ���������ʱ��������
#endif

#ifndef DRIVER_BSP_ADC16_EN
#define DRIVER_BSP_ADC16_EN OFF	
#endif

#if DRIVER_BSP_ADC16_EN==ON
#define DRIVER_BSP_ADC16_PORT DRV_GPIO_C
#define DRIVER_BSP_ADC16_PIN 10 //������������
#endif

#ifndef DRIVER_BSP_ADC18_EN
#define DRIVER_BSP_ADC18_EN OFF	
#endif

#if DRIVER_BSP_ADC18_EN==ON
#define DRIVER_BSP_ADC18_PORT DRV_GPIO_C
#define DRIVER_BSP_ADC18_PIN 12 //�������ؼ��
#endif

#ifndef DRIVER_BSP_ADC19_EN
#define DRIVER_BSP_ADC19_EN OFF	
#endif

#if DRIVER_BSP_ADC19_EN==ON
#define DRIVER_BSP_ADC19_PORT DRV_GPIO_D 
#define DRIVER_BSP_ADC19_PIN 0 //������ռ��
#endif

#ifndef DRIVER_BSP_ADC25_EN
#define DRIVER_BSP_ADC25_EN OFF	
#endif

#if DRIVER_BSP_ADC25_EN==ON
#define DRIVER_BSP_ADC25_PORT DRV_GPIO_D 
#define DRIVER_BSP_ADC25_PIN 6 //�������
#endif
#endif

// 1.11 �ڲ�FLASH����
#if (DRIVER_BSP_SYSTEM_FLASH_EN==ON)
#define  DRIVER_BSP_SYSTEM_FLASH_TEST_EN OFF
typedef enum
{
    DRV_SYSTEM_FLASH_PAGE_ERASE,       // 512byte
    DRV_SYSTEM_FLASH_SECTOR_ERASE,     // 4K
    DRV_SYSTEM_FLASH_BLOCK_32KB_ERASE, // 32K
    DRV_SYSTEM_FLASH_BLOCK_64KB_ERASE, // 64k
    DRV_SYSTEM_FLASH_CHIP_ERASE,       // all
} drv_system_flash_erase_e;

#endif

// 1.12 ���FLASH����
#if (DRIVER_BSP_NOR_FLASH_EN==ON)
typedef enum
{
    flash_Page_Erase,       // 256byte
    flash_Sector_Erase,     // 4K
    flash_Block_32KB_Erase, // 32K
    flash_Block_64KB_Erase, // 64k
    flash_Chip_Erase,       // all
} drv_flash_erase_t;
#endif
/* ======================================================================================*/
  /************оƬƽ̨���ý���**************/

/* ======================================������������ƽ̨ʵ��================================================
 * ��������ģ���������
 */


#if (DRIVER_FML_TIME_EN == ON)
#define DRIVER_FML_TIME_TEST_EN OFF
#endif


// 2.9 BATTERY����
#if (DRIVER_FML_BATTERY_EN == ON)
#define DRIVER_FML_BATTERY_TEST_EN ON
#endif

//  BMS��ذ�����
#if (DRIVER_BSP_SPIM_EN==ON)
//SPI ͨ�ŵ�ذ�
#if DRIVER_FML_BMS_SPI_EN==ON
#define DRIVER_FML_BMS_SPI_TEST_EN	OFF
#endif
#endif
#if (DRIVER_BSP_UART_EN == ON)
//UART ͨ�ŵ�ذ�
#if (DRIVER_FML_BMS_UART_EN==ON)
#define DRIVER_FML_BMS_UART_TEST_EN OFF
#endif
#endif

//	led����ģ�� LED����
#if DRIVER_FML_LED_EN==ON
#define  DRIVER_FML_LED_TEST_EN OFF
#define DRIVER_FML_LED_RIGHT1_PORT DRV_GPIO_B
#define DRIVER_FML_LED_RIGHT1_PIN 12
#define DRIVER_FML_LED_RIGHT2_PORT DRV_GPIO_B
#define DRIVER_FML_LED_RIGHT2_PIN 13
#define DRIVER_FML_LED_LEFT1_PORT DRV_GPIO_B
#define DRIVER_FML_LED_LEFT1_PIN 14
#define DRIVER_FML_LED_LEFT2_PORT DRV_GPIO_B
#define DRIVER_FML_LED_LEFT2_PIN 15
#endif
//	led��ʾ������ģ�� LED����
#if DRIVER_FML_LED_DISPLAY_EN==ON
#define DRIVER_FML_LED_DISPLAY_TEST_EN OFF
#define DRIVER_FML_LED_DISPLAY_DIO_PORT DRV_GPIO_B
#define DRIVER_FML_LED_DISPLAY_DIO_PIN 2
#define DRIVER_FML_LED_DISPLAY_CLK_PORT DRV_GPIO_B
#define DRIVER_FML_LED_DISPLAY_CLK_PIN 1
#define DRIVER_FML_LED_DISPLAY_STB_PORT DRV_GPIO_B
#define DRIVER_FML_LED_DISPLAY_STB_PIN 7
#endif

#if (DRIVER_FML_UART_DISPLAY_EN==ON)//����ͨ����ʾ����λ��
#define DRIVER_FML_UART_DISPLAY_TEST_EN ON
#define DRIVER_FML_UART_DISPLAY_WAKEUP_PORT DRV_GPIO_B
#define DRIVER_FML_UART_DISPLAY_WAKEUP_PIN 3		//����Tx�ɸ�����Ϊ��λ����ʾ�Ļ���IO
#endif

//	����ģ�� IO������
#if DRIVER_FML_FAN_EN==ON
#define DRIVER_FML_FAN_TEST_EN OFF
#define DRIVER_FML_FAN_PORT DRV_GPIO_B
#define DRIVER_FML_FAN_PIN 1
#endif
//	����ģ�� IO������
#if DRIVER_FML_HEAT_EN==ON
#define DRIVER_FML_HEAT_TEST_EN OFF
#define DRIVER_FML_HEAT_L_PORT DRV_GPIO_C //����L��
#define DRIVER_FML_HEAT_L_PIN 2
#define DRIVER_FML_HEAT_N_PORT DRV_GPIO_C	//����N��
#define DRIVER_FML_HEAT_N_PIN 1
#endif


//	������ģ�� IO������
#if DRIVER_FML_BEEP_EN==ON
#define DRIVER_FML_BEEP_TEST_EN ON
#define DRIVER_FML_BEEP_PORT DRV_GPIO_B
#define DRIVER_FML_BEEP_PIN 11
#endif

//	������ģ�� IO������
#if DRIVER_FML_ZERO_CROSSING_DETECTION_EN==ON
#define DRIVER_FML_ZERO_CROSSING_DETECTION_TEST_EN OFF
#define DRIVER_FML_ZERO_CROSSING_DETECTION_PORT DRV_GPIO_C
#define DRIVER_FML_ZERO_CROSSING_DETECTION_PIN 7
#define DRIVER_FML_ZERO_CROSSING_DETECTION_IRQ   OFF    //����ģʽѡ��(�жϷ�ʽ����ѯ��ʽ)
#endif


#if DRIVER_FML_TEMP_EN==ON
#define DRIVER_FML_TEMP_TEST_EN OFF
#endif
#if DRIVER_FML_UART_WEIGHING_EN==ON
#define DRIVER_FML_UART_WEIGHING_TEST_EN ON
#endif
#if DRIVER_FML_RELAY_EN==ON
#define DRIVER_FML_RELAY_TEST_EN OFF
#endif

#if (DRIVER_BSP_ADC_EN==ON)
#if (DRIVER_FML_INFRARED_EN == ON)
#define DRIVER_FML_INFRARED_TEST_EN OFF
#endif
#endif


#if (DRIVER_FML_MOTOR_TYPE_DC == ON)
#define DRIVER_FML_MOTOR_TYPE_DC_TEST_EN OFF
//��ˢ�Ƿ�װ����
#define DRIVER_FML_MOTOR_TYPE_DC_BRUSH_CHECK_PORT DRV_GPIO_B
#define DRIVER_FML_MOTOR_TYPE_DC_BRUSH_CHECK_PIN 8
//��ˢת�ٿ���pwm
#define DRIVER_FML_MOTOR_TYPE_DC_MOTOR_PWM_ID DRIVER_BSP_PWM_BEEP1
#define DRIVER_FML_MOTOR_TYPE_DC_PWM_FREQUENCY DRIVER_BSP_PWM_BSTIMER1_FREQUENCY
#define DRIVER_FML_MOTOR_TYPE_DC_INIT_SPEED0_DUTY  0
//#define DRIVER_FML_MOTOR_TYPE_DC_SPEED_DUTY 90   //Ĭ��״̬ת��
#define DRIVER_FML_MOTOR_TYPE_DC_LOW_SPEED_DUTY  50
#define DRIVER_FML_MOTOR_TYPE_DC_MIDDLE_SPEED_DUTY 70
#define DRIVER_FML_MOTOR_TYPE_DC_HIGH_SPEED_DUTY 90//90->1275
#define DRIVER_FML_MOTOR_TYPE_DC_CARPET_MODE_DUTY 10//��̺ģʽ�¹�ˢ���ת�ٽ���10%
#endif



#if DRIVER_FML_MOTOR_TYPE_BLDC==ON
// FG main ������������
#define DRIVER_FML_MOTOR_TYPE_BLDC_FG_SPEED_DETECTION_PORT DRV_GPIO_C
#define DRIVER_FML_MOTOR_TYPE_BLDC_FG_SPEED_DETECTION_PIN 1
#define DRIVER_FML_MOTOR_TYPE_BLDC_FG_SPEED_IRQ   ON    //����ģʽѡ��(�жϷ�ʽ����ѯ��ʽ)
//�������ʹ�ܿ��ƽ�
#define DRIVER_FML_MOTOR_TYPE_BLDC_POWER_EN_PORT DRV_GPIO_C
#define DRIVER_FML_MOTOR_TYPE_BLDC_POWER_EN_PIN 2
// pwm���ת�ٿ��ƽ�
#define DRIVER_FML_MOTOR_TYPE_BLDC_PWM_ID DRIVER_BSP_PWM_BEEP0
#define DRIVER_FML_MOTOR_TYPE_BLDC_PWM_FREQUENCY DRIVER_BSP_PWM_BSTIMER0_FREQUENCY
#define DRIVER_FML_MOTOR_TYPE_BLDC_INIT_SPEED_DUTY  0
#define DRIVER_FML_MOTOR_TYPE_BLDC_LOW_SPEED_DUTY  20
#define DRIVER_FML_MOTOR_TYPE_BLDC_MIDDLE_SPEED_DUTY 45
#define DRIVER_FML_MOTOR_TYPE_BLDC_HIGH_SPEED_LOW_DUTY 60
#define DRIVER_FML_MOTOR_TYPE_BLDC_HIGH_SPEED_DUTY 90//90->1275
#define DRIVER_FML_MOTOR_TYPE_BLDC_CARPET_MODE_DUTY 25//��̺ģʽ�����������������15%
#endif

#if DRIVER_FML_MOTOR_TYPE_BSAC==ON
// �������ת�̵�������
#define DRIVER_FML_MOTOR_TYPE_BSAC_ROTATE_PORT DRV_GPIO_C   //Ĭ�ϼ̵������򿪣���ת����
#define DRIVER_FML_MOTOR_TYPE_BSAC_ROTATE_PIN 3
// ���ɲ���̵�������
#define DRIVER_FML_MOTOR_TYPE_BSAC_BRAKE_PORT DRV_GPIO_B  //Ĭ�Ϲرղ��򿪣�ɲ����Ҫ�򿪣�ʹ��PWM����
#define DRIVER_FML_MOTOR_TYPE_BSAC_BRAKE_PIN 15
// ����ܵ�̵�������
#define DRIVER_FML_MOTOR_TYPE_BSAC_POWER_EN_PORT DRV_GPIO_B
#define DRIVER_FML_MOTOR_TYPE_BSAC_POWER_EN_PIN 14
// �����������
#define DRIVER_FML_MOTOR_TYPE_BSAC_FG_SPEED_DETECTION_PORT DRV_GPIO_C
#define DRIVER_FML_MOTOR_TYPE_BSAC_FG_SPEED_DETECTION_PIN 10
#define DRIVER_FML_MOTOR_TYPE_BSAC_FG_SPEED_IRQ   ON    //����ģʽѡ��(�жϷ�ʽ����ѯ��ʽ)
// pwm���ת�ٿ��ƽ�
#define DRIVER_FML_MOTOR_TYPE_BSAC_PWM_ID DRIVER_BSP_PWM_BEEP0
#define DRIVER_FML_MOTOR_TYPE_BSAC_PWM_FREQUENCY DRIVER_BSP_PWM_BSTIMER0_FREQUENCY
#define DRIVER_FML_MOTOR_TYPE_BSAC_INIT_SPEED_DUTY  0
#define DRIVER_FML_MOTOR_TYPE_BSAC_LOW_SPEED_DUTY  20
#define DRIVER_FML_MOTOR_TYPE_BSAC_MIDDLE_SPEED_DUTY 45
#define DRIVER_FML_MOTOR_TYPE_BSAC_HIGH_SPEED_DUTY 90//90->1275
#define DRIVER_FML_MOTOR_TYPE_BSAC_CARPET_MODE_DUTY 25//��̺ģʽ�����������������15%
#endif




//���ݱ��汸��ģ��
#if (DRIVER_FML_DATA_BACKUP_EN==ON)
#define DRIVER_FML_DATA_BACKUP_TEST_EN OFF
#endif


//���ADC ֵ�жϴ���������һ������
#if (DRIVER_FML_ADC_KEY_EN == ON)
#define DRIVER_FML_ADC_KEY_TEST_EN OFF
#endif



#if (DRIVER_BSP_ADC_EN==ON)
#if (DRIVER_FML_INFRARED_EN == ON)
#define DRIVER_FML_INFRARED_TEST_EN OFF
#endif
#endif

#if (DRIVER_FML_KEY_EN==ON)
/**************************2.1.1 ADC KEY����*************************/ 
#ifndef DRIVER_FML_ADC_KEY_EN
#define DRIVER_FML_ADC_KEY_EN OFF//���ADC ֵ�жϴ���������һ������
#endif

/**************************2.1.2 GPIO KEY����*************************/ 
#ifndef DRIVER_FML_GPIO_KEY_EN
#define DRIVER_FML_GPIO_KEY_EN OFF//���SW ֵ�жϴ������ǳ������Ƕ̰�
//#define DRIVER_FML_POT_DETECTION_EN	ON	//������ģ��
#endif

#if (DRIVER_FML_GPIO_KEY_EN==ON)//����ģ��
#define DRIVER_FML_KEY_TEST_EN ON
#define DRIVER_FML_KEY_SW_PORT DRV_GPIO_C
#define DRIVER_FML_KEY_SW_PIN 6		//����Tx�ɸ�����Ϊ��λ����ʾ�Ļ���IO
#define DRIVER_FML_KEY_KEY_SLEEP_EN_PORT DRV_GPIO_B
#define DRIVER_FML_KEY_KEY_SLEEP_EN_PIN 12		//���߼�����ʹ��
//	������ģ�� IO������
#if DRIVER_FML_POT_DETECTION_EN==ON
#define DRIVER_FML_POT_DETECTION_PORT DRV_GPIO_C
#define DRIVER_FML_POT_DETECTION_PIN 6
#endif

#define DRIVER_FML_KEY_IRQ   OFF    //����ģʽѡ��(�жϷ�ʽ����ѯ��ʽ)
#define DRIVER_FML_KEY_IRQ_MODE	OFF  // ON�жϼ�ⰴ�������ͷ�|OFFֻ��ⰴ������״̬
#define DRIVER_FML_KEY_STATE OFF   // ON�ϱ�������ֵ|OFF�ϱ�����״̬
#define DRIVER_FML_KEY_LONG_TIME_SET      2000  // ms
#endif
#endif

#endif // _DRIVER_CONFIG_H
