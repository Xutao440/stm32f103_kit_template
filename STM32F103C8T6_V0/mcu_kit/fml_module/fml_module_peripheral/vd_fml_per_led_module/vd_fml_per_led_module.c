#include "vd_fml_per_led_module.h"
#include "vd_bsp_systick.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"


vd_fml_per_led_t  leds_obj_arry[LED_OBJ_MAX] = {0};    //LED 对象数组
vd_uint16_t       leds_obj_arry_mask = 0;


/**************************************************************************
*函数名称：vd_led_on
*函数功能：内部函数led开,兼容pwm模式
*输入参数：led_id：led的id号
*返回参数：无
***************************************************************************/
static void vd_led_on(vd_uint8_t led_id)
{
	vd_fml_per_led_t* leds = vd_null;
	
	leds = &leds_obj_arry[led_id]; 

	if(leds->pwm_obj_id != -1){   //pwm模式
		vd_bsp_pwm_fake_gpio_set_value(leds->pwm_obj_id, leds->led_init.led_gpio_init_state ? VD_GPIO_LOW : VD_GPIO_HIGH);
	}else{
		vd_bsp_gpio_write(leds->led_init.gpio_port, leds->led_init.led_gpio_init_state ? VD_GPIO_LOW : VD_GPIO_HIGH);
	}
}

/**************************************************************************
*函数名称：vd_led_off
*函数功能：内部函数led关,兼容pwm模式
*输入参数：led_id：led的id号
*返回参数：无
***************************************************************************/
static void vd_led_off(vd_uint8_t led_id)
{
	vd_fml_per_led_t* leds = vd_null;
	
	leds = &leds_obj_arry[led_id]; 

	if(leds->pwm_obj_id != -1){   //pwm模式
		vd_bsp_pwm_fake_gpio_set_value(leds->pwm_obj_id, leds->led_init.led_gpio_init_state ? VD_GPIO_HIGH : VD_GPIO_LOW);
	}else{
		vd_bsp_gpio_write(leds->led_init.gpio_port, leds->led_init.led_gpio_init_state ? VD_GPIO_HIGH : VD_GPIO_LOW);
	}
}


/**************************************************************************
*函数名称：vd_fml_led_init
*函数功能：led灯初始化
*输入参数：led: led灯对象
*返回参数：成功返回led对象id
***************************************************************************/
vd_int32_t vd_fml_per_led_init(vd_fml_per_led_init_t* led, vd_pwm_t* led_pwm)
{
	vd_int32_t ret_value = VD_BSP_NOERROR;
	vd_int32_t   pwm_obj_id = -1, index = 0;           
	vd_fml_per_led_t* leds = vd_null;
	
	vd_check_return_val_error(led == NULL, -VD_BSP_ERR_PARAMETER, "[ %s ]: parameter error \r\n",__FUNCTION__ );
	leds = leds_obj_arry; 
	
	/*GPIO初始化*/
	log_info("vd_fml_per_led_init: led->gpio_port=%d\r\n",led->gpio_port);
	ret_value = vd_bsp_gpio_init(led->gpio_port, VD_GPIO_MODE_OUT_PP);  //默认推挽输出
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_SDK, "[ %s ]: bsp gpio init fail! \r\n",__FUNCTION__ );
	
	/*呼吸功能*/
	if(led_pwm != NULL){
		ret_value = vd_bsp_pwm_init(led_pwm);
		vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: bsp pwm init fail! \r\n",__FUNCTION__ );
		pwm_obj_id = ret_value; //保存pwm对象id
		log_info("vd_fml_per_led_init: pwm_obj_id=%d\r\n",pwm_obj_id);
	}
	
	/*注册到数组*/
	for(index = 0; index < LED_OBJ_MAX; index++){	
		if(((leds_obj_arry_mask >> index) & 0x01) == 0){
			leds_obj_arry_mask |= 0x0001 << index;
			log_info("leds_obj_arry_mask=%d, index=%d\r\n",leds_obj_arry_mask, index);
			/*自定义参数*/
			leds[index].led_init.gpio_port = led->gpio_port;                      //自定义引脚号
			leds[index].led_init.led_gpio_init_state = led->led_gpio_init_state;  //led引脚初始状态 
			leds[index].led_init.breath_speed = led->breath_speed;                //呼吸速度
			leds[index].led_init.flick_time_ms = led->flick_time_ms;              //闪烁时间间隔
			
			/*默认参数*/
			leds[index].pwm_obj_id = pwm_obj_id;                                  //pwm对象id
			if(led_pwm != NULL){
				leds[index].pwm_resolution = led_pwm->period;                     //pwm分辨率
			}
			leds[index].cur_pwm_duty = 0;                                         //当前占空比
			leds[index].led_mode = DRV_LED_OFF;		                              //led关闭	
			leds[index].work_time_s = 0xffffffff;		                          //led工作时间
			leds[index].curr_sys_time_ms = 0;                                     //led时间片
			leds[index].curr_work_time_s = 0;                                     //led已工作时间
			leds[index].curr_state = LED_STATE_OFF;                                //当前led状态
			ret_value = index;		
			break;
		}		
	}
	
	vd_check_return_val_error(index >= LED_OBJ_MAX, -VD_BSP_ERR_SDK, "[ %s ]: leds obj arry is full\r\n",__FUNCTION__ );
	
	return ret_value;
}


