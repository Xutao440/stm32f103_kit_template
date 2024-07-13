/**
 * @file vdchip40xx_50xx_timer.c
 * @author liyongshuang
 * @brief 
 * @version 0.1
 * @date 2023-06-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
/* ======================================================================================
 * includes
 */
#include "vdchip40xx_50xx_timer/vdchip40xx_50xx_timer.h"
#include "driver_timer.h"
#include "driver_system.h"
#include "vd_log.h"

/* ======================================================================================
 * types
 */
/* 定义timer驱动对象 */
typedef struct{
	struct_Timer_t 										*handle;	/* timer句柄 */
	void (*irq_callback)(void *arg);						/* timer中断回调 */
	void 															*arg;			/* 回调参数 */
	vd_bool_t													auto_load;/* 自动装载 */
}vdchip_40xx_50xx_timer_t;

vdchip_40xx_50xx_timer_t timer0 = {0};/* timer0驱动对象 */
vdchip_40xx_50xx_timer_t timer1 = {0};/* timer1驱动对象 */


/* ======================================================================================
 * declaration
 */
/*  平台接口  */
static vd_bool_t vdchip_40xx_50xx_timer_init(vd_bsp_timer_t *tim, void (*tim_irq_callback)(void *), void *arg);/* 初始化 */
static vd_bool_t vdchip_40xx_50xx_timer_open(vd_bsp_timer_t *tim);/* 打开 */
static vd_bool_t vdchip_40xx_50xx_timer_close(vd_bsp_timer_t *tim);/* 关闭 */


/* ======================================================================================
 * helper
 */
/**
 * @brief timer平台注册
 *
 * @param ops		timer平台接口
 * @return vd_void_t			注册结果
 */
vd_bool_t vdchip_40xx_50xx_timer_register(timer_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/* 接口注册 */
	ops->init = vdchip_40xx_50xx_timer_init;
	ops->open = vdchip_40xx_50xx_timer_open;
	ops->close = vdchip_40xx_50xx_timer_close;
	
	return vd_true;
}

/* ======================================================================================
 * private implementation
 */
/**
 * @brief timer初始化
 *
 * @param tim  					timer参数	
 * @return vd_bool_t		初始化结果
 */
static vd_bool_t vdchip_40xx_50xx_timer_init(vd_bsp_timer_t *tim, void (*tim_irq_callback)(void *), void *arg)
{
	vdchip_40xx_50xx_timer_t *timx = vd_null;
	IRQn_Type irq = 0;
	vd_uint8_t clk;
	
	if(tim->timer_id == VD_PORT_TIM0){
		timx = &timer0;
		timx->handle = Timer0;
		irq = TIMER0_IRQn;
	}else if(tim->timer_id == VD_PORT_TIM1){
		timx = &timer1;
		timx->handle = Timer1;
		irq = TIMER1_IRQn;
	}
	
	vd_check_return_val_error(timx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	__SYSTEM_TIMER_CLK_ENABLE();
	
	clk =  system_get_clock_config();
	
	if(clk * tim->period < 0xffffffff){
		timer_init(timx->handle, clk * tim->period, TIMER_DIV_NONE);/* 定时器初始化 */
	}else if(((clk * tim->period) >> 4) < 0xffffffff){
		timer_init(timx->handle, (clk * tim->period) >> 4, TIMER_DIV_16);/* 定时器初始化 */
	}else if(((clk * tim->period) >> 8) < 0xffffffff){
		timer_init(timx->handle, (clk * tim->period) >> 8, TIMER_DIV_256);/* 定时器初始化 */
	}else{
		log_error("tim->period overflow\r\n");
		return vd_false;
	}
	
	/* 使能中断 */
	NVIC_ClearPendingIRQ(irq);
	NVIC_SetPriority(irq, 0);
  NVIC_EnableIRQ(irq);
	
	/* 绑定回调 */
	timx->irq_callback = tim_irq_callback;
	timx->arg = arg;
	
	timx->auto_load = tim->auto_load;

  return vd_true;
}

/**
 * @brief timer打开
 *
 * @param tim  					timer参数	
 * @return vd_bool_t		打开结果
 */
static vd_bool_t vdchip_40xx_50xx_timer_open(vd_bsp_timer_t *tim)
{
	struct_Timer_t *htimx = vd_null;
	
	if(tim->timer_id == VD_PORT_TIM0){
		htimx = timer0.handle;
	}else if(tim->timer_id == VD_PORT_TIM1){
		htimx = timer1.handle;
	}

	vd_check_return_val_error(htimx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	timer_start(htimx);/* 开启定时器 */
	
  return vd_true;
}

/**
 * @brief timer关闭
 *
 * @param tim  					timer参数		
 * @return vd_bool_t		关闭结果
 */
static vd_bool_t vdchip_40xx_50xx_timer_close(vd_bsp_timer_t *tim)
{
	struct_Timer_t *htimx = vd_null;
	
	if(tim->timer_id == VD_PORT_TIM0){
		htimx = timer0.handle;
	}else if(tim->timer_id == VD_PORT_TIM1){
		htimx = timer1.handle;
	}

	vd_check_return_val_error(htimx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	timer_stop(htimx);/* 关闭定时器 */
	
  return vd_true;
}

/* ================================================== 中断服务函数 ================================================================ */
/**
 * @brief timer0中断服务函数
 *
 * @param void  		
 * @return void		
 */
__attribute__((section("ram_code"))) void timer0_handler(void)
{
	timer_int_clear(Timer0);
	
	if(timer0.irq_callback){
		timer0.irq_callback(timer0.arg);
	}
	
	if(!timer0.auto_load){
		timer_stop(timer0.handle);
	}
}

/**
 * @brief timer1中断服务函数
 *
 * @param void  		
 * @return void		
 */
__attribute__((section("ram_code"))) void timer1_handler(void)
{
	timer_int_clear(Timer1);	

	if(timer1.irq_callback){
		timer1.irq_callback(timer1.arg);
	}	
	
	if(!timer1.auto_load){
		timer_stop(timer1.handle);
	}
}


