#ifndef _VD_FML_PER_LED_MODULE_H
#define _VD_FML_PER_LED_MODULE_H

#include "base_type.h"
#include "vd_bsp_type.h"
#include "vd_bsp_gpio.h"
#include "vd_bsp_pwm.h"

#define LED_OBJ_MAX     5     //led灯最

/*LED的GPIO状态*/
typedef enum {
	LED_STATE_OFF = 0,     // 灭
    LED_STATE_ON,          // 亮
}VD_FML_LED_STATE_E;

/*LED 灯状态*/
typedef enum {
	DRV_LED_OFF = 0,    //灭
    DRV_LED_ON,         //亮
	DRV_LED_FLICK,      //闪烁
	DRV_LED_BREATHE,    //呼吸
	
}VD_FML_LED_MODE_E;

/**/
typedef struct {
    vd_ulong_t  bright_time;  // 一次闪烁或者呼吸时亮的时间
    vd_ulong_t  dark_time;    // 一次闪烁或者呼吸时灭的时间
    vd_int16_t  mode_count;   // 闪烁或呼吸的次数
	
} vd_fml_per_led_param_t;

/*led初始化*/
typedef struct {
	GPIO_PORT_E 	    gpio_port;            //自定义引脚号
	GPIO_STATE_E        led_gpio_init_state;  //led引脚初始状态  	
	vd_uint32_t         breath_speed;         //呼吸速度0
	vd_uint32_t         flick_time_ms;        //闪烁时间间隔
	
} vd_fml_per_led_init_t;

/*led对象*/
typedef struct {
	vd_fml_per_led_init_t  led_init;              //led初始化结构体
	vd_uint16_t			   cur_pwm_duty;          //当前占空比
	vd_uint16_t			   pwm_resolution;        //pwm分辨率
	vd_int32_t             pwm_obj_id;            //pwm对象默认传-1，初始pwm会更新
    VD_FML_LED_MODE_E      led_mode;              //led工作模式
    vd_uint32_t            work_time_s;           //led工作时间, 无需初始化
	vd_uint32_t            curr_sys_time_ms;      //led当前工作时间
	vd_uint32_t            curr_work_time_s;      //led当前工作时间
    VD_FML_LED_STATE_E     curr_state;            //当前led是亮还是灭

} vd_fml_per_led_t;

vd_int32_t vd_fml_per_led_init(vd_fml_per_led_init_t* led, vd_pwm_t* led_pwm);
vd_void_t vd_fml_per_led_loop(vd_void_t);
vd_bool_t vd_fml_per_led_breathe(vd_uint8_t led_id, vd_uint32_t* breathe_time_s);
vd_bool_t vd_fml_per_led_flick(vd_uint8_t led_id, vd_uint32_t* filck_time_s);
vd_bool_t vd_fml_per_led_off(vd_uint8_t led_id);
vd_bool_t vd_fml_per_led_on(vd_uint8_t led_id);
#endif
