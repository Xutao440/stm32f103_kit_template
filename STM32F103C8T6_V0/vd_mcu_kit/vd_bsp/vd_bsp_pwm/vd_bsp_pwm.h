#ifndef _VD_BSP_PWM_H_
#define _VD_BSP_PWM_H_ 	

#include "vd_bsp_gpio.h"
#include "base_type.h"
#include "vd_bsp_type.h"

#define PWM_MAX      5      //定义最大pwm个数
#define PWM_MAP_MAX  8      //定义最大pwm个数

#define PWM_TIM1     TIM1   //定时器1
#define PWM_TIM2     TIM2   //定时器1

/*pwm通道号*/
typedef enum {
    pwm_channel_1 =  TIM_Channel_1,                               
	pwm_channel_2 =  TIM_Channel_2,
	pwm_channel_3 =  TIM_Channel_3,
	pwm_channel_4 =  TIM_Channel_4,
	
} PWM_CHANNEL_E;

/*pwm结构体*/
typedef struct{
	GPIO_PORT_E      gpio_port;    //自定义引脚号
	vd_uint32_t      pwm_fre_hz;   //频率
	vd_uint16_t      pwm_duty;     //占空比 
    vd_uint16_t      period;       //PWM默认分辨率
	vd_int8_t        fre_offset;   //频率补偿
	GPIO_STATE_E     gpio_default_state; //引脚默认电平
}vd_pwm_t;

/*pwm_port_map*/
typedef struct{
	GPIO_PORT_E      gpio_port;    //自定义引脚号
	TIM_TypeDef *    timer;        //定时器
	PWM_CHANNEL_E    chnanle;      //通道号
	
}vd_pwm_port_map_t;


vd_int32_t vd_bsp_pwm_init(vd_pwm_t* pwm);
vd_int32_t vd_bsp_pwm_start(vd_int32_t pwm_obj_id);
vd_int32_t vd_bsp_pwm_stop(vd_int32_t pwm_obj_id);
vd_int32_t vd_bsp_pwm_fake_gpio_set_value(vd_int32_t pwm_obj_id, GPIO_STATE_E value); //模拟GPIO设置电平
vd_int32_t vd_bsp_pwm_set_duty(vd_int32_t pwm_obj_id, vd_uint32_t pwm_duty);

#endif
