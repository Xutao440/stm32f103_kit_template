#ifndef _VD_FML_BEEP_H_
#define _VD_FML_BEEP_H_

#include "base_type.h"
#include "vd_bsp_type.h"
#include "vd_bsp_gpio.h"
#include "vd_bsp_pwm.h"

#define BEEP_OBJ_MAX     1     //BEEP对象个数

/*蜂鸣器模式*/
typedef enum{
	VD_BEEP_NONE = 0,  //无鸣响
	VD_BEEP_SHORT,     //短鸣
	VD_BEEP_LONG,      //长鸣
	
	VD_BEEP_MAX,
}VD_BEEP_MODE;

/*蜂鸣器模式*/
typedef enum{
	VD_BEEP_OFF = 0,
	VD_BEEP_ON,
	
	VD_BEEP_STATE_MAX,
}VD_BEEP_STATE;

/*蜂鸣器初始化对象*/
typedef struct{
	GPIO_PORT_E  gpio_port;             //自定义引脚号
	GPIO_STATE_E beep_gpio_init_state;  //beep引脚初始状态
	vd_uint16_t  vd_beep_long_on_ms;    //长鸣时间
	vd_uint16_t  vd_beep_short_on_ms;   //短鸣时间
	vd_uint16_t  vd_beep_off_ms;        //关闭时间
	
}vd_fml_per_beep_init_t;

/*蜂鸣器对象*/
typedef struct {
	vd_fml_per_beep_init_t  beep_init;             //beep初始化结构体
	vd_int32_t              pwm_obj_id;            //pwm对象默认-1
	vd_uint32_t             curr_sys_time_ms;      //beep记录系统时间
	vd_uint32_t             curr_work_time_ms;     //beep当前工作剩余时间
    VD_BEEP_STATE           curr_state;            //beep当前状态
	VD_BEEP_MODE            vd_beep_mode;          //鸣响类型
	vd_uint8_t              vd_beep_count;         //蜂鸣器鸣响次数
} vd_fml_per_beep_t;


vd_int32_t vd_fml_per_beep_init(vd_fml_per_beep_init_t* beep, vd_pwm_t* beep_pwm);                 //蜂鸣器初始化函数
vd_int32_t vd_fml_pre_beep_control(vd_uint8_t beep_id, VD_BEEP_MODE beep_mode, uint8_t beep_count);//蜂鸣器控制函数函数
void vd_fml_per_beep_loop(void);                                                                   //蜂鸣器循环处理函数


#endif 
