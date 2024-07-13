/*
 * Project: mcu-kit
 * Module: lcd_»­Í¼¹¤¾ß
 * File: vd_fml_per_lcd_util.h
 * Created Date: 2022-04-20 19:26:27
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_LCD_UTIL_H
#define _DRV_LCD_UTIL_H

/* ======================================================================================
 * includes
 */
#include "base.h"
/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

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
 * extern
 */

vd_void_t vd_fml_per_lcd_show_char(vd_uint16_t x, vd_uint16_t y, vd_uint8_t chr);

vd_void_t vd_fml_per_lcd_show_number(vd_uint8_t x, vd_uint8_t y, vd_uint32_t num, vd_uint8_t len, vd_uint8_t size2);

vd_void_t vd_fml_per_lcd_show_string(vd_uint16_t x, vd_uint16_t y, vd_uint8_t* str);

vd_void_t vd_fml_per_lcd_show_bmp(vd_uint16_t x0, vd_uint16_t y0, vd_uint16_t x1, vd_uint16_t y1, vd_uint8_t BMP[]);

vd_void_t vd_fml_per_lcd_test(vd_void_t);

__vd_extern_c_leave__
#endif  // _DRV_LCD_UTIL_H