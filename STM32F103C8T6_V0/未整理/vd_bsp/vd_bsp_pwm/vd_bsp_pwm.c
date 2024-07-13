#include "vd_bsp_pwm/vd_bsp_pwm.h"
#include "platform_config.h"
#include "vd_log.h"

#ifndef PLATFORM_PWM_REGISTER
#define PLATFORM_PWM_REGISTER vd_null
#endif

typedef struct{
	vd_pwm_ops_t ops;
	vd_pwm_register_t ops_register;
}vd_pwm_drv_t;

vd_pwm_drv_t pwm = {
	.ops_register = PLATFORM_PWM_REGISTER,
};

/**
 * @brief pwm 对象归纳
 * 
 */
static vd_bsp_pwm_t g_vd_bsp_pwm[] = {
#if ( PLATFORM_PWM0_EN == ON || PLATFORM_PWM1_EN == ON || PLATFORM_PWM2_EN == ON || PLATFORM_PWM3_EN == ON || \
    PLATFORM_PWM4_EN == ON ||PLATFORM_PWM5_EN == ON ||PLATFORM_PWM6_EN == ON)
#if (PLATFORM_PWM0_EN == ON)
    {VD_PORT_PWM0, PLATFORM_PWM0_MODULE, PLATFORM_PWM0_PIN, PLATFORM_PWM0_CHANNEL},
#endif
#if (PLATFORM_PWM1_EN == ON)
    {VD_PORT_PWM1, PLATFORM_PWM1_MODULE, PLATFORM_PWM1_PIN, PLATFORM_PWM1_CHANNEL},
#endif
#if (PLATFORM_PWM2_EN == ON)
    {VD_PORT_PWM2, PLATFORM_PWM2_MODULE, PLATFORM_PWM2_PIN, PLATFORM_PWM2_CHANNEL},
#endif
#if (PLATFORM_PWM3_EN == ON)
    {VD_PORT_PWM3, PLATFORM_PWM3_MODULE, PLATFORM_PWM3_PIN, PLATFORM_PWM3_CHANNEL},
#endif
#if (PLATFORM_PWM4_EN == ON)
    {VD_PORT_PWM4, PLATFORM_PWM4_MODULE, PLATFORM_PWM4_PIN, PLATFORM_PWM4_CHANNEL},
#endif
#if (PLATFORM_PWM5_EN == ON)
    {VD_PORT_PWM5, PLATFORM_PWM5_MODULE, PLATFORM_PWM5_PIN, PLATFORM_PWM5_CHANNEL},
#endif
#if (PLATFORM_PWM6_EN == ON)
    {VD_PORT_PWM6, PLATFORM_PWM6_MODULE, PLATFORM_PWM6_PIN, PLATFORM_PWM6_CHANNEL},
#endif
#if (PLATFORM_PWM7_EN == ON)
    {VD_PORT_PWM7, PLATFORM_PWM7_MODULE, PLATFORM_PWM7_PIN, PLATFORM_PWM7_CHANNEL},
#endif
#else
    {vd_null, vd_null},
#endif
};


/**
 * @brief find pwm port id
 * 
 * @param pwm_id 
 * @return ** vd_bsp_pwm_t* 
 */

static vd_bsp_pwm_t* vd_bsp_pwm_find(vd_bsp_port_e pwm_id) 
{
    for (int i = 0; i < vd_array_size(g_vd_bsp_pwm); i++) {
        if (g_vd_bsp_pwm[i].id == pwm_id) {
            return &g_vd_bsp_pwm[i];
        }
    }
    return vd_null;
}


/**
 * @brief pwm init
 * 
 * @param vd_bsp_pwm_mode 模式 
 * @param vd_pwm_port 端口pin
 * @param vd_bsp_pwm_cfg 配置项
 * @return ** vd_bool_t 
 */
vd_int32_t vd_bsp_pwm_init(vd_bsp_port_e pwm_id, vd_uint32_t pwm_frequency, vd_uint32_t pwm_duty)
{
	vd_check_return_val(pwm_id < 0 || pwm_id >= VD_PORT_PWM_MAX, -VD_BSP_ERR_PORT_INVALID);
    vd_bool_t ret = vd_false;
	vd_bsp_pwm_t *pwm_obj;
	pwm_obj = vd_bsp_pwm_find(pwm_id);
	
	vd_check_return_val(pwm.ops_register == vd_null, -VD_BSP_ERR_REGISTER);
	ret = pwm.ops_register(&pwm.ops);
	vd_check_return_val(ret == vd_false, -VD_BSP_ERR_REGISTER);
	
	vd_check_return_val(pwm.ops.init == vd_null, -VD_BSP_ERR_SDK);
	ret = pwm.ops.init(pwm_obj, pwm_frequency, pwm_duty);
	vd_check_return_val(ret == vd_false, -VD_BSP_ERR_SDK);
	
	return VD_BSP_NOERROR;
}

/**
 * @brief pwm start
 * 
 * @param vd_pwm_port 端口pin
 * @return ** vd_bool_t 
 */
vd_int32_t vd_bsp_pwm_start(vd_bsp_port_e pwm_id)
{
	vd_check_return_val(pwm_id < 0 || pwm_id >= VD_PORT_PWM_MAX, -VD_BSP_ERR_PORT_INVALID);
	vd_bool_t ret = vd_false;
	vd_bsp_pwm_t *vd_pwm_t;
	vd_pwm_t = vd_bsp_pwm_find(pwm_id);
    
	vd_check_return_val(pwm.ops.start == vd_null, -VD_BSP_ERR_SDK);
	ret = pwm.ops.start(vd_pwm_t);
	vd_check_return_val(ret == vd_false, -VD_BSP_ERR_SDK);

	return VD_BSP_NOERROR;
}


/**
 * @brief pwm stop
 * 
 * @param vd_pwm_port 端口pin
 * @return ** vd_bool_t 
 */
vd_int32_t vd_bsp_pwm_stop(vd_bsp_port_e pwm_id)
{
	vd_check_return_val(pwm_id < 0 || pwm_id >= VD_PORT_PWM_MAX, -VD_BSP_ERR_PORT_INVALID);
	vd_bool_t ret = vd_false;
	vd_bsp_pwm_t *vd_pwm_t;
	vd_pwm_t = vd_bsp_pwm_find(pwm_id);
	
	vd_check_return_val(pwm.ops.stop == vd_null, -VD_BSP_ERR_SDK);
	ret = pwm.ops.stop(vd_pwm_t);
	vd_check_return_val(ret == vd_false, -VD_BSP_ERR_SDK);
	
	return VD_BSP_NOERROR;
}

/**
 * @brief 设置pwm参数
 * 
 * @param vd_pwm_port 端口pin
 * @return vd_int32_t 
 */
vd_int32_t vd_bsp_pwm_set(vd_bsp_port_e pwm_id, vd_uint32_t pwm_frequency, vd_uint32_t pwm_duty)
{
	vd_bool_t ret = vd_false;
	
	vd_check_return_val(pwm_id < 0 || pwm_id >= VD_PORT_PWM_MAX, -VD_BSP_ERR_PORT_INVALID);
  
	vd_bsp_pwm_t *pwm_obj;
	pwm_obj = vd_bsp_pwm_find(pwm_id);
	
	vd_check_return_val(pwm.ops.set == vd_null, -VD_BSP_ERR_SDK);
	ret = pwm.ops.set(pwm_obj, pwm_frequency, pwm_duty);
	vd_check_return_val(ret == vd_false, -VD_BSP_ERR_SDK);
	
	return VD_BSP_NOERROR;
}
