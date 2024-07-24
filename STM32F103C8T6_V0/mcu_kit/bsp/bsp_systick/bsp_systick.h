#ifndef _BSP_SYSTICK_H_
#define _BSP_SYSTICK_H_ 			   

#include "base_type.h"

/*定时时间类型*/
typedef enum {            
  TICK_TYEP_MS = 0,  
  TICK_TYEP_S,
	
} tick_type_e;

uint32_t bsp_time_differ(tick_type_e tick_type, uint32_t now_time);//获得时间差
uint32_t bsp_get_now_time(tick_type_e tick_type);                  //获得时间
void bsp_systick_init(uint32_t time_us);                           //系统时钟初始化

#endif

