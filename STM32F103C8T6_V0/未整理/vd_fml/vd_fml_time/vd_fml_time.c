/**
 * @file vd_fml_time.c
 * @author cqj (you@domain.com)
 * @brief 对定时器进行管理，每路定时器拥有一个链表，其他模块使用定时器时直接注册
 * @version 0.1
 * @date 2023-07-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "vd_fml_time.h"
#include "project_config.h"
typedef struct vd_timer_list
{
    struct vd_timer_list 		*next;					/* 下一回调对象指针 */
	vd_uint32_t 				time_cnt; 			/* 时间计数 计数值的自减操作最小单位为bsp中定时器的定时时间*/
	vd_uint32_t 				time_arr; 			/* 时间自动重装载值 */  
	vd_void_t (*func)(vd_void_t *arg);			/* 回调函数指针 */
	vd_void_t     				*arg;						/* 回调传入参数 */
	vd_bool_t					enable;					/* 使能状态 */
}vd_timer_list_t;

typedef vd_timer_list_t* list_head_t;

typedef struct {
    vd_fml_time_init_t          init;
	list_head_t                 time_list;
}vd_fml_time_t;

vd_fml_time_t g_fml_time[] = PROJECT_TIMER_MAP;

vd_uint64_t sys_curr_time = 0;

vd_void_t vd_sys_channel0_hander(vd_void_t *arg);
vd_void_t vd_sys_channel1_hander(vd_void_t *arg);
vd_void_t vd_sys_channel2_hander(vd_void_t *arg);
vd_void_t vd_sys_channel3_hander(vd_void_t *arg);

static list_head_t *vd_time_find_channel_list(vd_fml_time_channel_e  timer_channel_id);
static vd_void_t vd_fml_time_traversal(vd_fml_time_channel_e  timer_channel_id);  
static vd_bsp_port_e vd_time_find_time(vd_fml_time_channel_e  timer_channel_id);


/**
 * @brief time 0 hander for system time base
 * 
 * @return ** vd_void_t 
 */
vd_void_t  vd_sys_channel0_hander(vd_void_t *arg)
{
    /*==========定时器0中断函数，以下保护区静止修改，请在保护区外增加需要使用到的中断操作================*/
    static vd_uint32_t i = 0;
    i++;
    if( i>=10){
        i=0;
        sys_curr_time++;
    }
    /*===================================保护区结束==========================================================*/
    /**
     * @brief 其他操作所需中断操作，请操作简单逻辑，否则会影响系统时间运行
     * 
     */
      vd_fml_time_traversal(TIME_CHANNEL_0);

}

vd_void_t vd_sys_channel1_hander(vd_void_t *arg)
{
    vd_fml_time_traversal(TIME_CHANNEL_1);
}

vd_void_t vd_sys_channel2_hander(vd_void_t *arg)
{
    vd_fml_time_traversal(TIME_CHANNEL_2);
}

vd_void_t vd_sys_channel3_hander(vd_void_t *arg)
{
    vd_fml_time_traversal(TIME_CHANNEL_3);
}


/**
 * @brief timer init
 * 
 * @return ** vd_bool_t 
 */
vd_bool_t vd_fml_timer_init(vd_void_t)
{
    for(vd_uint8_t i=0; i<vd_array_size(g_fml_time);i++){
        switch (g_fml_time[i].init.channel){
            case TIME_CHANNEL_0:
                vd_bsp_timer_init(g_fml_time[i].init.timer_port,vd_sys_channel0_hander,vd_null);
            break;
            case TIME_CHANNEL_1:
                vd_bsp_timer_init( g_fml_time[i].init.timer_port,vd_sys_channel1_hander,vd_null);
            break;
            case TIME_CHANNEL_2:
                vd_bsp_timer_init( g_fml_time[i].init.timer_port,vd_sys_channel2_hander,vd_null);
            break;
            case TIME_CHANNEL_3:
                vd_bsp_timer_init( g_fml_time[i].init.timer_port,vd_sys_channel3_hander,vd_null);
            break;
            default:
                break;
        }   
        
        vd_bsp_timer_open(g_fml_time[i].init.timer_port);   
     
    }
  return vd_true;
}

/**
 * @brief 根据通道寻找链表
 * 
 * @param timer_channel_id 
 * @return ** vd_timer_list_t* 
 */
static list_head_t *vd_time_find_channel_list(vd_fml_time_channel_e  timer_channel_id)
{
    for(vd_uint8_t i =0; i< vd_array_size(g_fml_time);i++){
        if(g_fml_time[i].init.channel == timer_channel_id ){
            return &g_fml_time[i].time_list;
        }
    }
    return vd_null;
}

/**
 * @brief find time x
 * 
 * @param timer_channel_id 
 * @return ** vd_bsp_port_e 
 */
static vd_bsp_port_e vd_time_find_time(vd_fml_time_channel_e  timer_channel_id)
{
    for(vd_uint8_t i =0; i< vd_array_size(g_fml_time);i++){
        if(g_fml_time[i].init.channel == timer_channel_id ){
            return g_fml_time[i].init.timer_port;
        }
    }
    return -1;
}


