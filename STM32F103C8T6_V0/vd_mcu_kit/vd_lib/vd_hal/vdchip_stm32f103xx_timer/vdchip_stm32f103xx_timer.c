//#include "vdchip_stm32f103xx_timer.h"
//#include "vd_bsp_type.h"
//#include "base_type.h"
//#include "base_check.h"
//#include "log.h"




//vdchip_stm32f103xx_timer_t timerx[TIMERx_DRIVER_OBJ_MAX] = {0};    /* timer驱动对象 */

//vd_uint32_t vd_g_register_timer_period[3] = {0};                   //已注册定时器周期

///* ======================================================================================
// * declaration
// */
///*  平台接口  */
//static TIMERx_OBJ_ID_E vdchip_stm32f103xx_timer_init(vd_bsp_timer_t *tim, vd_uint32_t timer_ms, void (*tim_irq_callback)(void *), void *arg);
//static vd_bool_t vdchip_stm32f103xx_timer_open(TIMERx_OBJ_ID_E obj_id);
//static vd_bool_t vdchip_stm32f103xx_timer_close(TIMERx_OBJ_ID_E obj_id);


///* ======================================================================================
// * helper
// */
///**
// * @brief timer平台注册
// *
// * @param ops		timer平台接口
// * @return vd_void_t			注册结果
// */
//vd_bool_t vdchip_stm32f103xx_timer_register(timer_ops_t *ops)
//{
//	vd_check_return_val_error(ops == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
//	
//	/* 接口注册 */
//	ops->init = vdchip_stm32f103xx_timer_init;
//	ops->open = vdchip_stm32f103xx_timer_open;
//	ops->close = vdchip_stm32f103xx_timer_close;
//	
//	return vd_true;
//}
// 
//static TIMERx_OBJ_ID_E vdchip_stm32f103xx_find_timer_obj_id(vd_bsp_port_e timer_id)
//{
//	int index = 0;
//	vd_int8_t obj_len = 0;
//	TIMERx_OBJ_ID_E obj_id = VD_BSP_TIMERx_OBJ_NANE;
//	
//	obj_len = TIMERx_DRIVER_OBJ_MAX / TIMERx_COUNT;
//	
//	switch(timer_id){
//		case VD_PORT_TIM1:	
//			for(index = 0; index < obj_len; index++){
//				if(timerx[index].irq_callback == vd_null){
//					obj_id = (TIMERx_OBJ_ID_E)index;
//					break;
//				}
//			}
//			break;
//		case VD_PORT_TIM2:
//			for(index = obj_len; index < obj_len * 2; index++){
//				if(timerx[index].irq_callback == vd_null){
//					obj_id = (TIMERx_OBJ_ID_E)index;
//					break;
//				}
//			}
//			break;
//		default:
//			break;	
//	}
//	return obj_id;
//}


