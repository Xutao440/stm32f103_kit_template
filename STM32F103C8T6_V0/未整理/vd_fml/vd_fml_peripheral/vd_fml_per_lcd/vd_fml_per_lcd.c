#include "vd_fml_per_lcd.h"
#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "vd_log.h"


#ifndef LCD_CHIP_PORT_REGISTER
#define LCD_CHIP_PORT_REGISTER vd_null
#endif



//LCD????
typedef struct{
	lcd_chip_port_register_t 	port_register;	//?????
	lcd_param_t								param;					//?????????
	lcd_ops_t 								ops;						//LCD???????
	lcd_point_t								pre_point;			//¦Ë?¨¹??
}lcd_drv_t;

//LCD????????
static lcd_drv_t lcd = {
	.port_register = LCD_CHIP_PORT_REGISTER,
#if (PROJECT_LCD_EN == ON)
	.param = {PROJECT_LCD_POWER_PORT, PROJECT_LCD_RESET_PORT, PROJECT_LCD_BACKLIGHT_PORT, PROJECT_LCD_DATA_PORT, PROJECT_LCD_TE_PORT, PROJECT_LCD_ROTATE}
#endif
};






//#include "vd_fml_per_lcd_pic.h"
/**
 * @brief : lcd?????
 *
 * @return vd_true
 * @return vd_false
 */
vd_bool_t vd_fml_per_lcd_init(vd_void_t)
{
	vd_bool_t ret = vd_false;
	vd_int32_t _ret;	
//	lcd_point_t point;
	
	/* lcd??? */
	vd_check_return_val_error(lcd.port_register == vd_null, vd_false, "lcd not bound\n");
	ret = lcd.port_register(&lcd.ops);
	vd_check_return_val_error(ret == vd_false, vd_false, "lcd register error\n");
	
	vd_check_return_val_error(lcd.param.rotate >= ROTATE_MAX,  vd_false, "rotate error\n");
	
	//gpio?????
	if(lcd.param.power_port >= 0 && lcd.param.power_port < VD_PORT_GPIO_MAX){
		_ret = vd_bsp_gpio_init(lcd.param.power_port, VD_GPIO_MODE_OUTPUT);
		vd_check_return_val_error(_ret < 0,  vd_false, "power_port init error!!! ret = %d\n", _ret);
		ret = vd_fml_per_lcd_power_control(1);
		vd_check_return_val_error(ret == vd_false,  vd_false, "power on error\r\n");
	}
	if(lcd.param.back_light_port >= 0 && lcd.param.back_light_port < VD_PORT_GPIO_MAX) {
		_ret = vd_bsp_gpio_init(lcd.param.back_light_port, VD_GPIO_MODE_OUTPUT);
		vd_check_return_val_error(_ret < 0,  vd_false, "back_light_port init error!!! ret = %d\n", _ret);
		ret = vd_fml_per_lcd_backlight_control(0);
    	vd_check_return_val_error(ret == vd_false,  vd_false, "backlight off error\r\n");
	}
	if(lcd.param.reset_port >= 0 && lcd.param.reset_port < VD_PORT_GPIO_MAX) {
		_ret = vd_bsp_gpio_init(lcd.param.reset_port, VD_GPIO_MODE_OUTPUT);
		vd_check_return_val_error(_ret < 0,  vd_false, "reset_port init error!!! ret = %d\n", _ret);
		ret = vd_fml_per_lcd_reset();
		vd_check_return_val_error(ret == vd_false,  vd_false, "reset error\r\n");
	}
	
	/* ????????? */
	if(lcd.ops.init){
		ret = lcd.ops.init(&lcd.param);
		vd_check_return_val_error(ret == vd_false, vd_false, "lcd init error\n");
	}

	//???¨¢
	if(lcd.ops.clean){
		ret = lcd.ops.clean(&lcd.param);
		vd_check_return_val_error(ret == vd_false, vd_false, "lcd clean error\n");
	}

	 //?a??¨¦?¦Ì??¡§?¨¢
	PLATFORM_DELAY_MS(50);
	
	ret = vd_fml_per_lcd_backlight_control(1);
	vd_check_return_val_error(ret == vd_false,  vd_false, "backlight on error\r\n");
//		point.x1 = 0;
//	point.y1 = 0;
//	point.x2 = 359;
//	point.y2 = 359;
//while(1){
//	vd_fml_per_lcd_display(&point,(unsigned char*)gImage_Open_UI);
//}
	return vd_true;
}

/**
 * @brief : lcd???
 *
 * @return vd_void_t
 */
vd_void_t vd_fml_per_lcd_loop(vd_void_t)
{
//		log_debug(">>>>>%s, %d\r\n", __func__, __LINE__);
	vd_check_return(lcd.ops.loop == vd_null);
	lcd.ops.loop(&lcd.param);
}

