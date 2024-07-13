#include "lcd_st77903_bohu_1.6_400_400_qspi.h"
#include "vd_log.h"
#include "vd_bsp_qspi/vd_bsp_qspi.h"

#define LCD_WIDTH		400
#define LCD_HEIGHT	400

typedef enum {
    LCD_CMD = 0,  // 命令
    LCD_DELAY,    // 延时
}init_table_type_e;

typedef struct{
	init_table_type_e type;	
	union {
		vd_uint8_t 	data[20];//写内容
		vd_uint16_t delay;  // 延时时间,ms
	} u;
}init_table_t;

static init_table_t init_table[] = {
	  {LCD_CMD, 0xf0, 1, 0xc3},
    {LCD_CMD, 0xf0, 1, 0x96},
    {LCD_CMD, 0xf0, 1, 0xa5},
    {LCD_CMD, 0xe9, 1, 0x20},
    {LCD_CMD, 0xe7, 4, 0x80, 0x77, 0x1f, 0xcc},
    {LCD_CMD, 0xc1, 4, 0x77, 0x07, 0xcf, 0x16},
    {LCD_CMD, 0xc2, 4, 0x77, 0x07, 0xcf, 0x16},
    {LCD_CMD, 0xc3, 4, 0x00, 0x00, 0x00, 0x04},//4, 0x22, 0x02, 0x22, 0x04},
    {LCD_CMD, 0xc4, 4, 0x00, 0x00, 0x00, 0x04},//4, 0x22, 0x02, 0x22, 0x04},
    {LCD_CMD, 0xc5, 1, 0xed},
    {LCD_CMD, 0xe0, 14, 0x87, 0x09, 0x0c, 0x06, 0x05, 0x03, 0x29, 0x32, 0x49, 0x0f, 0x1b, 0x17, 0x2a, 0x2f},
    {LCD_CMD, 0xe1, 14, 0x87, 0x09, 0x0c, 0x06, 0x05, 0x03, 0x29, 0x32, 0x49, 0x0f, 0x1b, 0x17, 0x2a, 0x2f},
    {LCD_CMD, 0xe5, 14, 0xbe, 0xf5, 0xb1, 0x22, 0x22, 0x25, 0x10, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
    {LCD_CMD, 0xe6, 14, 0xbe, 0xf5, 0xb1, 0x22, 0x22, 0x25, 0x10, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22},
    {LCD_CMD, 0xec, 2, 0x40, 0x03},
    {LCD_CMD, 0x36, 1, 0x0C},//4
    {LCD_CMD, 0x3a, 1, 0x05},//7：888；6：666；5：565
    {LCD_CMD, 0xb2, 1, 0x00},
    {LCD_CMD, 0xb3, 1, 0x01},
    {LCD_CMD, 0xb4, 1, 0x00},
    {LCD_CMD, 0xb5, 4, 0x00, 0x06, 0x00, 0x06},
    {LCD_CMD, 0xb6, 2, 0xc7, 0x31},
    {LCD_CMD, 0xa5, 9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x2a, 0x8a, 0x02},
    {LCD_CMD, 0xa6, 9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x2a, 0x8a, 0x02},
    {LCD_CMD, 0xba, 7, 0x0a, 0x5a, 0x23, 0x10, 0x25, 0x02, 0x00},
    {LCD_CMD, 0xbb, 8, 0x00, 0x30, 0x00, 0x2c, 0x82, 0x87, 0x18, 0x00},
    {LCD_CMD, 0xbc, 8, 0x00, 0x30, 0x00, 0x2c, 0x82, 0x87, 0x18, 0x00},
    {LCD_CMD, 0xbd, 11, 0xa1, 0xb2, 0x2b, 0x1a, 0x56, 0x43, 0x34, 0x65, 0xff, 0xff, 0x0f},
    {LCD_CMD, 0x35, 1, 0x00},
    {LCD_CMD, 0x21, 0, 0x00},
    {LCD_CMD, 0x11, 0, 0x00},
		{LCD_DELAY, 120},
    {LCD_CMD, 0x29, 0, 0x00},		
    {LCD_DELAY, 120},
#if 0
    {LCD_CMD,  0xb0,     1,     0xa5},
    {LCD_CMD,  0xcc,     9,     0x40, 0x00, 0x3f, 0x00, 0x14, 0x14, 0x20, 0x20, 0x03},
#endif
};



//LCD驱动
typedef struct{
	vd_bool_t 	flush_ok;		//刷图完成标志
	vd_uint8_t 	display_buf_line[2][LCD_WIDTH * 2];//行显示缓存
	vd_uint8_t 	*display_buf;//界面显示缓存
}st77903_bohu_1_6_400_400_t;

st77903_bohu_1_6_400_400_t *st77903_bohu_1_6_400_400 = vd_null;

/**
* @brief : lcd写命令
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_write_cmd(vd_bsp_port_e port, vd_uint8_t cmd, vd_uint8_t *data, vd_uint16_t data_len)
{
	vd_int32_t ret;
	vd_qspi_tx_cfg_t qspi_cfg = {0};	
	vd_uint8_t 	my_data[64] = {0};
	vd_uint16_t buf_len;
	
	qspi_cfg.line = QSPI_LINE_X1;
	
	buf_len = 4;
	my_data[0] = 0xDE;
	my_data[1] = 0x00;
	my_data[2] = cmd;
	my_data[3] = 0x00;
	if(data_len){		
		
		vd_check_return_val_error(data_len > sizeof(my_data) - 4,  vd_false, "data_len > buf len\n");
		vd_check_return_val_error(data == vd_null,  vd_false, "data_len > 0 but data is null\n");
		
		memcpy(&my_data[4], data, data_len);
		buf_len += data_len;		
	}
	
	vd_bsp_qspim_cs_control(port, 0);
	
	ret = vd_bsp_qspim_write(port, &qspi_cfg, my_data, buf_len);
	
	vd_bsp_qspim_cs_control(port, 1);
	
	vd_check_return_val_error(ret < 0,  vd_false, "lcd write cmd error!!! ret = %d\n", ret);
	
	return true;
}

/**
* @brief : lcd写数据
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_write_data(vd_bsp_port_e port, vd_uint8_t *data, vd_uint16_t data_len)
{
	vd_int32_t ret;
	vd_qspi_tx_cfg_t qspi_cfg = {0};	
	
	qspi_cfg.line = QSPI_LINE_X4;
	qspi_cfg.instruct_len = 1;
	qspi_cfg.instruct = 0xDE;
	qspi_cfg.address_len = 3;
	qspi_cfg.address = 0x006000;
	
	vd_bsp_qspim_cs_control(port, 0);
	
	ret = vd_bsp_qspim_write(port, &qspi_cfg, data, data_len);
	
	vd_bsp_qspim_cs_control(port, 1);
	
	vd_check_return_val_error(ret < 0,  vd_false, "lcd write data error!!! ret = %d\n", ret);
	
	return vd_true;
}

/**
* @brief : lcd刷图完成回调
 *
 * @return vd_void_t
 */
vd_void_t lcd_st77903_bohu_1_6_400_400_flush_cplt_callback(vd_void_t *arg)
{
	lcd_param_t *param = (lcd_param_t *)arg;
	
	vd_bsp_qspim_cs_control(param->data_port, 1);
	
	st77903_bohu_1_6_400_400->flush_ok = vd_true;
}

/**
* @brief : lcd刷图
 *
 * @return vd_void_t
 */
vd_void_t lcd_st77903_bohu_1_6_400_400_flush(lcd_param_t *param, vd_uint32_t cmd,  vd_uint8_t *data, vd_uint32_t data_len)
{
	vd_qspi_tx_cfg_t qspi_cfg = {0};
	
	qspi_cfg.address = cmd << 8;
	qspi_cfg.address_len = 3;
	qspi_cfg.instruct = 0xDE;
	qspi_cfg.instruct_len = 1;
	qspi_cfg.line = QSPI_LINE_X4;	
	
	while(st77903_bohu_1_6_400_400->flush_ok == vd_false){};
	
	vd_bsp_qspim_cs_control(param->data_port, 0);
		
	if (data_len == 0){
		vd_bsp_qspim_write(param->data_port, &qspi_cfg, vd_null, 0);
		PLATFORM_DELAY_US(40);
		vd_bsp_qspim_cs_control(param->data_port, 1);
	}else{
		vd_bsp_qspim_write_dma(param->data_port, &qspi_cfg, data, data_len, lcd_st77903_bohu_1_6_400_400_flush_cplt_callback, param);
		st77903_bohu_1_6_400_400->flush_ok = vd_false;
	}
}





/**
* @brief : lcd休眠
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_sleep(lcd_param_t *param)
{	
	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, 0x28, vd_null, 0) == false){
		return vd_false;
	}
	
	PLATFORM_DELAY_MS(120);
	
	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, 0x10, vd_null, 0) == false){
		return vd_false;
	}
	
	PLATFORM_DELAY_MS(150);	
	
	return vd_true;
}

/**
* @brief : lcd唤醒
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_wakeup(lcd_param_t *param)
{
	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, 0x11, vd_null, 0) == false){
		return vd_false;
	}
	
	PLATFORM_DELAY_MS(120);
	
	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, 0x29, vd_null, 0) == false){
		return vd_false;
	}
	
	PLATFORM_DELAY_MS(100);	
	
	return vd_true;
}



/**
 * @brief : lcd初始化
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_init(lcd_param_t *param)
{
	vd_int32_t ret;
	vd_uint32_t	i;
	
	//参数校验
	vd_check_return_val_error(param == vd_null,  vd_false, "param is null\n");
	vd_check_return_val_error(param->data_port < VD_PORT_QSPIM0 || param->data_port >= VD_PORT_QSPIM_MAX,  vd_false, "data_port error\n");
	
	st77903_bohu_1_6_400_400 = PLATFORM_MALLOC(sizeof(st77903_bohu_1_6_400_400_t));
	vd_check_return_val_error(st77903_bohu_1_6_400_400 == vd_null,  vd_false, "st77903_bohu_1_6_400_400 driver malloc error\r\n");
	memset(st77903_bohu_1_6_400_400, 0, sizeof(st77903_bohu_1_6_400_400_t));
	st77903_bohu_1_6_400_400->flush_ok = vd_true;

	/* qspi初始化 */
	ret = vd_bsp_qspim_init(param->data_port);
	vd_check_return_val_error(ret < 0,  vd_false, "lcd qspim init error!!! ret = %d\n", ret);
	
	
	//写屏幕初始化参数
	for(i = 0; i < sizeof(init_table) / sizeof(init_table[0]); i++){
		
			if(init_table[i].type == LCD_DELAY){
				PLATFORM_DELAY_MS(init_table[i].u.delay);
				continue;
			}
			
			if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, init_table[i].u.data[0], &init_table[i].u.data[2], init_table[i].u.data[1]) == false){
				return vd_false;
			}
	}	
	
	return true;
}

