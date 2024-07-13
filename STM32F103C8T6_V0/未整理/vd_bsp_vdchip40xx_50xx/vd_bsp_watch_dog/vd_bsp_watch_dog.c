/*
 * Project: rf01
 * Module: module_name
 * File: vd_bsp_watch_dog.c
 * Created Date: 2022-06-01 09:41:30
 * Author: huangjunli
 * Description: description
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

#include "vd_bsp_watch_dog.h"

#include "log.h"
#if (PLATFORM_WATCH_DOG_EN == ON)

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

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/* ======================================================================================
 * implementation
 */

/**
 * @brief : watch dog使能
 *
 * @return vd_bool_t
 */
vd_bool_t vd_bsp_watch_dog_enable(vd_void_t) {
#if (KIT_PLATFORM == KIT_VDCHIP4005)
		uint8_t lu8_TempValue;
    pmu_enable_isr(PMU_WTD_INT_EN);
    lu8_TempValue  = WDT_IRQ_ENABLE;
    lu8_TempValue |= PMU_WTD_CTRL_EN;
    ool_write(PMU_REG_WTD_CTRL, lu8_TempValue);
#endif
    return vd_true;
}

/**
 * @brief : watch dog失能
 *
 * @return vd_bool_t
 */
vd_bool_t vd_bsp_watch_dog_disable(vd_void_t) {
#if (KIT_PLATFORM == KIT_VDCHIP4005)
		uint8_t lu8_TempValue;
    lu8_TempValue  = WDT_IRQ_DISABLE;
    lu8_TempValue |= PMU_WTD_CTRL_EN;
    ool_write(PMU_REG_WTD_CTRL, lu8_TempValue);
#endif
    return vd_true;
}

/**
 * @brief : watch dog初始化
 *
 * @param period:复位周期(目前RF01中为1600)
 * @return vd_bool_t
 */
vd_bool_t vd_bsp_watch_dog_init(void) {
#if (KIT_PLATFORM == KIT_VDCHIP4005)
    wdt_Init_t  wdt;
    vd_uint32_t second_timer = 5;  // 5秒超时复位
		// co_printf("lp clock is %d %d.\r\n", pmu_get_rc_clk(false));
		wdt.WdtCount   = pmu_get_rc_clk(false) * second_timer;
		wdt.Timeout    = 100;
    wdt.IRQ_Enable = WDT_IRQ_ENABLE;
    wdt_init(wdt);
#endif
	log_info("Error:vd_bsp_watch_dog_init is ok!!!\n");
    return vd_true;
}

/**
 * @brief : watch dog 喂狗
 *
 * @return vd_bool_t
 */
vd_void_t vd_bsp_watch_dog_kick(vd_void_t) {
#if (KIT_PLATFORM == KIT_VDCHIP4005)
    wdt_Refresh();

	//	vd_bsp_watch_dog_disable();
//	log_info("Dog Dog Refresh\r\n");
#endif
    return;
}
#endif

