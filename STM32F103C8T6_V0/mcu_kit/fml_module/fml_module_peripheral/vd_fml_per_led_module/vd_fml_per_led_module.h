#ifndef _VD_FML_PER_LED_MODULE_H
#define _VD_FML_PER_LED_MODULE_H

#include "base_type.h"
#include "vd_bsp_type.h"
#include "vd_bsp_gpio.h"
#include "vd_bsp_pwm.h"

#define LED_OBJ_MAX     5     //led����

/*LED��GPIO״̬*/
typedef enum {
	LED_STATE_OFF = 0,     // ��
    LED_STATE_ON,          // ��
}VD_FML_LED_STATE_E;

/*LED ��״̬*/
typedef enum {
	DRV_LED_OFF = 0,    //��
    DRV_LED_ON,         //��
	DRV_LED_FLICK,      //��˸
	DRV_LED_BREATHE,    //����
	
}VD_FML_LED_MODE_E;

/**/
typedef struct {
    vd_ulong_t  bright_time;  // һ����˸���ߺ���ʱ����ʱ��
    vd_ulong_t  dark_time;    // һ����˸���ߺ���ʱ���ʱ��
    vd_int16_t  mode_count;   // ��˸������Ĵ���
	
} vd_fml_per_led_param_t;

/*led��ʼ��*/
typedef struct {
	GPIO_PORT_E 	    gpio_port;            //�Զ������ź�
	GPIO_STATE_E        led_gpio_init_state;  //led���ų�ʼ״̬  	
	vd_uint32_t         breath_speed;         //�����ٶ�0
	vd_uint32_t         flick_time_ms;        //��˸ʱ����
	
} vd_fml_per_led_init_t;

/*led����*/
typedef struct {
	vd_fml_per_led_init_t  led_init;              //led��ʼ���ṹ��
	vd_uint16_t			   cur_pwm_duty;          //��ǰռ�ձ�
	vd_uint16_t			   pwm_resolution;        //pwm�ֱ���
	vd_int32_t             pwm_obj_id;            //pwm����Ĭ�ϴ�-1����ʼpwm�����
    VD_FML_LED_MODE_E      led_mode;              //led����ģʽ
    vd_uint32_t            work_time_s;           //led����ʱ��, �����ʼ��
	vd_uint32_t            curr_sys_time_ms;      //led��ǰ����ʱ��
	vd_uint32_t            curr_work_time_s;      //led��ǰ����ʱ��
    VD_FML_LED_STATE_E     curr_state;            //��ǰled����������

} vd_fml_per_led_t;

vd_int32_t vd_fml_per_led_init(vd_fml_per_led_init_t* led, vd_pwm_t* led_pwm);
vd_void_t vd_fml_per_led_loop(vd_void_t);
vd_bool_t vd_fml_per_led_breathe(vd_uint8_t led_id, vd_uint32_t* breathe_time_s);
vd_bool_t vd_fml_per_led_flick(vd_uint8_t led_id, vd_uint32_t* filck_time_s);
vd_bool_t vd_fml_per_led_off(vd_uint8_t led_id);
vd_bool_t vd_fml_per_led_on(vd_uint8_t led_id);
#endif