///* ======================================================================================
// * private implementation
// */
///**
// * @brief timer初始化
// *
// * @param tim  					timer参数	
// * @return vd_bool_t		    初始化结果
// */
//static TIMERx_OBJ_ID_E vdchip_stm32f103xx_timer_init(vd_bsp_timer_t *tim, vd_uint32_t timer_ms, void (*tim_irq_callback)(void *), void *arg)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_TypeDef * TIMx = NULL;
//	TIMERx_OBJ_ID_E obj_id = VD_BSP_TIMERx_OBJ_NANE;
//	vd_uint32_t timer_period = 0;
//	IRQn_Type irq;
//		
//	switch(tim->timer_id){
//		case VD_PORT_TIM1:		
//			if(vd_g_register_timer_period[1] == 0){
//				vd_g_register_timer_period[1] = tim->period;
//			}
//			irq = TIM1_UP_IRQn;
//			TIMx = TIM1;
//			timer_period = vd_g_register_timer_period[1];
//			obj_id = vdchip_stm32f103xx_find_timer_obj_id(tim->timer_id);
//			log_info("vdchip_stm32f103xx_find_timer_obj_id = %d \r\n",obj_id);
//			vd_check_return_val_error(obj_id == VD_BSP_TIMERx_OBJ_NANE, obj_id, "in func:%s, timer1,get obj id fail!\r\n", __func__);
//			/*使能定时器总线时钟*/
//			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	
//			break;
//			
//		case VD_PORT_TIM2:			
//			if(vd_g_register_timer_period[2] == 0){
//				vd_g_register_timer_period[2] = tim->period;
//			}
//			irq = TIM2_IRQn;
//			TIMx = TIM2;
//			timer_period = vd_g_register_timer_period[2];  //us
//			obj_id = vdchip_stm32f103xx_find_timer_obj_id(tim->timer_id);
//			vd_check_return_val_error(obj_id == VD_BSP_TIMERx_OBJ_NANE, obj_id, "in func:%s, timer2,get obj id fail!\r\n", __func__);
//			/*使能定时器总线时钟*/
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 		
//			break;
//			
//		default:
//			break;		
//	}
//	timerx[obj_id].arg = arg;
//	timerx[obj_id].irq_callback = tim_irq_callback;
//	vd_check_return_val_error(timer_ms < timer_period, VD_BSP_TIMERx_OBJ_NANE, "in func:%s, timer_ms < %d!\r\n", __func__,timer_period);
//	timerx[obj_id].timer_count = timer_ms * 1000 / timer_period;
//	log_info("timerx[%d],timer_count=%d,timer_period=%d \r\n",obj_id, timerx[obj_id].timer_count, timer_period);
//	timerx[obj_id].timer_curcount = 0;
//	timerx[obj_id].timer_enable = DISABLE;
//	//时钟配置:T = (自动装载周期的值 + 1)*(预分频系数 + 1) / 72MHz,   (SystemCoreClock / 1000000 - 1);   //1us
//	TIM_InternalClockConfig(TIMx); 
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //一分频
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数
//	TIM_TimeBaseInitStructure.TIM_Period = tim->period - 1;
//	TIM_TimeBaseInitStructure.TIM_Prescaler = (SystemCoreClock / 1000000 - 1);   //1us
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
//	TIM_ClearFlag(TIMx, TIM_FLAG_Update);
//	/*中断配置*/
//	TIM_ITConfig(TIMx, TIM_IT_Update | TIM_IT_Trigger, ENABLE);
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
//	NVIC_InitStructure.NVIC_IRQChannel = irq;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&NVIC_InitStructure);
//	TIM_Cmd(TIMx, ENABLE);
//	
//	timerx[obj_id].auto_load = tim->auto_load;
//	
//	
//	log_info("vdchip_stm32f103xx_timer_init succeed!,%d\r\n, irq=%d",obj_id, irq);
//	
//	return obj_id;
//}

///**
// * @brief timer打开
// *
// * @param tim  			    timer参数	
// * @return vd_bool_t		打开结果
// */
//static vd_bool_t vdchip_stm32f103xx_timer_open(TIMERx_OBJ_ID_E obj_id)
//{
//	vd_bool_t ret_value = vd_true;
//	vd_uint8_t index = 0, obj_len = 0;
//	
//	vd_check_return_val_error((obj_id > VD_BSP_TIMER2_19_OBJ) && (obj_id < VD_BSP_TIMER1_OBJ_ALL) , vd_false, "in func:%s, erro_obj_id=%d, open fail!", __func__,obj_id);
//	
//	obj_len = TIMERx_DRIVER_OBJ_MAX / TIMERx_COUNT;
//	
//	if(obj_id == VD_BSP_TIMER1_OBJ_ALL){
//		for(index = 0; index < obj_len; index++){
//			if(timerx[index].irq_callback != vd_null){
//				timerx[index].timer_enable = ENABLE;
//				timerx[index].timer_curcount = 0;	
//			}
//		}
//	}else if(obj_id == VD_BSP_TIMER2_OBJ_ALL){
//		for(index = obj_len; index < obj_len*2; index++){
//			if(timerx[index].irq_callback != vd_null){
//				timerx[index].timer_enable = ENABLE;
//				timerx[index].timer_curcount = 0;	
//			}
//		}	
//	}else if(obj_id == VD_BSP_TIMERx_OBJ_NANE){
//		ret_value = vd_false;
//		goto erro;
//	}
//		
//	if(timerx[obj_id].timer_enable != ENABLE){
//		timerx[obj_id].timer_enable = ENABLE;
//		timerx[obj_id].timer_curcount = 0;	
//	}
//	
//erro:	
//  return ret_value;
//}

