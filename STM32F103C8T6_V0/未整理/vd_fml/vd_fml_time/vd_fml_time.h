/**
 * @file vd_fml_time.h
 * @author cqj (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef VD_FML_TIME_H
#define VD_FML_TIME_H

#include "vd_bsp_timer/vd_bsp_timer.h"
#include "vd_bsp_type.h"
#include "platform_config.h"

//��ʱģ��ͨ��
typedef enum{
	TIME_CHANNEL_0,
	TIME_CHANNEL_1,
	TIME_CHANNEL_2,
	TIME_CHANNEL_3,
	TIME_CHANNEL_MAX,
}vd_fml_time_channel_e;



//��ʱģ��ͨ��ӳ��ӳ��
typedef struct{
	vd_fml_time_channel_e		channel;
	vd_bsp_port_e				timer_port;
}vd_fml_time_init_t;

vd_uint64_t vd_get_sys_curr_time(vd_void_t);

//time channel init
vd_bool_t vd_fml_timer_init(vd_void_t);

//register timer
vd_bool_t vd_fml_timer_register(vd_fml_time_channel_e  timer_channel_id,vd_void_t (*func)(vd_void_t*),vd_void_t *arg,vd_uint32_t time);

//delete timer
vd_uint8_t vd_fml_timer_delete(vd_fml_time_channel_e  timer_channel_id,vd_void_t (*func)(vd_void_t*));

//���ûص���������
vd_uint8_t vd_fml_timer_Settime(vd_fml_time_channel_e  timer_channel_id,vd_void_t (*func)(vd_void_t *),vd_uint32_t time);

//���ûص������Ƿ�ʹ�ܻص�
vd_uint8_t  vd_fml_timer_Enable(vd_fml_time_channel_e  timer_channel_id,vd_void_t (*func)(vd_void_t *), vd_uint8_t enable);

#endif // !VD_FML_TIMME_H
