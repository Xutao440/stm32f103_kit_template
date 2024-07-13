#ifndef _VD_BSP_PWM_H
#define _VD_BSP_PWM_H

#include "base.h"
#include "vd_bsp_type.h"

//pwm����
typedef struct{
    vd_bsp_port_e 				id;
	//���²���ͨ������ʵ�֣�ƽ̨����Ҫ�Ĳ����ɸ���,��conf������
		vd_uint8_t 						module;				   	//ģ��/��������id
    vd_bsp_port_e 				pin;              //ʹ�õ�pin��
		vd_uint8_t 						channel;          //ʹ�õ�ͨ����
}vd_bsp_pwm_t;

/**
 * @brief pwm �ص���������
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