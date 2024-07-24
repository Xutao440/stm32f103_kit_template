#ifndef _VD_BSP_TIMER_H
#define _VD_BSP_TIMER_H

#include "base_type.h"

#define  TIMERx_COUNT  2
#define  TIMERx_DRIVER_OBJ_MAX    (10*TIMERx_COUNT)     //��ʱ��1-2,ÿ����ʱ���������ע��10��

/*��ʱ���˿�*/
typedef enum{
//	VD_PORT_TIM0,
	VD_PORT_TIM1,
	VD_PORT_TIM2,
	VD_PORT_TIM_MAX,

}TIMERx_PORT_E;

/*��ʱ��obj id*/
typedef enum{
	VD_BSP_TIMER1_0_OBJ = 0,        //��ʱ��1
	VD_BSP_TIMER1_1_OBJ,            //��ʱ��1
	VD_BSP_TIMER1_2_OBJ,            //��ʱ��1
	VD_BSP_TIMER1_3_OBJ,            //��ʱ��1
	VD_BSP_TIMER1_4_OBJ,            //��ʱ��1
	VD_BSP_TIMER1_5_OBJ,            //��ʱ��1
	VD_BSP_TIMER1_6_OBJ,            //��ʱ��1
	VD_BSP_TIMER1_7_OBJ,            //��ʱ��1
	VD_BSP_TIMER1_8_OBJ0,           //��ʱ��1
	VD_BSP_TIMER1_9_OBJ,            //��ʱ��1
	VD_BSP_TIMER2_10_OBJ,            //��ʱ��2
	VD_BSP_TIMER2_11_OBJ,            //��ʱ��2
	VD_BSP_TIMER2_12_OBJ,            //��ʱ��2
	VD_BSP_TIMER2_13_OBJ,            //��ʱ��2
	VD_BSP_TIMER2_14_OBJ,            //��ʱ��2
	VD_BSP_TIMER2_15_OBJ,            //��ʱ��2
	VD_BSP_TIMER2_16_OBJ,            //��ʱ��2
	VD_BSP_TIMER2_17_OBJ,            //��ʱ��2
	VD_BSP_TIMER2_18_OBJ0,           //��ʱ��2
	VD_BSP_TIMER2_19_OBJ,            //��ʱ��2
	
	VD_BSP_TIMER1_OBJ_ALL = 86,     //��ʱ��1 MAX
	VD_BSP_TIMER2_OBJ_ALL = 87,     //��ʱ��2 MAX	
	VD_BSP_TIMERx_OBJ_NANE = 88,    //�����
	
}TIMERx_OBJ_ID_E;

/* timer�����ṹ�� */
typedef struct {
	vd_bool_t		timer_enable;	     //ʹ�ܱ�־
	vd_uint32_t     timer_count;         //ʱ���������
	vd_uint32_t     timer_curcount;      //��ǰ����ֵ
	void (*irq_callback)(void *arg);	 //timer�жϻص�
	void 			*arg;			     //�ص�����
	
} vd_timer_obj_t;


TIMERx_OBJ_ID_E vd_bsp_timer_init(TIMERx_PORT_E timer_id, vd_uint32_t timer_ms, void (*tim_irq_callback)(void *), void *arg);  //��ʱ����ʼ������

vd_int32_t vd_bsp_timer_open(TIMERx_OBJ_ID_E timer_obj_id); //������ʱ��

vd_int32_t vd_bsp_timer_close(TIMERx_OBJ_ID_E timer_obj_id);//�رն�ʱ��

#endif






