#ifndef _VD_BSP_PWM_H
#define _VD_BSP_PWM_H

#include "base.h"
#include "vd_bsp_type.h"

//pwm参数
typedef struct{
    vd_bsp_port_e 				id;
	//以下参数通过配置实现，平台不需要的参数可赋空,由conf下配置
		vd_uint8_t 						module;				   	//模块/控制器的id
    vd_bsp_port_e 				pin;              //使用的pin脚
		vd_uint8_t 						channel;          //使用的通道号
}vd_bsp_pwm_t;

/**
 * @brief pwm 回调函数对象
 * 
 */
typedef struct 
{
    vd_bool_t (*init)(vd_bsp_pwm_t *pwm, vd_uint32_t pwm_frequency, vd_uint32_t pwm_duty); 
    vd_bool_t (*start)(vd_bsp_pwm_t *pwm);
    vd_bool_t (*stop)(vd_bsp_pwm_t *pwm);
		vd_bool_t (*set)(vd_bsp_pwm_t *pwm, vd_uint32_t pwm_frequency, vd_uint32_t pwm_duty);
}vd_pwm_ops_t;


typedef vd_bool_t (*vd_pwm_register_t)(vd_pwm_ops_t *ops);



vd_int32_t vd_bsp_pwm_init(vd_bsp_port_e pwm_id, vd_uint32_t pwm_frequency, vd_uint32_t pwm_duty);
vd_int32_t vd_bsp_pwm_start(vd_bsp_port_e pwm_id);
vd_int32_t vd_bsp_pwm_stop(vd_bsp_port_e pwm_id);
vd_int32_t vd_bsp_pwm_set(vd_bsp_port_e pwm_id, vd_uint32_t pwm_frequency, vd_uint32_t pwm_duty);

#endif // !_VD_BSP_PWM_H