/**************************************************************************
*函数名称：vd_fml_per_led_on
*函数功能：led亮
*输入参数：led_id： led灯对象id号
*返回参数：成功返回vd_ture, 失败返回vd_false
***************************************************************************/
vd_bool_t vd_fml_per_led_on(vd_uint8_t led_id)
{
	vd_int32_t ret_value = -1;
	vd_fml_per_led_t* leds = vd_null;
	
	vd_check_return_val_error(led_id > LED_OBJ_MAX, vd_false, "[ %s ]: parameter error \r\n",__FUNCTION__ );
	leds = leds_obj_arry;
	if(leds[led_id].led_mode != DRV_LED_ON){
		leds[led_id].led_mode = DRV_LED_ON;
		leds[led_id].curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
		leds[led_id].work_time_s = 0xffffffff;
		leds[led_id].curr_work_time_s = vd_bsp_get_now_time(TICK_TYEP_S);	
	}
	
	return vd_true;
}

/**************************************************************************
*函数名称：vd_fml_per_led_off
*函数功能：led灭
*输入参数：led_id： led灯对象id号
*返回参数：成功返回vd_ture, 失败返回vd_false
***************************************************************************/
vd_bool_t vd_fml_per_led_off(vd_uint8_t led_id)
{
	vd_int32_t ret_value = -1;
	vd_fml_per_led_t* leds = vd_null;
	
	vd_check_return_val_error(led_id > LED_OBJ_MAX, vd_false, "[ %s ]: parameter error \r\n",__FUNCTION__ );
	leds = leds_obj_arry;
	if(leds[led_id].led_mode != DRV_LED_OFF){
		leds[led_id].led_mode = DRV_LED_OFF;
		leds[led_id].curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
		leds[led_id].work_time_s = 0xffffffff;
		leds[led_id].curr_work_time_s = vd_bsp_get_now_time(TICK_TYEP_S);			
	}

	return vd_true;
}


/**************************************************************************
*函数名称：vd_fml_per_led_flick
*函数功能：led闪烁
*输入参数：led_id： led灯对象id号
*返回参数：成功返回vd_ture, 失败返回vd_false
***************************************************************************/
vd_bool_t vd_fml_per_led_flick(vd_uint8_t led_id, vd_uint32_t* filck_time_s)
{
	vd_int32_t ret_value = -1;
	vd_fml_per_led_t* leds = vd_null;
	
	vd_check_return_val_error(led_id > LED_OBJ_MAX, vd_false, "[ %s ]: parameter error \r\n",__FUNCTION__ );
	leds = leds_obj_arry;
	
	if(leds[led_id].led_mode != DRV_LED_FLICK){
		leds[led_id].led_mode = DRV_LED_FLICK;
		if(filck_time_s != NULL){
			leds[led_id].work_time_s = *filck_time_s;	
		}else{
			leds[led_id].work_time_s = 0xffffffff;
		}	
		leds[led_id].curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
		leds[led_id].curr_work_time_s = vd_bsp_get_now_time(TICK_TYEP_S);
		log_info("led_flick: led_mode=%d\r\n",leds[led_id].led_mode);
		log_info("led_flick: gpio_port=%d\r\n",leds[led_id].led_init.gpio_port);
		log_info("led_flick: flick_time_ms=%d\r\n",leds[led_id].led_init.flick_time_ms);		
	}

	
	return vd_true;	
}


