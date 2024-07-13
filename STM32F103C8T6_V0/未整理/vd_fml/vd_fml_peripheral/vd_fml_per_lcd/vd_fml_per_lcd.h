#ifndef __VD_FML_PER_LCD_H
#define __VD_FML_PER_LCD_H

#include "vd_fml_per_lcd_type.h"
#include "vd_fml_per_lcd_chip_config.h"



/**
 * @brief : lcd初始化
 *
 * @return vd_true
 * @return vd_false
 */
vd_bool_t vd_fml_per_lcd_init(vd_void_t);

/**
 * @brief : LCD轮询
 *
 * @return vd_void_t
 */
vd_void_t vd_fml_per_lcd_loop(vd_void_t);

/**
 * @brief : LCD复位
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_reset(vd_void_t);

/**
 * @brief : LCD睡眠
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_sleep(vd_void_t);

/**
 * @brief : LCD唤醒
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_wakeup(vd_void_t);

/**
 * @brief : LCD设置位置
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_set_pos(lcd_point_t *point);

/**
 * @brief : LCD显示
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_display(lcd_point_t *point, vd_uint8_t* color);

/**
 * @brief : LCD清屏
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_clean(vd_void_t);

/**
 * @brief : LCD电源控制
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_power_control(vd_bool_t sw);

/**
 * @brief : LCD背光控制
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_backlight_control(vd_uint32_t val);

/**
 * @brief : LCD获取尺寸
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_get_size(lcd_size_t *size);


#endif
