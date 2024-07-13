/*
 * Project: mcu-kit
 * Module: uart driver
 * File: vd_bsp_uart.h
 * Created Date: 2022-03-23 10:45:36
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _VDCHIP40XX_50XX_UART_H
#define _VDCHIP40XX_50XX_UART_H

/* ======================================================================================
 * includes
 */
#include "vd_bsp_uart/vd_bsp_uart.h"


/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;



vd_bool_t vd_fr8000_uart_register(vd_bsp_uart_ops_t *ops);



__vd_extern_c_leave__
#endif  // _DRV_UART_H
