/*
 * Project: mcu-kit
 * Module: knob driver
 * File: drv_knob.c
 * Created Date: 2022-03-31 17:18:42
 * Author: meizhen
 * Description: 旋钮驱动
 * -----
 * @Descripttion:
 * @version: 1.0版本
 * @Author: daibaoye
 * @Date: 2022-05-05 09:27:36
 * @LastEditors: daibaoye
 * @LastEditTime: 2022-05-20 10:01:53
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
#include "vd_fml_per_knob.h"

//#if (PLATFORM_KNOB_EN == ON)
#include "EncoderScan.h"
#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "system_config.h"
#include "log.h"
#include "common_time.h"
/* ======================================================================================
 * macros
 */
static vd_uint8_t knobpad_read(vd_void_t);
/* ======================================================================================
 * types
 */
static vd_uint32_t num = 0;
/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
static drv_knob_t g_drv_knobs[] = {
    {22, 23, 0},
    // {DRV_KNOB_A_PIN, DRV_KNOB_B_PIN, 0},
};
/* ======================================================================================
 * helper
 */
/**
 * @brief : knob事件上报
 *
 * @param knob
 */
vd_uint8_t knob_tt=126;
vd_void_t knob_report(knob_event_t* knob_event) {
   log_debug("knob %s\r\n", (knob_event->rotate == ROTATE_LEFT) ? "Left" : "Right");
    knob_event->ev_type  = EV_SYS_KNOB;
    knob_event->type_num = num++;
//    log_info("driver:type_num %d\r\n", knob_event->type_num);
        //增加事件过滤
    if(get_mq_queue_capacity() < 1){
  	    event_push(knob_event);
    }
}
/* ======================================================================================
 * private implementation
 */
vd_uint8_t knob_cnt = 0; 
static vd_void_t knob_pin_isr(vd_uint8_t pin, vd_uint8_t value) {
		//log_debug("inter:pin%d,val%d\n", pin, value);
    knob_event_t      knob;
    static vd_ulong_t pre_timestamp = 0;
    // log_debug("value=%d\n", value);
    if (common_time_get_current_time_ms() - pre_timestamp >= 20) {
				pre_timestamp = common_time_get_current_time_ms();
        knob.rotate = knobpad_read();
        if (knob.rotate != ROTATE_NONE) {
//					if(knob_cnt >=1){
//						knob_cnt = 0;
						knob_report(&knob);
//					}else{
//						knob_cnt++;
//					}  					
        }
    }
		if(value == 1)
		{				
//				//drv_gpio_irq_mode(pin, DRV_GPIO_MODE_EXTI_IT_FALLING);
//			   vd_bsp_gpio_irq_enable(g_drv_knobs[0].a_pin,VD_GPIO_IRQ_DISABLE);
//        vd_bsp_gpio_irq_enable(g_drv_knobs[0].b_pin,VD_GPIO_IRQ_DISABLE);
//        vd_bsp_gpio_requst_irq(g_drv_knobs[0].a_pin,VD_GPIO_IRQ_MODE_FALLING,knob_pin_isr,&g_drv_knobs[0].a_pin);
//        vd_bsp_gpio_irq_enable(g_drv_knobs[0].a_pin,VD_GPIO_IRQ_ENABLE);
//        vd_bsp_gpio_requst_irq(g_drv_knobs[0].b_pin,VD_GPIO_IRQ_MODE_FALLING,knob_pin_isr,&g_drv_knobs[0].b_pin);
//        vd_bsp_gpio_irq_enable(g_drv_knobs[0].b_pin,VD_GPIO_IRQ_ENABLE);
			   //vd_bsp_gpio_irq_enable(pin,VD_GPIO_IRQ_DISABLE);
			   vd_bsp_gpio_requst_irq(pin,VD_GPIO_IRQ_MODE_FALLING,knob_pin_isr,&pin);
			  // vd_bsp_gpio_irq_enable(pin,VD_GPIO_IRQ_ENABLE);
		} else{
//				//drv_gpio_irq_mode(pin, DRV_GPIO_MODE_EXTI_IT_RISING);
//			  vd_bsp_gpio_irq_enable(g_drv_knobs[0].a_pin,VD_GPIO_IRQ_DISABLE);
//        vd_bsp_gpio_irq_enable(g_drv_knobs[0].b_pin,VD_GPIO_IRQ_DISABLE);
//        vd_bsp_gpio_requst_irq(g_drv_knobs[0].a_pin,VD_GPIO_IRQ_MODE_RISING,knob_pin_isr,&g_drv_knobs[0].a_pin);
//        vd_bsp_gpio_irq_enable(g_drv_knobs[0].a_pin,VD_GPIO_IRQ_ENABLE);
//        vd_bsp_gpio_requst_irq(g_drv_knobs[0].b_pin,VD_GPIO_IRQ_MODE_RISING,knob_pin_isr,&g_drv_knobs[0].b_pin);
//        vd_bsp_gpio_irq_enable(g_drv_knobs[0].b_pin,VD_GPIO_IRQ_ENABLE);
				 //vd_bsp_gpio_irq_enable(pin,VD_GPIO_IRQ_DISABLE);
			   vd_bsp_gpio_requst_irq(pin,VD_GPIO_IRQ_MODE_RISING,knob_pin_isr,&pin);
			  // vd_bsp_gpio_irq_enable(pin,VD_GPIO_IRQ_ENABLE);
		}
		
}

