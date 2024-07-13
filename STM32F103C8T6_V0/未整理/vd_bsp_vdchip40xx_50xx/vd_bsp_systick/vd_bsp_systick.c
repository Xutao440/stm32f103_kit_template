/*
 * Project: mcu-kit
 * Module: 滴答定时器驱动
 * File: drv_systick.c
 * Created Date: 2022-03-28 19:21:16
 * Author: meizhen
 * Description: 用于表示mcu系统时间
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)

/* ======================================================================================
 * includes
 */
#include "vd_bsp_systick.h"

/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */

#if (PLATFORM_SYSTICK_EN == ON)
static volatile vd_ulong_t g_systick = 0;
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/**
 * @brief : 滴答定时器中断回调，计数器累加
 *
 */
void SysTick_Handler(void) {
    g_systick++;
}

/* ======================================================================================
 * implementation
 */

#if (KIT_PLATFORM == KIT_RF01)
#include "bx_kernel.h"
#include "bx_kernel_timer.h"
#include "ke_timer.h"
#include "systick.h"

void plat_systick_init(void) {
    SysTick->LOAD = (vd_uint32_t)(MAIN_CLOCK / 1000 - 1UL); /* set reload register */  // 1us
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
    SysTick->VAL  = 0UL;                                             /* Load the SysTick Counter Value */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */

    NVIC_ClearPendingIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);
    __enable_irq();
}

#else
void plat_systick_init(void)
{
    
}
#endif


/**
 * @brief :
 *
 * @return vd_void_t
 */
vd_void_t drv_systick_loop(vd_void_t) {
		#if PALTFORM_SYSTICK_LOG
    log_print(".");
		#endif
}

/**
 * @brief : 获取系统滴答定时器
 *
 * @return vd_uin32_t
 */
vd_ulong_t drv_systick_get(vd_void_t) {
    return g_systick;
}

/**
 * @brief 系统滴答定时器初始化
 *
 * @return vd_bool_t
 */
vd_bool_t drv_systick_init(vd_void_t) {
    plat_systick_init();
		#if PALTFORM_SYSTICK_LOG
    log_debug("drv_systick_init success\r\n");
		#endif
    return vd_true;
}
#endif