/**
 * @brief : LCD??¦Ë
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_reset(vd_void_t)
{
	vd_int32_t ret;
	
	if(lcd.ops.reset){
		return lcd.ops.reset(&lcd.param);
	}
	
	if(lcd.param.reset_port >= 0 && lcd.param.reset_port < VD_PORT_GPIO_MAX){
		log_debug("lcd reset pin = %d, set 1\r\n", lcd.param.reset_port);
		ret = vd_bsp_gpio_write(lcd.param.reset_port, 1);
		vd_check_return_val_error(ret < 0,  vd_false, "reset ops write error!!! ret = %d\n", ret);
		PLATFORM_DELAY_MS(300);

		log_debug("lcd reset pin = %d, set 0\r\n", lcd.param.reset_port);
		ret = vd_bsp_gpio_write(lcd.param.reset_port, 0);
		vd_check_return_val_error(ret < 0,  vd_false, "reset ops write error!!! ret = %d\n", ret);
		PLATFORM_DELAY_MS(200);

		log_debug("lcd reset pin = %d, set 1\r\n", lcd.param.reset_port);
		ret = vd_bsp_gpio_write(lcd.param.reset_port, 1);
		vd_check_return_val_error(ret < 0,  vd_false, "reset ops write error!!! ret = %d\n", ret);
		PLATFORM_DELAY_MS(300);
		
		return vd_true;
	}
	
	return vd_false;
}

/**
 * @brief : LCD???
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_sleep(vd_void_t)
{
	vd_bool_t ret = vd_false;

	vd_check_return_val_error(lcd.ops.sleep == vd_null,  vd_false, "lcd sleep ops is null\n");
	ret = lcd.ops.sleep(&lcd.param);
	vd_check_return_val_error(ret == vd_false, vd_false, "lcd sleep error\n");

	return vd_true;
}

/**
 * @brief : LCD????
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_wakeup(vd_void_t)
{
	vd_bool_t ret = vd_false;

	vd_check_return_val_error(lcd.ops.wakeup == vd_null,  vd_false, "lcd wakeup ops is null\n");
	ret = lcd.ops.wakeup(&lcd.param);
	vd_check_return_val_error(ret == vd_false, vd_false, "lcd wakeup error\n");

	return vd_true;
}

/**
 * @brief : LCD????¦Ë??
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_set_pos(lcd_point_t *point)
{
	vd_bool_t ret = vd_false;
	
	if(memcmp(point, &lcd.pre_point, sizeof(lcd_point_t)) != 0){//???????¦Ë?¨°????
		
		vd_check_return_val_error(lcd.ops.set_pos == vd_null,  vd_false, "lcd set_pos ops is null\n");
		ret = lcd.ops.set_pos(&lcd.param, point);
		vd_check_return_val_error(ret == vd_false, vd_false, "lcd set_pos error\n");
		
		memcpy(&lcd.pre_point, point, sizeof(lcd_point_t));
	}
		
	return vd_true;
}

/**
 * @brief : LCD???
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_display(lcd_point_t *point, vd_uint8_t* color)
{
	vd_bool_t ret = vd_false;
	
	vd_check_return_val_error(point == vd_null,  vd_false, "point is null\n");
	vd_check_return_val_error(color == vd_null,  vd_false, "color is null\n");
	
	ret = vd_fml_per_lcd_set_pos(point);
	vd_check_return_val_error(ret == vd_false, vd_false, "lcd set_pos error\n");
	
	vd_check_return_val_error(lcd.ops.display == vd_null,  vd_false, "lcd display ops is null\n");
	ret = lcd.ops.display(&lcd.param, point, color);
	vd_check_return_val_error(ret == vd_false, vd_false, "lcd display error\n");
	
	return vd_true;
}

/**
 * @brief : LCD????
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_clean(vd_void_t)
{
	vd_bool_t ret = vd_false;

	vd_check_return_val_error(lcd.ops.clean == vd_null,  vd_false, "lcd clean ops is null\n");
	ret = lcd.ops.clean(&lcd.param);
	vd_check_return_val_error(ret == vd_false, vd_false, "lcd clean error\n");

	return vd_true;
}

/**
 * @brief : LCD???????
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_power_control(vd_bool_t sw)
{
	vd_int32_t ret;
	
	if(lcd.ops.power_control){
		return lcd.ops.power_control(&lcd.param, sw);
	}
	
	if(lcd.param.power_port >= 0 && lcd.param.power_port < VD_PORT_GPIO_MAX){
		log_debug("lcd power pin = %d set %d\r\n", lcd.param.power_port, sw);
		ret = vd_bsp_gpio_write(lcd.param.power_port, sw);
		vd_check_return_val_error(ret < 0,  vd_false, "power ops write error!!! ret = %d\n", ret);
		PLATFORM_DELAY_MS(20);
		return vd_true;
	}
	
	return vd_false;
}

/**
 * @brief : LCD???????
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_backlight_control(vd_uint32_t val)
{
	vd_int32_t ret;

	if(lcd.ops.backlight_control){
		return lcd.ops.backlight_control(&lcd.param, val);
	}
	
	if(lcd.param.back_light_port >= 0 && lcd.param.back_light_port < VD_PORT_GPIO_MAX){
		log_debug("lcd backlight pin = %d set %d\r\n", lcd.param.back_light_port, val);
		ret = vd_bsp_gpio_write(lcd.param.back_light_port, val);
		vd_check_return_val_error(ret < 0,  vd_false, "backlight ops write error!!! ret = %d\n", ret);
		return vd_true;
	}

	return vd_false;
}

/**
 * @brief : LCD??????
 *
 * @return vd_void_t
 */
vd_bool_t vd_fml_per_lcd_get_size(lcd_size_t *size)
{
	vd_bool_t ret = vd_false;

	vd_check_return_val_error(lcd.ops.get_size == vd_null,  vd_false, "lcd get_size ops is null\n");
	ret = lcd.ops.get_size(size);
	vd_check_return_val_error(ret == vd_false, vd_false, "lcd get_size error\n");

	return vd_true;
}



