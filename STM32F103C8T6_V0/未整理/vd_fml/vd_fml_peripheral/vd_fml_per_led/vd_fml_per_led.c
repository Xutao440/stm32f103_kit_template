/*
 * Project: mcu-kit
 * Module:led driver
 * File: vd_fml_per_led.c
 * Created Date: 2022-04-08 10:24:33
 * Author: 
 * Description: led灯驱动
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (0)

/* ======================================================================================
 * includes
 */
#include "vd_fml_per_led.h"

#include "common.h"
#include "log.h"
#include "platform_config.h"
#include "project_config.h"

#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "vd_bsp_pwm/vd_bsp_pwm.h"


#if (PROJECT_LED_EN == ON)
/* ======================================================================================
 * macros
 */
static vd_fml_per_led_t g_vd_fml_per_leds[] = PROJECT_LED_MAP;

static vd_uint32_t duty         = 0;
static vd_ulong_t currtime      = 0;
static vd_uint8_t breathe_type  = 0;
static vd_uint8_t breathe_count = 0;


/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
vd_uint8_t breathe_en = 1;
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/**
 * @brief : 通过id查找vd_fml_per_led
 *
 * @param led_id
 * @return vd_fml_per_led_t*
 */
static vd_fml_per_led_t* vd_fml_per_led_find(vd_uint8_t led_id) {
    for (vd_uint8_t i = 0; i < vd_array_size(g_vd_fml_per_leds); i++) {
        if (led_id == g_vd_fml_per_leds[i].id) {
            return &g_vd_fml_per_leds[i];
        }
    }
    return vd_null;
}



/* ======================================================================================
 * implementation
 */


/**
 * @brief : led灯常亮
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_on(vd_uint8_t led_id) {
    vd_fml_per_led_t* vd_fml_per_led = vd_fml_per_led_find(led_id);
    vd_check_return_val_error(vd_fml_per_led == vd_null, vd_false, "led_id=%d error", led_id);
		vd_bsp_gpio_init(vd_fml_per_led->pin,VD_GPIO_MODE_OUTPUT);
    vd_fml_per_led->led_mode        = DRV_LED_ON;
    vd_fml_per_led->curr_mode_count = 0;
    vd_fml_per_led->curr_time       = 0;
	//vd_fml_per_led->curr_state 		= LED_STATE_OFF;
    return vd_true;
}

/**
 * @brief : led灯常灭
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_off(vd_uint8_t led_id) {
    vd_fml_per_led_t* vd_fml_per_led = vd_fml_per_led_find(led_id);
    vd_check_return_val_error(vd_fml_per_led == vd_null, vd_false, "led_id=%d error", led_id);
    vd_fml_per_led->led_mode        = DRV_LED_OFF;
    vd_fml_per_led->curr_mode_count = 0;
    vd_fml_per_led->curr_time       = 0;
//	vd_fml_per_led->curr_state 	    = LED_STATE_ON;
    return vd_true;
}

/**
 * @brief :LED灯自定义亮起时间
 *
 * @param led_id
 * @param adjust_time
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_adjust(vd_uint8_t led_id, vd_ulong_t adjust_time) {
    vd_fml_per_led_t* vd_fml_per_led = vd_fml_per_led_find(led_id);
    vd_check_return_val_error(vd_fml_per_led == vd_null, vd_false, "led_id=%d error", led_id);
    vd_fml_per_led->led_mode          = DRV_LED_ADJUST_TIME;
    vd_fml_per_led->param.bright_time = adjust_time;
    vd_fml_per_led->curr_mode_count   = 0;
    vd_fml_per_led->curr_time         = 0;
    return vd_true;
}

/**
 * @brief :led灯常闪
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_flash(vd_uint8_t led_id) {
    vd_fml_per_led_t* vd_fml_per_led = vd_fml_per_led_find(led_id);
    vd_check_return_val_error(vd_fml_per_led == vd_null, vd_false, "led_id=%d error", led_id);
    vd_fml_per_led->led_mode        = DRV_LED_FLASH;
    vd_fml_per_led->curr_time       = 0;
    vd_fml_per_led->param.mode_count= -1;
    return vd_true;
}

/**
 * @brief :led灯闪几次
 *
 * @param led_id
 * @param flash_count
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_flash_count(vd_uint8_t led_id, vd_uint32_t flash_count) {
    vd_fml_per_led_t* vd_fml_per_led = vd_fml_per_led_find(led_id);
    vd_check_return_val_error(vd_fml_per_led == vd_null, vd_false, "led_id=%d error", led_id);
    vd_fml_per_led->led_mode            = DRV_LED_FLASH;
    vd_fml_per_led->param.mode_count    = flash_count;
    vd_fml_per_led->curr_mode_count     = 0;
    vd_fml_per_led->curr_time           = 0;
    return vd_true;
}

/**
 * @brief : led灯常呼吸
 *
 * @param led_id
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_breathe(vd_uint8_t led_id,vd_uint32_t breath_speed) {
    vd_fml_per_led_t* vd_fml_per_led = vd_fml_per_led_find(led_id);
    vd_check_return_val_error(vd_fml_per_led == vd_null, vd_false, "led_id=%d error", led_id);
		if(vd_fml_per_led->led_mode == DRV_LED_BREATHE) {
		  return vd_true;
		}
    vd_fml_per_led->led_mode        = DRV_LED_BREATHE;
    vd_fml_per_led->curr_mode_count = 0;
    vd_fml_per_led->curr_time       = 0;
		vd_fml_per_led->breath_speed    = breath_speed;
		
    #if LOG_ENABLE_DEBUG
    log_debug("vd_fml_per_led->led_mode=%d\r\n",vd_fml_per_led->led_mode);
    #endif
    return vd_true;
}

/**
 * @brief led灯呼吸几次
 *
 * @param led_id
 * @param flash_count
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_breathe_count(vd_uint8_t led_id, vd_uint32_t breathe_count) {
    vd_fml_per_led_t* vd_fml_per_led = vd_fml_per_led_find(led_id);
    vd_check_return_val_error(vd_fml_per_led == vd_null, vd_false, "led_id=%d error", led_id);
    vd_fml_per_led->led_mode         = DRV_LED_BREATHE;
    vd_fml_per_led->param.mode_count = breathe_count;
    vd_fml_per_led->curr_mode_count  = 0;
    vd_fml_per_led->curr_time        = 0;
    vd_fml_per_led->curr_state       = 0;
    #if LOG_ENABLE_DEBUG
    log_debug("vd_fml_per_led->led_mode=%d\r\n", vd_fml_per_led->led_mode);
    #endif
    return vd_true;
}


/**
 * @brief :设置led,所有参数可自定义设置
 *
 * @param led_id
 * @param mode
 * @param param
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_set(vd_uint8_t led_id, vd_uint8_t mode, vd_fml_per_led_param_t* param) {
    vd_fml_per_led_t* vd_fml_per_led = vd_fml_per_led_find(led_id);
    vd_check_return_val_error(vd_fml_per_led == vd_null, vd_false, "led_id=%d error", led_id);
    vd_fml_per_led->led_mode        = mode;
    vd_fml_per_led->curr_mode_count = 0;
    vd_fml_per_led->curr_time       = 0;
    memcpy(&vd_fml_per_led->param, param, sizeof(vd_fml_per_led_param_t));
    return vd_true;
}


/**
 * @brief LDE loop
 * 
 * @return ** vd_void_t 
 */
