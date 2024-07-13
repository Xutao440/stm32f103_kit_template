#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#include "base_type.h"


/************************ log config ************************/
#define LOG_ENABLE  1      //LOG总使能开关
     
#define ERROR_LOG   1       //错误
#define INFO_LOG    1       //信息
#define DEBUG_LOG   1       //调试
#define FATAL_LOG   1       //缺陷
#define ABORT_LOG   1       //死机
#define WARN_LOG    1       //警告


/************************ timer config ************************/
//TIMER配置
#define TIME1_EN 					ON
#define TIME1_PERIOD 				1000        //单位us  大于1us

#define TIME2_EN 					OFF
#define TIME2_PERIOD 				1000        //单位us

//PWM配置
#if (TIME1_EN == OFF)
	#define TIM1_CHANNEL1_EN        OFF 
	#define TIM1_CHANNEL2_EN        OFF
	#define TIM1_CHANNEL3_EN        OFF
	#define TIM1_CHANNEL4_EN        OFF
#endif

#if (TIME2_EN == OFF)
	#define TIM2_CHANNEL1_EN        OFF 
	#define TIM2_CHANNEL2_EN        ON
	#define TIM2_CHANNEL3_EN        OFF
	#define TIM2_CHANNEL4_EN        OFF
#endif



/************************ 测试 config ************************/

#define  TEST_GPIO1_PORT                   GPIOA
#define  TEST_GPIO1_PIN                    GPIO_Pin_4

#define  TEST_GPIO2_PORT                   GPIOA
#define  TEST_GPIO2_PIN                    GPIO_Pin_5


/************************ OLED config ************************/
/*是否使用*/
#define USE_OLED    	0

/*引脚配置*/
#define OLED_SCL_GPIO	GPIOB
#define OLED_SCL_PIN    GPIO_Pin_8

#define OLED_SDA_GPIO   GPIOB
#define OLED_SDA_PIN    GPIO_Pin_9


/************************ 旋钮 config ************************/

//KNOB_A  输入上拉
#define ENC1A_GPIO_PORT	  GPIOB
#define ENC1A_GPIO_PIN    GPIO_Pin_8
//KNOB_B  输入上拉
#define ENC1B_GPIO_PORT   GPIOB
#define ENC1B_GPIO_PIN    GPIO_Pin_9
//KNOB_KEY
#define KEY_GPIO_PORT   GPIOB
#define KEY_GPIO_PIN    GPIO_Pin_9


/************************ uart config ************************/

#define USART1_ENABLE   1     //使能串口
#define USART2_ENABLE   0
#define USART3_ENABLE   0

#define LOG_UART_PORT   1  //log串口选择
//串口1
#define USART1_RX_PORT	   GPIOA
#define USART1_RX_PIN	   GPIO_Pin_10
#define USART1_TX_PORT	   GPIOA
#define USART1_TX_PIN	   GPIO_Pin_9
#define USART1_MODE        USART_Mode_Rx | USART_Mode_Tx
#define USART1_BAUDRATE    115200
#define USART1_PRIORITY    1

//串口2
#define USART2_RX_PORT	   GPIOA
#define USART2_RX_PIN	   GPIO_Pin_3
#define USART2_TX_PORT	   GPIOA
#define USART2_TX_PIN	   GPIO_Pin_2
#define USART2_MODE        USART_Mode_Rx | USART_Mode_Tx
#define USART2_BAUDRATE    115200
#define USART2_PRIORITY    1

//串口3  未调试成功
#define USART3_RX_PORT	   GPIOB
#define USART3_RX_PIN	   GPIO_Pin_11
#define USART3_TX_PORT	   GPIOB
#define USART3_TX_PIN	   GPIO_Pin_10
#define USART3_MODE        USART_Mode_Rx | USART_Mode_Tx
#define USART3_BAUDRATE    115200
#define USART3_PRIORITY    1

/************************ pwm config ************************/

/*是否使用*/
#define USE_OLED    	0

//timer2_pwm
#define TIMER2_PWM_PORT	   GPIOA
#define TIMER2_PWM_PIN	   GPIO_Pin_3
#define TIMER2_PWM_CHANNEL TIM_channel_4

#endif