/**
 * @brief time register
 * 
 * @param timer_channel_id 
 * @param func 
 * @param arg 
 * @param time 
 * @return ** vd_bool_t 
 */
vd_bool_t vd_fml_timer_register(vd_fml_time_channel_e  timer_channel_id,vd_void_t (*func)(vd_void_t*),vd_void_t *arg,vd_uint32_t time)
{
    vd_timer_list_t *p, *data;
    list_head_t     *p_head;
   
    p_head = vd_time_find_channel_list(timer_channel_id);
    p = *p_head;

    data = (vd_timer_list_t *)PLATFORM_MALLOC(sizeof(vd_timer_list_t));		
    
	if(data == vd_null) 
    return vd_false; /* 申请失败*/
	data->time_arr = time;
	data->time_cnt = time;
	data->enable = vd_true;
	data->func = func;
	data->arg = arg;
	data->next = NULL;

	if(*p_head == vd_null){ 
		*p_head = data; 
   
	} else {
        vd_bsp_timer_close( vd_time_find_time(timer_channel_id));
		while(p->next != NULL){
			p = p->next;
		}
		p->next = data;
	}
     vd_bsp_timer_open( vd_time_find_time(timer_channel_id));
	return vd_true;
}


/**
 * @brief 
 * 
 * @param timer_channel_id 
 * @param func 
 * @return ** vd_uint8_t  0 空链表 1 删除成功 2未找到节点
 */
vd_uint8_t vd_fml_timer_delete(vd_fml_time_channel_e  timer_channel_id,vd_void_t (*func)(vd_void_t*))
{
    vd_timer_list_t *temp,*p;
    list_head_t     *p_head;
    
    p_head = vd_time_find_channel_list(timer_channel_id);
	if(*p_head == vd_null) 
    return 0;
    
    p = *p_head;
    temp = p;
    
	if((*p_head)->func == func){
		*p_head = (*p_head)->next;
		PLATFORM_FREE(temp);
		return 1;
	}
	while(p->next != vd_null){
		if((p->next)->func == func){
			temp = p->next;
			p->next = (p->next)->next;
			PLATFORM_FREE(temp);
			return 1;
		} else {
			p = p->next;
		}	
	}
	return 2; 
}

/**
 * @brief set timer time
 * 
 * @param timer_channel_id 
 * @param func 
 * @param time 
 * @return ** vd_uint8_t    0 空链表 1 设置成功 2未找到节点
 */
vd_uint8_t vd_fml_timer_Settime(vd_fml_time_channel_e  timer_channel_id,vd_void_t (*func)(vd_void_t *),vd_uint32_t time)
{
    list_head_t     *p_head;
    vd_timer_list_t *p;
    
    p_head = vd_time_find_channel_list(timer_channel_id);
	if(*p_head == vd_null) return 0;
    
    p = *p_head;
	if(p->func == func){
		p->time_arr = time;
		p->time_cnt = time;
		return 1;
	}
	while(p->next != vd_null){
		if((p->next)->func == func){
			p->next->time_arr = time;
			p->next->time_cnt= time;
			return 1;
		} else {
			p = p->next;
		}	
	}
	return 2; 
}


/**
 * @brief enable timer
 * 
 * @param timer_channel_id 
 * @param func 
 * @param enable 
 * @return ** vd_uint8_t   0 空链表 1 使能成功 2未找到节点
 */
vd_uint8_t  vd_fml_timer_Enable(vd_fml_time_channel_e  timer_channel_id,vd_void_t (*func)(vd_void_t *), vd_uint8_t enable)
{
    list_head_t     *p_head;
    vd_timer_list_t *p;
    
    p_head = vd_time_find_channel_list(timer_channel_id);
	if(*p_head == vd_null) return 0;
    
    p = *p_head;
    
	if(p->func == func ){
		p->enable = enable;
		return 1;
	}
	while(p->next != vd_null){
		if((p->next)->func == func){
			p->next->enable = enable;
			return 1;
		} else {
			p = p->next;
		}	
	}
	return 2; 
}

/**
 * @brief 遍历链表
 * 
 * @param timer_channel_id 
 * @return ** vd_void_t 
 */
static vd_void_t vd_fml_time_traversal(vd_fml_time_channel_e  timer_channel_id)  
{	
    list_head_t     *p_head;
    vd_timer_list_t *p;
    
    p_head = vd_time_find_channel_list(timer_channel_id);
    
    p = *p_head;
    while(p != vd_null)
	{
	    p->time_cnt--;
		if(p->time_cnt == 0 && p->enable == vd_true){
            if(p->func == vd_null){
                vd_fml_timer_delete(timer_channel_id,p->func);
                return;
            }
			p->func(p->arg); 
			p->time_cnt = p->time_arr;
        }
        p = p->next;
	}
		 
}


/**
 * @brief get curren system run time
 * 
 * @return ** vd_uint64_t 1ms累加
 */
vd_uint64_t vd_get_sys_curr_time(vd_void_t)
{
    return sys_curr_time;
}


