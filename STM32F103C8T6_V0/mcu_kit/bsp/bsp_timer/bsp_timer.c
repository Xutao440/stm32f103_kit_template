#include "vd_bsp_timer.h"
#include "vd_bsp_config.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"

vd_timer_obj_t timerx[TIMERx_DRIVER_OBJ_MAX] = {0};    //timer驱动对象
vd_uint32_t vd_g_register_timer_period[3] = {0};       //已注册定时器周期


/*
************************************************************
*	函数名称：	vd_bsp_find_timer_obj_id
*	函数功能：	得到timer对象id号
*	入口参数：	无
*	返回参数：	无
************************************************************
*/ 
static TIMERx_OBJ_ID_E vd_bsp_find_timer_obj_id(TIMERx_PORT_E timer_id)
{
	int index = 0;
	vd_int8_t obj_len = 0;
	TIMERx_OBJ_ID_E obj_id = VD_BSP_TIMERx_OBJ_NANE;
	
	obj_len = TIMERx_DRIVER_OBJ_MAX / TIMERx_COUNT;
	
	switch(timer_id){
		case VD_PORT_TIM1:	
			for(index = 0; index < obj_len; index++){
				if(timerx[index].irq_callback == vd_null){
					obj_id = (TIMERx_OBJ_ID_E)index;
					break;
				}
			}
			break;
		case VD_PORT_TIM2:
			for(index = obj_len; index < obj_len * 2; index++){
				if(timerx[index].irq_callback == vd_null){
					obj_id = (TIMERx_OBJ_ID_E)index;
					break;
				}
			}
			break;
		default:
			break;	
	}
	return obj_id;
}


/*
************************************************************
*	函数名称：	vd_bsp_timer_init
*	函数功能：	定时器初始化
*	入口参数：	无
*	返回参数：	无
************************************************************
*/ 
TIMERx_OBJ_ID_E vd_bsp_timer_init(TIMERx_PORT_E timer_id, vd_uint32_t timer_ms, \
                                  void (*tim_irq_callback)(void *), void *arg)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TypeDef * TIMx = NULL;
	TIMERx_OBJ_ID_E obj_id = VD_BSP_TIMERx_OBJ_NANE;
	vd_uint16_t timer_period = 0;
	IRQn_Type irq;
	
	switch(timer_id){
		case VD_PORT_TIM1:		
			if(vd_g_register_timer_period[1] == 0){
				vd_g_register_timer_period[1] = TIME1_PERIOD;
			}
			irq = TIM1_UP_IRQn;
			TIMx = TIM1;
			timer_period = vd_g_register_timer_period[1];
			obj_id = vd_bsp_find_timer_obj_id(timer_id);
			log_info("vd_bsp_find_timer_obj_id = %d \r\n",obj_id);
			vd_check_return_val_error(obj_id == VD_BSP_TIMERx_OBJ_NANE, obj_id, "in func:%s, timer1,get obj id fail!\r\n", __func__);
			/*使能定时器总线时钟*/
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	
			break;
			
		case VD_PORT_TIM2:			
			if(vd_g_register_timer_period[2] == 0){
				vd_g_register_timer_period[2] = TIME2_PERIOD;
			}
			irq = TIM2_IRQn;
			TIMx = TIM2;
			timer_period = vd_g_register_timer_period[2];  //us
			obj_id = vd_bsp_find_timer_obj_id(timer_id);
			vd_check_return_val_error(obj_id == VD_BSP_TIMERx_OBJ_NANE, obj_id, "in func:%s, timer2,get obj id fail!\r\n", __func__);
			/*使能定时器总线时钟*/
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 		
			break;
			
		default:
			break;		
	}
	timerx[obj_id].arg = arg;
	timerx[obj_id].irq_callback = tim_irq_callback;
	vd_check_return_val_error(timer_ms < timer_period, VD_BSP_TIMERx_OBJ_NANE, "in func:%s, timer_ms < %d!\r\n", __func__,timer_period);
	timerx[obj_id].timer_count = timer_ms * 1000 / timer_period;
	log_info("timerx[%d],timer_count=%d,timer_period=%d \r\n",obj_id, timerx[obj_id].timer_count, timer_period);
	timerx[obj_id].timer_curcount = 0;
	timerx[obj_id].timer_enable = DISABLE;
	//时钟配置:T = (自动装载周期的值 + 1)*(预分频系数 + 1) / 72MHz,   (SystemCoreClock / 1000000 - 1);   //1us
	TIM_InternalClockConfig(TIMx); 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数
	TIM_TimeBaseInitStructure.TIM_Period = timer_period - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = (SystemCoreClock / 1000000 - 1);   //1us
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);
	/*中断配置*/
	TIM_ITConfig(TIMx, TIM_IT_Update | TIM_IT_Trigger, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_InitStructure.NVIC_IRQChannel = irq;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIMx, ENABLE);
		
	log_info("vdchip_stm32f103xx_timer_init succeed!,%d\r\n, irq=%d",obj_id, irq);
	
	return obj_id;
}