static vd_uint8_t knobpad_read(vd_void_t) {
    vd_uint8_t i, gpioa_value, gpiob_value;
    vd_uint8_t res = 0;
    for (i = 0; i < vd_array_size(g_drv_knobs); i++) {
//        gpioa_value = drv_gpio_read(g_drv_knobs[i].a_pin);
//        gpiob_value = drv_gpio_read(g_drv_knobs[i].b_pin);
			gpioa_value=vd_bsp_gpio_read(g_drv_knobs[i].a_pin);
      gpiob_value=vd_bsp_gpio_read(g_drv_knobs[i].b_pin);
			
        res = EncoderScan(gpioa_value, gpiob_value);
        // log_debug("gpioa=%d gpiob=%d\r\n", i, gpioa_value, gpiob_value);
        if (res & (ENC_1_R | ENC_1_L)) {
				
            if (res & ENC_1_R) {
                return ROTATE_LEFT;
            } else if (res & ENC_1_L) {
                return ROTATE_RIGHT;
            }
        }
    }
    return ROTATE_NONE;
}
/* ======================================================================================
 * implementation
 */

/**
 * @brief : 旋钮主循环
 *
 * @return vd_void_t
 */
vd_void_t vd_fml_per_knob_task_handler(vd_void_t){
//vd_void_t drv_knob_loop(vd_void_t) {
    knob_event_t knob;

    knob.rotate = knobpad_read();
    if (knob.rotate != ROTATE_NONE) {
        knob_report(&knob);
    }
}

/**
 * @brief : 旋钮初始化
 *
 * @return vd_bool_t
 */
//vd_bool_t drv_knob_init(vd_void_t) {
vd_bool_t vd_fml_per_knob_init(vd_void_t){
    vd_uint8_t i;
    for (i = 0; i < vd_array_size(g_drv_knobs); i++) {
//        drv_gpio_t drv_gpio;
//        drv_gpio.pin  = g_drv_knobs[i].a_pin;
//        drv_gpio.mode = DRV_GPIO_MODE_INPUT;
//        drv_gpio.pull = DRV_GPIO_NOPULL;
//        drv_gpio_init(&drv_gpio);
//        drv_gpio_requst_irq(drv_gpio.pin, DRV_GPIO_MODE_EXTI_IT_FALLING, knob_pin_isr);

//        drv_gpio.pin  = g_drv_knobs[i].b_pin;
//        drv_gpio.mode = DRV_GPIO_MODE_INPUT;
//        drv_gpio.pull = DRV_GPIO_NOPULL;
//        drv_gpio_init(&drv_gpio);
//        drv_gpio_requst_irq(drv_gpio.pin, DRV_GPIO_MODE_EXTI_IT_FALLING, knob_pin_isr);
			
			
			      vd_bsp_gpio_irq_enable(g_drv_knobs[i].a_pin,VD_GPIO_IRQ_DISABLE);
            vd_bsp_gpio_irq_enable(g_drv_knobs[i].b_pin,VD_GPIO_IRQ_DISABLE);
            vd_bsp_gpio_requst_irq(g_drv_knobs[i].a_pin,VD_GPIO_IRQ_MODE_FALLING,knob_pin_isr,&g_drv_knobs[i].a_pin);
            vd_bsp_gpio_irq_enable(g_drv_knobs[i].a_pin,VD_GPIO_IRQ_ENABLE);
            vd_bsp_gpio_requst_irq(g_drv_knobs[i].b_pin,VD_GPIO_IRQ_MODE_FALLING,knob_pin_isr,&g_drv_knobs[i].b_pin);
            vd_bsp_gpio_irq_enable(g_drv_knobs[i].b_pin,VD_GPIO_IRQ_ENABLE);
    }
    return vd_true;
}
//#endif