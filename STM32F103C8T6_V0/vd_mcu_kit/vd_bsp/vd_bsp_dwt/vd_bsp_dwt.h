#ifndef _VD_BSP_DWT_H
#define _VD_BSP_DWT_H

#include "base_type.h"

#define DWT_CR            *(uint32_t *)0xE0001000
#define DWT_CYCCNT        *(uint32_t *)0xE0001004
#define DEM_CR            *(uint32_t *)0xE000EDFC
#define DEM_CR_TRCENA     (1 << 24)
#define DWT_CR_CYCCNTENA  (1 << 0)


void vd_bsp_dwt_dalay_init(void);
void vd_bsp_dwt_delay_us(volatile uint32_t us);
void vd_bsp_dwt_delay_ms(volatile uint32_t ms);
void vd_bsp_dwt_delay_s(volatile uint32_t s);
#endif
