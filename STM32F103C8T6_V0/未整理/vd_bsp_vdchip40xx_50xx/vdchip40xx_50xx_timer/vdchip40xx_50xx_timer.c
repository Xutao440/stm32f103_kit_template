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
/* ����timer�������� */
typedef struct{
	struct_Timer_t 										*handle;	/* timer��� */
	void (*irq_callback)(void *arg);						/* timer�жϻص� */
	void 															*arg;			/* �ص����� */
	vd_bool_t													auto_load;/* �Զ�װ�� */
}vdchip_40xx_50xx_timer_t;

vdchip_40xx_50xx_timer_t timer0 = {0};/* timer0�������� */
vdchip_40xx_50xx_timer_t timer1 = {0};/* timer1�������� */


/* ======================================================================================
 * declaration
 */
/*  ƽ̨�ӿ�  */
static vd_bool_t vdchip_40xx_50xx_timer_init(vd_bsp_timer_t *tim, void (*tim_irq_callback)(void *), void *arg);/* ��ʼ�� */
static vd_bool_t vdchip_40xx_50xx_timer_open(vd_bsp_timer_t *tim);/* �� */
static vd_bool_t vdchip_40xx_50xx_timer_close(vd_bsp_timer_t *tim);/* �ر� */


/* ======================================================================================
 * helper
 */
/**
 * @brief timerƽ̨ע��
 *
 * @param ops		timerƽ̨�ӿ�
 * @return vd_void_t			ע����
 */
vd_bool_t vdchip_40xx_50xx_timer_register(timer_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/* �ӿ�ע�� */
	ops->init = vdchip_40xx_50xx_timer_init;
	ops->open = vdchip_40xx_50xx_timer_open;
	ops->close = vdchip_40xx_50xx_timer_close;
	
	return vd_true;
}

/* ======================================================================================
 * private implementation
 */
/**
 * @brief timer��ʼ��
 *
 * @param tim  					timer����	
 * @return vd_bool_t		��ʼ�����
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
		timer_init(timx->handle, clk * tim->period, TIMER_DIV_NONE);/* ��ʱ����ʼ�� */
	}else if(((clk * tim->period) >> 4) < 0xffffffff){
		timer_init(timx->handle, (clk * tim->period) >> 4, TIMER_DIV_16);/* ��ʱ����ʼ�� */
	}else if(((clk * tim->period) >> 8) < 0xffffffff){
		timer_init(timx->handle, (clk * tim->period) >> 8, TIMER_DIV_256);/* ��ʱ����ʼ�� */
	}else{
		log_error("tim->period overflow\r\n");
		return vd_false;
	}
	
	/* ʹ���ж� */
	NVIC_ClearPendingIRQ(irq);
	NVIC_SetPriority(irq, 0);
  NVIC_EnableIRQ(irq);
	
	/* �󶨻ص� */
	timx->irq_callback = tim_irq_callback;
	timx->arg = arg;
	
	timx->auto_load = tim->auto_load;

  return vd_true;
}

/**
 * @brief timer��
 *
 * @param tim  					timer����	
 * @return vd_bool_t		�򿪽��
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
	
	timer_start(htimx);/* ������ʱ�� */
	
  return vd_true;
}

/**
 * @brief timer�ر�
 *
 * @param tim  					timer����		
 * @return vd_bool_t		�رս��
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
	
	timer_stop(htimx);/* �رն�ʱ�� */
	
  return vd_true;
}

/* ================================================== �жϷ����� ================================================================ */
/**
 * @brief timer0�жϷ�����
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
 * @brief timer1�жϷ�����
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