/**
 * @brief : lcd轮询
 *
 */
vd_void_t lcd_st77903_bohu_1_6_400_400_loop(lcd_param_t *param)
{
	vd_uint32_t i = 0;
	vd_uint8_t display_idx = 0;
	
	if(st77903_bohu_1_6_400_400->display_buf == vd_null){
		return;
	}
	
	log_debug(">>>>> %s\r\n", __func__, __LINE__);
	
			/* vs(0x61) packet */
	for (i = 0; i < 1; i++)
	{
			lcd_st77903_bohu_1_6_400_400_flush(param, 0x61, NULL, 0); 
	}
	
	/* hbp(0x60) packet */
	for (i = 0; i < 6; i++)
	{
			lcd_st77903_bohu_1_6_400_400_flush(param, 0x60, NULL, 0); 
	}

	/* transmit display cache data to lcd line by line */	
	for (i = 0; i <= LCD_HEIGHT; i++)
	{	
		if(i != 0){
			lcd_st77903_bohu_1_6_400_400_flush(	param, \
																					0x60, \
																					st77903_bohu_1_6_400_400->display_buf_line[display_idx], \
																					sizeof(st77903_bohu_1_6_400_400->display_buf_line[0]));	
		}
		
		display_idx++;
		if(display_idx >= sizeof(st77903_bohu_1_6_400_400->display_buf_line) / sizeof(st77903_bohu_1_6_400_400->display_buf_line[0])){
			display_idx = 0;
		}
						
		memcpy(	st77903_bohu_1_6_400_400->display_buf_line[display_idx], \
						&st77903_bohu_1_6_400_400->display_buf[i * sizeof(st77903_bohu_1_6_400_400->display_buf_line[0])], \
						sizeof(st77903_bohu_1_6_400_400->display_buf_line[0]));
	}

	for (i = 0; i < 6; i++)
	{
			lcd_st77903_bohu_1_6_400_400_flush(param, 0x60, vd_null, 0); 
	}
}
	
