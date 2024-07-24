#ifndef _VD_BSP_TIMER_H
#define _VD_BSP_TIMER_H

#include "base_type.h"

#define  TIMERx_COUNT  2
#define  TIMERx_DRIVER_OBJ_MAX    (10*TIMERx_COUNT)     //定时器1-2,每个定时器允许最大注册10个

/*定时器端口*/
typedef enum{
//	VD_PORT_TIM0,
	VD_PORT_TIM1,
	VD_PORT_TIM2,
	VD_PORT_TIM_MAX,

}TIMERx_PORT_E;

/*定时器obj id*/
typedef enum{
	VD_BSP_TIMER1_0_OBJ = 0,        //定时器1
	VD_BSP_TIMER1_1_OBJ,            //定时器1
	VD_BSP_TIMER1_2_OBJ,            //定时器1
	VD_BSP_TIMER1_3_OBJ,            //定时器1
	VD_BSP_TIMER1_4_OBJ,            //定时器1
	VD_BSP_TIMER1_5_OBJ,            //定时器1
	VD_BSP_TIMER1_6_OBJ,            //定时器1
	VD_BSP_TIMER1_7_OBJ,            //定时器1
	VD_BSP_TIMER1_8_OBJ0,           //定时器1
	VD_BSP_TIMER1_9_OBJ,            //定时器1
	VD_BSP_TIMER2_10_OBJ,            //定时器2
	VD_BSP_TIMER2_11_OBJ,            //定时器2
	VD_BSP_TIMER2_12_OBJ,            //定时器2
	VD_BSP_TIMER2_13_OBJ,            //定时器2
	VD_BSP_TIMER2_14_OBJ,            //定时器2
	VD_BSP_TIMER2_15_OBJ,            //定时器2
	VD_BSP_TIMER2_16_OBJ,            //定时器2
	VD_BSP_TIMER2_17_OBJ,            //定时器2
	VD_BSP_TIMER2_18_OBJ0,           //定时器2
	VD_BSP_TIMER2_19_OBJ,            //定时器2
	
	VD_BSP_TIMER1_OBJ_ALL = 86,     //定时器1 MAX
	VD_BSP_TIMER2_OBJ_ALL = 87,     //定时器2 MAX	
	VD_BSP_TIMERx_OBJ_NANE = 88,    //错误的
	
}TIMERx_OBJ_ID_E;

/* timer参数结构体 */
typedef struct {
	vd_bool_t		timer_enable;	     //使能标志
	vd_uint32_t     timer_count;         //时间计数个数
	vd_uint32_t     timer_curcount;      //当前计数值
	void (*irq_callback)(void *arg);	 //timer中断回调
	void 			*arg;			     //回调参数
	
} vd_timer_obj_t;


TIMERx_OBJ_ID_E vd_bsp_timer_init(TIMERx_PORT_E timer_id, vd_uint32_t timer_ms, void (*tim_irq_callback)(void *), void *arg);  //定时器初始化函数

vd_int32_t vd_bsp_timer_open(TIMERx_OBJ_ID_E timer_obj_id); //开启定时器

vd_int32_t vd_bsp_timer_close(TIMERx_OBJ_ID_E timer_obj_id);//关闭定时器

#endif