/**************************************************************************
*函数名称：vd_fml_per_led_breathe
*函数功能：led呼吸
*输入参数：led_id： led灯对象id号
*返回参数：成功返回vd_ture, 失败返回vd_false
***************************************************************************/
vd_bool_t vd_fml_per_led_breathe(vd_uint8_t led_id, vd_uint32_t* breathe_time_s)
{
	vd_int32_t ret_value = -1,pwm_obj_id = -1;
	vd_fml_per_led_t* leds = vd_null;
	
	vd_check_return_val_error(led_id > LED_OBJ_MAX, vd_false, "[ %s ]: parameter error \r\n",__FUNCTION__ );
	leds = leds_obj_arry;
	
	if(leds[led_id].led_mode != DRV_LED_BREATHE){
		leds[led_id].led_mode = DRV_LED_BREATHE;
		if(breathe_time_s != NULL){
			leds[led_id].work_time_s = *breathe_time_s;	
		}else{
			leds[led_id].work_time_s = 0xffffffff;
		}
		leds[led_id].curr_state = LED_STATE_OFF;                          //当前led状态
		leds[led_id].curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
		leds[led_id].curr_work_time_s = vd_bsp_get_now_time(TICK_TYEP_S);
		pwm_obj_id = leds[led_id].pwm_obj_id;
		vd_bsp_pwm_set_duty(pwm_obj_id, 0);   //占空比初始为0
	}

	return vd_true;	
}

/**************************************************************************
*函数名称：vd_fml_per_led_loop
*函数功能：led灯处理函数
*输入参数：无
*返回参数：无
***************************************************************************/
vd_void_t vd_fml_per_led_loop(vd_void_t) 
{	
    vd_uint8_t index = 0;
    vd_fml_per_led_t* leds = vd_null;
	static vd_bool_t filck_once_flag = vd_true;
	vd_uint32_t flick_time_ms = 0, breathe_time_ms = 0;
	
	while((leds_obj_arry_mask >> index) & 0x01){		
        leds = &leds_obj_arry[index]; 		
        switch (leds->led_mode) {
            case DRV_LED_OFF:{    //LED关
				vd_led_off(index);
				leds->curr_state = LED_STATE_OFF;			
                break;
			}
            case DRV_LED_ON:{     //LED开
				vd_led_on(index);
				leds->curr_state = LED_STATE_ON;			
                break;
			}
            case DRV_LED_FLICK:{  //LED闪烁		
				if(vd_bsp_time_differ(TICK_TYEP_S, leds->curr_work_time_s) <= leds->work_time_s){
						flick_time_ms = vd_bsp_time_differ(TICK_TYEP_MS, leds->curr_sys_time_ms);
						if(leds->curr_state != LED_STATE_ON){
							vd_led_on(index);
							if( flick_time_ms >= leds->led_init.flick_time_ms){
								leds->curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
								leds->curr_state = LED_STATE_ON;								
							}							
						}else if(leds->curr_state != LED_STATE_OFF){
							vd_led_off(index);
							if(flick_time_ms >= leds->led_init.flick_time_ms){
								leds->curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
								leds->curr_state = LED_STATE_OFF;								
							}
						}
				}else{					
					leds->led_mode = DRV_LED_OFF;
				}				
                break;
			}
            case DRV_LED_BREATHE:{  //LED呼吸				
				if(vd_bsp_time_differ(TICK_TYEP_S, leds->curr_work_time_s) <= leds->work_time_s){
					breathe_time_ms = vd_bsp_time_differ(TICK_TYEP_MS, leds->curr_sys_time_ms);
					if(breathe_time_ms >= leds->led_init.breath_speed){
						leds->curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
						if(leds->curr_state == LED_STATE_OFF){
							leds->cur_pwm_duty++;
							vd_bsp_pwm_set_duty(leds->pwm_obj_id, leds->cur_pwm_duty);   //占空比初始为0
							if(leds->cur_pwm_duty >= leds->pwm_resolution){
								leds->curr_state = LED_STATE_ON;
							}
						}else if(leds->curr_state == LED_STATE_ON){
							leds->cur_pwm_duty--;
							vd_bsp_pwm_set_duty(leds->pwm_obj_id, leds->cur_pwm_duty);   //占空比初始为0
							if(leds->cur_pwm_duty == 0){
								leds->curr_state = LED_STATE_OFF;
							}
						}						
					}
				}else{					
					leds->led_mode = DRV_LED_OFF;
				}									
                break;
			}
            default:
				leds->led_mode = DRV_LED_OFF;
                break;
        }
		index++;
    } 
}





