#ifndef __VD_FML_PER_LCD_TYPE_H
#define __VD_FML_PER_LCD_TYPE_H

#include "base.h"
#include "vd_bsp_type.h"

//旋转角度
typedef enum{
	ROTATE_0,
	ROTATE_90,
	ROTATE_180,
	ROTATE_270,
	ROTATE_MAX,
}lcd_rotate_e;

//尺寸
typedef struct {
	vd_uint32_t width;
	vd_uint32_t height;
}lcd_size_t;

//坐标
typedef struct {
	vd_int32_t x1;
	vd_int32_t y1;
	vd_int32_t x2;
	vd_int32_t y2;
}lcd_point_t;

//LCD初始化参数
typedef struct{
	vd_bsp_port_e	power_port;											//电源端口
	vd_bsp_port_e	reset_port;											//复位端口
	vd_bsp_port_e	back_light_port;								//背光端口
	vd_bsp_port_e	data_port;											//数据端口
	vd_bsp_port_e	TE_port;												//TE端口
	lcd_rotate_e	rotate;													//角度
}lcd_param_t;

//LCD驱动接口
typedef struct{
	vd_bool_t (*init)(lcd_param_t *param);
	vd_void_t (*loop)(lcd_param_t *param);
	vd_bool_t (*reset)(lcd_param_t *param);
	vd_bool_t (*sleep)(lcd_param_t *param);
	vd_bool_t (*wakeup)(lcd_param_t *param);
	vd_bool_t (*set_pos)(lcd_param_t *param, lcd_point_t *point);
	vd_bool_t (*display)(lcd_param_t *param, lcd_point_t *point, vd_uint8_t *color);
	vd_bool_t (*clean)(lcd_param_t *param);
	vd_bool_t (*power_control)(lcd_param_t *param, vd_bool_t sw);
	vd_bool_t (*backlight_control)(lcd_param_t *param, vd_uint32_t val);
	vd_bool_t (*get_size)(lcd_size_t *size);
}lcd_ops_t;

//tp注册函数
typedef vd_bool_t (*lcd_chip_port_register_t)(lcd_ops_t *ops);


#endif