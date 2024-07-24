#ifndef _VD_BSP_PWM_H_
#define _VD_BSP_PWM_H_ 	

#include "vd_bsp_gpio.h"
#include "base_type.h"
#include "vd_bsp_type.h"

#define PWM_MAX      5      //�������pwm����
#define PWM_MAP_MAX  8      //�������pwm����

#define PWM_TIM1     TIM1   //��ʱ��1
#define PWM_TIM2     TIM2   //��ʱ��1

/*pwmͨ����*/
typedef enum {
    pwm_channel_1 =  TIM_Channel_1,                               
	pwm_channel_2 =  TIM_Channel_2,
	pwm_channel_3 =  TIM_Channel_3,
	pwm_channel_4 =  TIM_Channel_4,
	
} PWM_CHANNEL_E;

/*pwm�ṹ��*/
typedef struct{
	GPIO_PORT_E      gpio_port;    //�Զ������ź�
	vd_uint32_t      pwm_fre_hz;   //Ƶ��
	vd_uint16_t      pwm_duty;     //ռ�ձ� 
    vd_uint16_t      period;       //PWMĬ�Ϸֱ���
	vd_int8_t        fre_offset;   //Ƶ�ʲ���
	GPIO_STATE_E     gpio_default_state; //����Ĭ�ϵ�ƽ
}vd_pwm_t;

/*pwm_port_map*/
typedef struct{
	GPIO_PORT_E      gpio_port;    //�Զ������ź�
	TIM_TypeDef *    timer;        //��ʱ��
	PWM_CHANNEL_E    chnanle;      //ͨ����
	
}vd_pwm_port_map_t;


vd_int32_t vd_bsp_pwm_init(vd_pwm_t* pwm);
vd_int32_t vd_bsp_pwm_start(vd_int32_t pwm_obj_id);
vd_int32_t vd_bsp_pwm_stop(vd_int32_t pwm_obj_id);
vd_int32_t vd_bsp_pwm_fake_gpio_set_value(vd_int32_t pwm_obj_id, GPIO_STATE_E value); //ģ��GPIO���õ�ƽ
vd_int32_t vd_bsp_pwm_set_duty(vd_int32_t pwm_obj_id, vd_uint32_t pwm_duty);

#endif