vd_void_t vd_fml_per_led_loop(vd_void_t) {
    vd_uint8_t i = 0;
    vd_fml_per_led_t* vd_fml_per_led = vd_null;
      
    for (i = 0; i < vd_array_size(g_vd_fml_per_leds); i++) {
        vd_fml_per_led = &g_vd_fml_per_leds[i];
        switch (vd_fml_per_led->led_mode) {

            case DRV_LED_OFF:

            //    if (vd_fml_per_led->curr_state == LED_STATE_ON) {

                    vd_fml_per_led->curr_state = LED_STATE_OFF;
                    vd_bsp_gpio_write(vd_fml_per_led->pin,vd_fml_per_led->active_high ? VD_GPIO_LOW : VD_GPIO_HIGH);
                    vd_bsp_pwm_stop(vd_fml_per_led->pwm_port);
             //   }
                break;

            case DRV_LED_ON:
            //    if (vd_fml_per_led->curr_state == LED_STATE_OFF) {
                    vd_fml_per_led->curr_state = LED_STATE_ON;
                    vd_bsp_gpio_write(vd_fml_per_led->pin,vd_fml_per_led->active_high ? VD_GPIO_HIGH : VD_GPIO_LOW);

          //      }
                break;

            case DRV_LED_ADJUST_TIME:

                if (common_time_get_current_time_ms() - vd_fml_per_led->curr_time <= vd_fml_per_led->param.bright_time) {
                    vd_bsp_gpio_write(vd_fml_per_led->pin, vd_fml_per_led->active_high ? VD_GPIO_HIGH : VD_GPIO_LOW);
                    vd_fml_per_led->curr_state = LED_STATE_ON; 

                } else {
                     vd_bsp_gpio_write(vd_fml_per_led->pin, vd_fml_per_led->active_high ? VD_GPIO_LOW : VD_GPIO_HIGH);
                     vd_fml_per_led->curr_state = LED_STATE_OFF;
                }

                break;

            case DRV_LED_FLASH:
            #if LOG_ENABLE_DEBUG
            log_debug("curr_time=%dl,curr_state=%d,mode_count=%d", vd_fml_per_led->curr_time, vd_fml_per_led->curr_state,vd_fml_per_led->curr_mode_count);
            #endif   
                if (vd_fml_per_led->curr_time == 0) {

                    vd_fml_per_led->curr_time = common_time_get_current_time_ms();

                     vd_bsp_gpio_write(vd_fml_per_led->pin, vd_fml_per_led->active_high ? VD_GPIO_HIGH: VD_GPIO_LOW);
                    vd_fml_per_led->curr_state = LED_STATE_ON;

                } else if (common_time_get_current_time_ms() - vd_fml_per_led->curr_time <= vd_fml_per_led->param.bright_time) {

                    if (vd_fml_per_led->curr_state == LED_STATE_OFF) {

                         vd_bsp_gpio_write(vd_fml_per_led->pin, vd_fml_per_led->active_high ? VD_GPIO_HIGH : VD_GPIO_LOW);
                         vd_fml_per_led->curr_state = LED_STATE_ON;

                    }

                } else if (common_time_get_current_time_ms() - vd_fml_per_led->curr_time <=vd_fml_per_led->param.bright_time + vd_fml_per_led->param.dark_time) {
                    if (vd_fml_per_led->curr_state == LED_STATE_ON) {
                        vd_bsp_gpio_write(vd_fml_per_led->pin, vd_fml_per_led->active_high ? VD_GPIO_LOW : VD_GPIO_HIGH);
                        vd_fml_per_led->curr_state = LED_STATE_OFF;
                    }
                    } else {
                        
                         if((vd_fml_per_led->param.mode_count) == -1){
                            if (++vd_fml_per_led->curr_mode_count >= vd_fml_per_led->param.mode_count) {
                            vd_fml_per_led->param.mode_count = 0;
                            vd_fml_per_led->led_mode = DRV_LED_FLASH;
                            }
                          }

                    if (vd_fml_per_led->param.mode_count) {
                        if (++vd_fml_per_led->curr_mode_count >= vd_fml_per_led->param.mode_count) {
                            vd_fml_per_led->param.mode_count = 0;
                            vd_fml_per_led->led_mode = DRV_LED_OFF;
                        }              
                
                    }
                    vd_fml_per_led->curr_time = common_time_get_current_time_ms();
                    }

                break;

            case DRV_LED_BREATHE:
            #if LOG_ENABLE_DEBUG
                log_debug("time=%d\r\n", common_time_get_current_time_ms() - currtime);
            #endif
                if (common_time_get_current_time_ms() - vd_fml_per_led->curr_time >= 20 && vd_fml_per_led->curr_state == 0) {
                    vd_fml_per_led->duty_cycle += vd_fml_per_led->breath_speed;
                    duty = vd_fml_per_led->duty_cycle;
                    vd_bsp_pwm_init(vd_fml_per_led->pwm_port,2000,duty); 
                    vd_bsp_pwm_start(vd_fml_per_led->pwm_port);
                    #if LOG_ENABLE_DEBUG
                    log_debug("1 duty=%d  state= %d\r\n", vd_fml_per_led->duty_cycle,vd_fml_per_led->curr_state);
                    #endif
                    vd_fml_per_led->curr_time = common_time_get_current_time_ms();

                    if (vd_fml_per_led->duty_cycle > 450) {
                        vd_fml_per_led->curr_state = 1;
                    }

                } else if (common_time_get_current_time_ms()-vd_fml_per_led->curr_time >= 20 && vd_fml_per_led->curr_state == 1) {
                    vd_fml_per_led->duty_cycle -= vd_fml_per_led->breath_speed;
                    duty = vd_fml_per_led->duty_cycle;
                    vd_bsp_pwm_init(vd_fml_per_led->pwm_port,2000,duty);                     
                    vd_bsp_pwm_start(vd_fml_per_led->pwm_port);
                    vd_fml_per_led->curr_time = common_time_get_current_time_ms();
                #if LOG_ENABLE_DEBUG
                log_debug("2 duty=%d\r\n  state= %d\r\n", vd_fml_per_led->duty_cycle,vd_fml_per_led->curr_state);
                #endif 
                    if (vd_fml_per_led->duty_cycle <= 20) {

                        vd_fml_per_led->curr_state = 0;

                        if (vd_fml_per_led->param.mode_count == -1) {
                            vd_fml_per_led->led_mode = DRV_LED_BREATHE;
                          
                        }else if(vd_fml_per_led->param.mode_count){
                             if (++vd_fml_per_led->curr_mode_count >= vd_fml_per_led->param.mode_count) {
                                vd_fml_per_led->param.mode_count = 0;
                                vd_fml_per_led->led_mode = DRV_LED_OFF;														
                            }

                        }

                    }

                }
							
                break;

            default:

                break;

        }

    }
    
}

/**
 * @brief : led框架初始化
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_led_init(vd_void_t)
{
    for (vd_uint8_t i = 0; i < vd_array_size(g_vd_fml_per_leds); i++) {
        if (g_vd_fml_per_leds[i].led_mode == DRV_LED_BREATHE) {
            vd_bsp_pwm_init(g_vd_fml_per_leds[i].pwm_port,2000,g_vd_fml_per_leds[i].duty_cycle);     //PWM mode
        }else{
             vd_bsp_gpio_init(g_vd_fml_per_leds[i].pin,VD_GPIO_MODE_OUTPUT); //GPIO mode
			}
  
}
    return vd_true;
}

#endif