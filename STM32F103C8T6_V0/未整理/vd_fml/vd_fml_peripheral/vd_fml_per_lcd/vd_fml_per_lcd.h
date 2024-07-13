#ifndef __VD_FML_PER_LCD_H
#define __VD_FML_PER_LCD_H

#include "vd_fml_per_lcd_type.h"
#include "vd_fml_per_lcd_chip_config.h"



/**
 * @brief : lcd��ʼ��
 *
 * @return vd_true
 * @return vd_false
 */
vd_bool_t vd_fml_per_lcd_init(vd_void_t);

/**
 * @brief : LCD��ѯ
 *
 * @return vd_void_t
 */
vd_void_t vd_fml_per_lcd_loop(vd_void_t);

/**
 * @brief : LCD��λ
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_reset(vd_void_t);

/**
 * @brief : LCD˯��
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_sleep(vd_void_t);

/**
 * @brief : LCD����
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_wakeup(vd_void_t);

/**
 * @brief : LCD����λ��
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_set_pos(lcd_point_t *point);

/**
 * @brief : LCD��ʾ
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_display(lcd_point_t *point, vd_uint8_t* color);

/**
 * @brief : LCD����
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_clean(vd_void_t);

/**
 * @brief : LCD��Դ����
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_power_control(vd_bool_t sw);

/**
 * @brief : LCD�������
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_backlight_control(vd_uint32_t val);

/**
 * @brief : LCD��ȡ�ߴ�
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_get_size(lcd_size_t *size);


#endif