/*
************************************************************
*	函数名称：	vd_bsp_timer_open
*	函数功能：	打开对应obj定时器任务
*	入口参数：	obj_id：初始化返回的id号
*	返回参数：	成功返回0
************************************************************
*/
vd_int32_t vd_bsp_timer_open(TIMERx_OBJ_ID_E obj_id)
{
	vd_uint8_t index = 0, obj_len = 0;
	vd_int32_t ret_value = VD_BSP_NOERROR;
	
	vd_check_return_val_error((obj_id > VD_BSP_TIMER2_19_OBJ) && (obj_id < VD_BSP_TIMER1_OBJ_ALL) , \
	                          vd_false, "in func:%s, erro_obj_id=%d, open fail!", __func__,obj_id);
	
	obj_len = TIMERx_DRIVER_OBJ_MAX / TIMERx_COUNT;
	
	if(obj_id == VD_BSP_TIMER1_OBJ_ALL){
		for(index = 0; index < obj_len; index++){
			if(timerx[index].irq_callback != vd_null){
				timerx[index].timer_enable = ENABLE;
				timerx[index].timer_curcount = 0;	
			}
		}
	}else if(obj_id == VD_BSP_TIMER2_OBJ_ALL){
		for(index = obj_len; index < obj_len*2; index++){
			if(timerx[index].irq_callback != vd_null){
				timerx[index].timer_enable = ENABLE;
				timerx[index].timer_curcount = 0;	
			}
		}	
	}else if(obj_id == VD_BSP_TIMERx_OBJ_NANE){
		ret_value = -VD_BSP_ERR_SDK;
		goto erro;
	}
		
	if(timerx[obj_id].timer_enable != ENABLE){
		timerx[obj_id].timer_enable = ENABLE;
		timerx[obj_id].timer_curcount = 0;	
	}
	
erro:	
  return ret_value;
}

/*
************************************************************
*	函数名称：	vd_bsp_timer_close
*	函数功能：	关闭对应obj定时器任务
*	入口参数：	obj_id：初始化返回的id号
*	返回参数：	成功返回0
************************************************************
*/
vd_int32_t vd_bsp_timer_close(TIMERx_OBJ_ID_E obj_id)
{
	vd_int32_t ret_value = VD_BSP_NOERROR;
	vd_uint8_t index = 0, obj_len = 0;
	
	vd_check_return_val_error((obj_id > VD_BSP_TIMER2_19_OBJ) && (obj_id < VD_BSP_TIMER1_OBJ_ALL) , \
								vd_false, "in func:%s, erro_obj_id=%d, open fail!", __func__,obj_id);
	
	obj_len = TIMERx_DRIVER_OBJ_MAX / TIMERx_COUNT;
	
	if(obj_id == VD_BSP_TIMER1_OBJ_ALL){
		for(index = 0; index < obj_len; index++){
			if(timerx[index].irq_callback != vd_null){
				timerx[index].timer_enable = DISABLE;
				timerx[index].timer_curcount = 0;	
			}
		}
	}else if(obj_id == VD_BSP_TIMER2_OBJ_ALL){
		for(index = obj_len; index < obj_len*2; index++){
			if(timerx[index].irq_callback != vd_null){
				timerx[index].timer_enable = DISABLE;
				timerx[index].timer_curcount = 0;	
			}
		}	
	}else if(obj_id == VD_BSP_TIMERx_OBJ_NANE){
		ret_value = -VD_BSP_ERR_SDK;
		goto erro;
	}
		
	if(timerx[obj_id].timer_enable != DISABLE){
		timerx[obj_id].timer_enable = DISABLE;
		timerx[obj_id].timer_curcount = 0;	
	}
	
erro:	
  return ret_value;
}

/*************************************** 中断处理 ***********************************************/

//定时器1中断处理函数
void time1_irq_handler(void)
{
	vd_uint8_t index = VD_BSP_TIMER1_0_OBJ;
	
	for(index = VD_BSP_TIMER1_0_OBJ; index <= VD_BSP_TIMER1_3_OBJ; index++){
		if(timerx[index].timer_enable == ENABLE){
			//log_info("!!!!timerx[%d].timer_enable == ENABLE , curcount=%d, count=%d\r\n",index,timerx[index].timer_curcount,timerx[index].timer_count);			
			timerx[index].timer_curcount++;			
			if(timerx[index].timer_curcount >= timerx[index].timer_count){
				log_info("timerx[%d].timer_enable == ENABLE , curcount=%d, count=%d\r\n",index,timerx[index].timer_curcount,timerx[index].timer_count);
				timerx[index].timer_curcount = 0;
				timerx[index].irq_callback(timerx[index].arg);
			}
		}	
	}
}


//定时器2中断处理函数
void time2_irq_handler(void)
{
	vd_uint8_t index = VD_BSP_TIMER2_10_OBJ;
	
	for(index = VD_BSP_TIMER2_10_OBJ; index <= VD_BSP_TIMER2_19_OBJ; index++){
		if(timerx[index].timer_enable == ENABLE){
			timerx[index].timer_curcount++;
			if(timerx[index].timer_curcount >= timerx[index].timer_count){
				timerx[index].timer_curcount = 0;
				timerx[index].irq_callback(timerx[index].arg);
			}
		}	
	}
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		time2_irq_handler();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		time1_irq_handler();
		//log_info("TIM1_UP_IRQHandler\r\n");	
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}