///**
// * @brief timer关闭
// *
// * @param tim  			    timer参数		
// * @return vd_bool_t		关闭结果
// */
//static vd_bool_t vdchip_stm32f103xx_timer_close(TIMERx_OBJ_ID_E obj_id)
//{
//	vd_bool_t ret_value = vd_true;
//	vd_uint8_t index = 0, obj_len = 0;
//	
//	vd_check_return_val_error((obj_id > VD_BSP_TIMER2_19_OBJ) && (obj_id < VD_BSP_TIMER1_OBJ_ALL) , vd_false, "in func:%s, erro_obj_id=%d, open fail!", __func__,obj_id);
//	
//	obj_len = TIMERx_DRIVER_OBJ_MAX / TIMERx_COUNT;
//	
//	if(obj_id == VD_BSP_TIMER1_OBJ_ALL){
//		for(index = 0; index < obj_len; index++){
//			if(timerx[index].irq_callback != vd_null){
//				timerx[index].timer_enable = DISABLE;
//				timerx[index].timer_curcount = 0;	
//			}
//		}
//	}else if(obj_id == VD_BSP_TIMER2_OBJ_ALL){
//		for(index = obj_len; index < obj_len*2; index++){
//			if(timerx[index].irq_callback != vd_null){
//				timerx[index].timer_enable = DISABLE;
//				timerx[index].timer_curcount = 0;	
//			}
//		}	
//	}else if(obj_id == VD_BSP_TIMERx_OBJ_NANE){
//		ret_value = vd_false;
//		goto erro;
//	}
//		
//	if(timerx[obj_id].timer_enable != DISABLE){
//		timerx[obj_id].timer_enable = DISABLE;
//		timerx[obj_id].timer_curcount = 0;	
//	}
//	
//erro:	
//  return ret_value;
//}


///*************************************** 中断处理 ***********************************************/

////定时器1中断处理函数
//void time1_irq_handler(void)
//{
//	vd_uint8_t index = VD_BSP_TIMER1_0_OBJ;
//	
//	for(index = VD_BSP_TIMER1_0_OBJ; index <= VD_BSP_TIMER1_3_OBJ; index++){
//		if(timerx[index].timer_enable == ENABLE){
//			//log_info("!!!!timerx[%d].timer_enable == ENABLE , curcount=%d, count=%d\r\n",index,timerx[index].timer_curcount,timerx[index].timer_count);			
//			timerx[index].timer_curcount++;			
//			if(timerx[index].timer_curcount >= timerx[index].timer_count){
//				log_info("timerx[%d].timer_enable == ENABLE , curcount=%d, count=%d\r\n",index,timerx[index].timer_curcount,timerx[index].timer_count);
//				timerx[index].timer_curcount = 0;
//				timerx[index].irq_callback(timerx[index].arg);
//			}
//		}	
//	}
//}
////定时器2中断处理函数
//void time2_irq_handler(void)
//{
//	vd_uint8_t index = VD_BSP_TIMER2_10_OBJ;
//	
//	for(index = VD_BSP_TIMER2_10_OBJ; index <= VD_BSP_TIMER2_19_OBJ; index++){
//		if(timerx[index].timer_enable == ENABLE){
//			timerx[index].timer_curcount++;
//			if(timerx[index].timer_curcount >= timerx[index].timer_count){
//				timerx[index].timer_curcount = 0;
//				timerx[index].irq_callback(timerx[index].arg);
//			}
//		}	
//	}
//}


//void TIM2_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		time2_irq_handler();
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//}


//void TIM1_UP_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
//	{
//		time1_irq_handler();
//		//log_info("TIM1_UP_IRQHandler\r\n");	
//		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//	}
//}