/**
 * @brief : lcd设置坐标
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_set_pos(lcd_param_t *param, lcd_point_t *point)
{
	vd_bool_t ret = false;
	vd_uint8_t temp;
	
	
	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, 0x2a, vd_null, 0) == false){//列地址设置
		return vd_false;
	}
	// mcu_delay(50);
	temp = (point->x1 >> 8) & 0xff;
	if(lcd_st77903_bohu_1_6_400_400_write_data(param->data_port, &temp, 1) == false){
		return vd_false;
	}
	temp = point->x1 & 0xff;
	if(lcd_st77903_bohu_1_6_400_400_write_data(param->data_port, &temp, 1) == false){
		return vd_false;
	}
	temp = (point->x2 >> 8) & 0xff;
	if(lcd_st77903_bohu_1_6_400_400_write_data(param->data_port, &temp, 1) == false){
		return vd_false;
	}
	temp = point->x2 & 0xff;
	if(lcd_st77903_bohu_1_6_400_400_write_data(param->data_port, &temp, 1) == false){
		return vd_false;
	}
	 
	
	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, 0x2b, vd_null, 0) == false){//行地址设置
		return vd_false;
	}	
	// mcu_delay(50);
	temp = (point->y1 >> 8) & 0xff;
	if(lcd_st77903_bohu_1_6_400_400_write_data(param->data_port, &temp, 1) == false){
		return vd_false;
	}
	temp = point->y1 & 0xff;
	if(lcd_st77903_bohu_1_6_400_400_write_data(param->data_port, &temp, 1) == false){
		return vd_false;
	}
	temp = (point->y2 >> 8) & 0xff;
	if(lcd_st77903_bohu_1_6_400_400_write_data(param->data_port, &temp, 1) == false){
		return vd_false;
	}
	temp = point->y2 & 0xff;
	if(lcd_st77903_bohu_1_6_400_400_write_data(param->data_port, &temp, 1) == false){
		return vd_false;
	}


	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, 0x2c, vd_null, 0) == false){ //储存器写
		return vd_false;
	}
	
	return vd_true;
}

/**
 * @brief : lcd显示
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_display(lcd_param_t *param, lcd_point_t *point, vd_uint8_t *color)
{
//	memcpy(st77903_bohu_1_6_400_400->display_buf, color, sizeof(st77903_bohu_1_6_400_400->display_buf));
	st77903_bohu_1_6_400_400->display_buf = color;
	return vd_true;
}

/**
 * @brief : lcd清屏
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_clean(lcd_param_t *param)
{
	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, 0x01, vd_null, 0) == false){// Software reset
		return vd_false;
	}	
	
	PLATFORM_DELAY_MS(1);
	
	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, 0x29, vd_null, 0) == false){
		return vd_false;
	}	
	
	PLATFORM_DELAY_US(120);
	
	return vd_true;
}

/**
 * @brief : lcd背光控制
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_backlight_control(lcd_param_t *param, vd_uint32_t val)
{
	if(lcd_st77903_bohu_1_6_400_400_write_cmd(param->data_port, val ? 0x29 : 0x28, vd_null, 0) == false){
		return vd_false;
	}	
	
	return vd_true;
}

/**
* @brief : lcd获取尺寸
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_get_size(lcd_size_t *size)
{
	vd_check_return_val_error(size == vd_null,  vd_false, "size is null\n");
	
	size->width = LCD_WIDTH;
	size->height = LCD_HEIGHT;
		
	return vd_true;
}

/**
 * @brief : lcd注册
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_st77903_bohu_1_6_400_400_register(lcd_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null,  vd_false, "lcd register ops is null\n");
	
	ops->init = lcd_st77903_bohu_1_6_400_400_init;
	ops->loop = lcd_st77903_bohu_1_6_400_400_loop;
	ops->sleep = lcd_st77903_bohu_1_6_400_400_sleep;
	ops->wakeup = lcd_st77903_bohu_1_6_400_400_wakeup;
	ops->set_pos = lcd_st77903_bohu_1_6_400_400_set_pos;
	ops->display = lcd_st77903_bohu_1_6_400_400_display;
	
	ops->clean = lcd_st77903_bohu_1_6_400_400_clean;
	ops->backlight_control = lcd_st77903_bohu_1_6_400_400_backlight_control;
	ops->get_size = lcd_st77903_bohu_1_6_400_400_get_size;
	
	return vd_true;
}