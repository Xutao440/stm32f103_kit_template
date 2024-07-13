/*
 * Project: mcu-kit
 * Module:key driver
 * File: vd_fml_per_key.c
 * Created Date: 2022-03-28 17:17:57
 * Author: 
 * Description: 按键驱动
 * -----
 * @Author: daibaoye
 * @Date: 2022-05-05 09:27:36
 * @LastEditors: daibaoye
 * @LastEditTime: 2022-05-24 09:34:32
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
#include "vd_fml_per_key.h"

#if (PROJECT_KEY_EN == ON)
#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "vd_log.h"
#include "system_config.h"

/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
static key_info_t g_key_map[] = PROJECT_KEY_MAP;
vd_uint8_t vd_fml_per_key_init_flag = 0;
/* ======================================================================================
 * helper
 */

/**
 * @brief : key事件上报
 *
 * @param key
 */
static void key_report(key_event_t* key_event, vd_uint8_t report_flag)
 {
    #if LOG_ENABLE_DEBUG
    log_debug("0:press 1:relse 2:longpress  keycode=%d, key_flag=%d\r\n", key_event->key_code, key_event->key_flag);
    #endif 
    key_event->ev_type = EV_SYS_KEY;

    //增加事件过滤
    if(report_flag == KEY_REPORT_LIMIT){
        if(get_mq_queue_capacity() < 1 && get_mq_queue_lock() == 2){
            log_error("mq_queue is full\r\n");
	    }
    }else{
    	event_push(key_event);
    }
}

/* ======================================================================================
 * private implementation
 */
static vd_uint8_t keypad_read(void) {
    vd_uint8_t i, gpio_value;
    for (i = 0; i < vd_array_size(g_key_map); i++) {
        gpio_value = vd_bsp_gpio_read(g_key_map[i].pin);
				//低电平触发，加快中断中执行速度
//        if (g_key_map[i].active_high && gpio_value) {
//            return g_key_map[i].value;
//        } 
				if (!g_key_map[i].active_high && !gpio_value) {
            return g_key_map[i].value;
        }
    }

    return KEY_RESERVED;
}
/* ======================================================================================
 * implementation
 */

/**
 * @brief : 按键驱动轮询
 *
 * @return vd_void_t
 */
vd_void_t vd_fml_per_key_loop(vd_void_t) 
{
    key_event_t key;
//    static vd_ulong_t  pre_timestamp = 0;
//    static vd_ulong_t  report_timestamp = 0; //上报时间
//    static vd_ulong_t  cur_timestamp = 0; //当前时间
//    static key_event_t last_key;
	static bool last_state=false;

//    cur_timestamp = common_time_get_current_time_ms();
//    if (pre_timestamp == 0) {
//        pre_timestamp = cur_timestamp;
//        memset(&last_key, 0, sizeof(key_event_t));
//    }

//    if (cur_timestamp - pre_timestamp >= DRV_KEY_POLL_TIME) {
//        pre_timestamp = cur_timestamp;

//        key.key_code = keypad_read();
//        key.time     = cur_timestamp;
//    if (key.key_code != KEY_RESERVED) {
//        if (last_key.key_code == KEY_RESERVED){
//            report_timestamp = cur_timestamp;
//            key.key_flag = KEY_PRESS;  // key press
//            key_report(&key, KEY_REPORT_NONE);          // key press repeat
//      } else {
//#if (PROJECT_KEY_LONG_PRESS == ON)
//            key_report(&key, KEY_REPORT_NONE);  // key press repeat
//#endif
//    if(cur_timestamp - report_timestamp > LONG_TIME_SET){
//        report_timestamp = cur_timestamp;
//        key.key_flag = KEY_LONG_PRESS;           // key press
//        key_report(&key, KEY_REPORT_LIMIT); // key press repeat
//  
//        }
//            }
//        } else {
//            if (last_key.key_code != KEY_RESERVED) {
//                report_timestamp = cur_timestamp;
//                key.key_code = last_key.key_code;
//                key.key_flag = KEY_RELEASE;
//                key_report(&key, KEY_REPORT_NONE);  // key release
//                key.key_code = KEY_RESERVED;
//            }
//        }

//        last_key.key_code = key.key_code;
//        last_key.key_flag = key.key_flag;
//        last_key.time = key.time;
//    }
//不需要使用长按，注释掉，加快中断执行速度
	  key.key_code = keypad_read();
    if (key.key_code != KEY_RESERVED) {

						last_state=true;
            key.key_flag = KEY_PRESS;  // key press
            key_report(&key, KEY_REPORT_NONE);          // key press repeat
    }
		else {
            if (last_state==true) {
								last_state=false;
                key.key_flag = KEY_RELEASE;
                key_report(&key, KEY_REPORT_NONE);  // key release
                key.key_code = KEY_RESERVED;
						}
     }
}

/**
 * @brief : key驱动初始化
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_key_init(vd_void_t) {
    uint32_t i;
    for (i = 0; i < vd_array_size(g_key_map); i++) {
         if (g_key_map[i].active_high) {
                 
             vd_bsp_gpio_init(g_key_map[i].pin,VD_GPIO_MODE_INPUT_PULLDOWN);
         }else{
            
            vd_bsp_gpio_init(g_key_map[i].pin,VD_GPIO_MODE_INPUT_PULLUP);
         }

    }
#if LOG_ENABLE_DEBUG
    log_debug("vd_bsp_key_init success\r\n");
#endif
		vd_fml_per_key_init_flag = 1;
    return vd_true;
}

#endif