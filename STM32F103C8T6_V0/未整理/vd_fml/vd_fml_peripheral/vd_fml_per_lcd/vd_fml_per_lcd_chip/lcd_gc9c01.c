#include "lcd_gc9c01.h"
#include "base.h"
#include "vd_log.h"
#include "vd_bsp_qspi/vd_bsp_qspi.h"

#define LCD_WIDTH		480
#define LCD_HEIGHT	272
#define GC9C01_EXTI_PIN	14	//PB6

typedef struct{
	vd_uint8_t 	display_buf[2][LCD_WIDTH * 2 * 2];
	
	vd_uint8_t send_flag 	: 1;
	vd_uint8_t irq_en			: 1;
	vd_uint8_t flush_ok		:	1;
	vd_uint8_t reserve		:	5;
}gc9c01_t;

gc9c01_t *gc9c01 = vd_null;

/**
 * @brief : 写数据
 *
 * @return vd_bool_t
 */
static void gc9c01_write_buff(vd_bsp_port_e qspi_id, uint8_t *buffer, uint8_t len)
{
	vd_int32_t ret;
	vd_qspi_tx_cfg_t cfg = {0};
	
	cfg.line = QSPI_LINE_X1;
	
	ret = vd_bsp_qspim_cs_control(qspi_id, 0);
	vd_check_return_error(ret < 0, "gc9c01 cs control error!!! ret = %d\n", ret);
	ret = vd_bsp_qspim_write(qspi_id, &cfg, buffer, len);
	vd_check_return_error(ret < 0, "gc9c01 qspi write error!!! ret = %d\n", ret);
	ret = vd_bsp_qspim_cs_control(qspi_id, 1);
	vd_check_return_error(ret < 0, "gc9c01 cs control error!!! ret = %d\n", ret);
}

/**
 * @brief : 写命令
 *
 * @return vd_bool_t
 */
static void gc9c01_write_cmd(vd_bsp_port_e qspi_id, uint8_t cmd)
{
	uint8_t spi_data[4];

	spi_data[0]=0x02;
	spi_data[1]=0x00;
	spi_data[2]=cmd;
	spi_data[3]=0x00;
	
	gc9c01_write_buff(qspi_id, spi_data, 4);
}

/**
 * @brief : 写参数
 *
 * @return vd_bool_t
 */
static void gc9c01_write_cmd_with_param(vd_bsp_port_e qspi_id, vd_uint8_t cmd, vd_uint8_t para)
{
	uint8_t spi_data[5];
	
	spi_data[0]=0x02;
	spi_data[1]=0x00;
	spi_data[2]=cmd;
	spi_data[3]=0x00;
	spi_data[4]=para;
	
	gc9c01_write_buff(qspi_id, spi_data, 5);
}


/**
 * @brief : 设置窗口
 *
 * @return vd_bool_t
 */
static void gc9c01_set_window(vd_bsp_port_e qspi_id, uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e)
{   
//		  y_s+=0;
//	  y_e-=1;
//	   	x_s-=0;
//    x_e-=1;
    uint8_t spi_data[8];
   
    spi_data[0] = 0x02;
    spi_data[1] = 0x00;
    spi_data[2] = 0x2a;
    spi_data[3] = 0x00;
    spi_data[4] = x_s>>8;
    spi_data[5] = x_s&0xff;
    spi_data[6] = x_e>>8;
    spi_data[7] = x_e&0xff;
    gc9c01_write_buff(qspi_id, spi_data, 8);

    spi_data[0] = 0x02;
    spi_data[1] = 0x00;
    spi_data[2] = 0x2b;
    spi_data[3] = 0x00;
    spi_data[4] = y_s>>8;
    spi_data[5] = y_s&0xff;
    spi_data[6] = y_e>>8;
    spi_data[7] = y_e&0xff;
    gc9c01_write_buff(qspi_id, spi_data, 8);

    gc9c01_write_cmd(qspi_id, 0x2c); 
	//gc9c01_write_cmd(0x3c);   //ram+dam ??????
}


/**
 * @brief : 外部中断回调
 *
 * @return vd_bool_t
 */
static vd_void_t gc9c01_exti_callback(vd_void_t *arg)
{
	vd_int32_t ret;
	
	vd_check_return_error(gc9c01 == vd_null, "func = %s, line = %d\r\n", __func__, __LINE__);
	
	gc9c01->irq_en = 1;
//	 exti_interrupt_disable(EXTI_LINE14_PB6);
	ret = vd_bsp_gpio_irq_enable(GC9C01_EXTI_PIN, VD_GPIO_IRQ_DISABLE);
	vd_check_return_error(ret < 0, "ret = %d, func = %s, line = %d\r\n", ret, __func__, __LINE__);
}

/**
 * @brief : 刷图完成回调
 *
 * @return vd_bool_t
 */
static vd_void_t gc9c01_flush_cplt_callback(vd_void_t *arg)
{
	lcd_param_t *param = (lcd_param_t *)arg;
	
	vd_check_return_error(gc9c01 == vd_null, "func = %s, line = %d\r\n", __func__, __LINE__);
	
	vd_bsp_qspim_cs_control(param->data_port, 1);
	
	gc9c01->flush_ok = vd_true;
}

/**
 * @brief : 刷图
 *
 * @return vd_bool_t
 */
static vd_void_t gc9c01_flush(lcd_param_t *param, vd_uint8_t *data, vd_uint32_t data_len)
{
  vd_uint32_t total_count = data_len / 2;   // accoding source width
	vd_qspi_tx_cfg_t qspi_cfg = {0};
	
	vd_check_return_error(gc9c01 == vd_null, "func = %s, line = %d\r\n", __func__, __LINE__);
	
	qspi_cfg.address = 0x003C00;
	qspi_cfg.address_len = 3;
	qspi_cfg.instruct = 0x32;
	qspi_cfg.instruct_len = 1;
	qspi_cfg.line = QSPI_LINE_X4;	

	while(gc9c01->flush_ok == vd_false){};

	vd_bsp_qspim_cs_control(param->data_port, 0);
		

	vd_bsp_qspim_write_dma(param->data_port, &qspi_cfg, data, total_count, gc9c01_flush_cplt_callback, param);
	gc9c01->flush_ok = vd_false;

}

/**
 * @brief : 初始化
 *
 * @return vd_bool_t
 */
vd_bool_t gc9c01_init(lcd_param_t *param)
{
	vd_int32_t ret;
	vd_uint32_t	i;
	
	//参数校验
	vd_check_return_val_error(param == vd_null,  vd_false, "param is null\n");
	vd_check_return_val_error(param->data_port < VD_PORT_QSPIM0 || param->data_port >= VD_PORT_QSPIM_MAX,  vd_false, "data_port error\n");

	gc9c01 = PLATFORM_MALLOC(sizeof(gc9c01_t));
	vd_check_return_val_error(gc9c01 == vd_null, vd_false, "gc9c01 malloc error\r\n");
	memset(gc9c01, 0, sizeof(gc9c01_t));
	gc9c01->flush_ok = vd_true;
	
	/* qspi初始化 */
	ret = vd_bsp_qspim_init(param->data_port);
	vd_check_return_val_error(ret < 0,  vd_false, "lcd qspim init error!!! ret = %d\n", ret);
	
	//外部中断初始化
	ret = vd_bsp_gpio_requst_irq(GC9C01_EXTI_PIN, VD_GPIO_IRQ_MODE_RISING, gc9c01_exti_callback, vd_null);
	vd_check_return_val_error(ret < 0,  vd_false, "lcd exti	init error!!! ret = %d\n", ret);
	ret = vd_bsp_gpio_irq_enable(GC9C01_EXTI_PIN, VD_GPIO_IRQ_ENABLE);
	vd_check_return_val_error(ret < 0,  vd_false, "lcd exti	enable error!!! ret = %d\n", ret);
	
	gc9c01_write_cmd_with_param(param->data_port, 0xff, 0xa5);
	gc9c01_write_cmd_with_param(param->data_port, 0xe7, 0x10);
	gc9c01_write_cmd_with_param(param->data_port, 0x35, 0x00);
	gc9c01_write_cmd_with_param(param->data_port, 0x36, 0x00);     // gc9c01_write_cmd_with_param(0x36, 0xc0);
	gc9c01_write_cmd_with_param(param->data_port, 0x3a, 0x01);
	gc9c01_write_cmd_with_param(param->data_port, 0x40, 0x01);/////
	gc9c01_write_cmd_with_param(param->data_port, 0x41, 0x01);
	gc9c01_write_cmd_with_param(param->data_port, 0x44, 0x15);
	gc9c01_write_cmd_with_param(param->data_port, 0x45, 0x15);
	gc9c01_write_cmd_with_param(param->data_port, 0x7d, 0x03);
	gc9c01_write_cmd_with_param(param->data_port, 0xc1, 0xbb);
	gc9c01_write_cmd_with_param(param->data_port, 0xc2, 0x05);
	gc9c01_write_cmd_with_param(param->data_port, 0xc3, 0x10);
	gc9c01_write_cmd_with_param(param->data_port, 0xc6, 0x3e);
	gc9c01_write_cmd_with_param(param->data_port, 0xc7, 0x25);
	gc9c01_write_cmd_with_param(param->data_port, 0xc8, 0x21);  ///////
	gc9c01_write_cmd_with_param(param->data_port, 0x7a, 0x51);  /////
	gc9c01_write_cmd_with_param(param->data_port, 0x6f, 0x49);   /////
	gc9c01_write_cmd_with_param(param->data_port, 0x78, 0x57);    ////
	gc9c01_write_cmd_with_param(param->data_port, 0xc9, 0x00);
	gc9c01_write_cmd_with_param(param->data_port, 0x67, 0x11); /////
	gc9c01_write_cmd_with_param(param->data_port, 0x51, 0x0a);
	gc9c01_write_cmd_with_param(param->data_port, 0x52, 0x7d);  /////
	gc9c01_write_cmd_with_param(param->data_port, 0x53, 0x0a);
	gc9c01_write_cmd_with_param(param->data_port, 0x54, 0x7d);   /////
	gc9c01_write_cmd_with_param(param->data_port, 0x46, 0x0a);
	gc9c01_write_cmd_with_param(param->data_port, 0x47, 0x2a);
	gc9c01_write_cmd_with_param(param->data_port, 0x48, 0x0a);
	gc9c01_write_cmd_with_param(param->data_port, 0x49, 0x1a);
	gc9c01_write_cmd_with_param(param->data_port, 0x44, 0x15);
	gc9c01_write_cmd_with_param(param->data_port, 0x45, 0x15);
	gc9c01_write_cmd_with_param(param->data_port, 0x73, 0x08);
	gc9c01_write_cmd_with_param(param->data_port, 0x74, 0x10);
			
//		gc9c01_write_cmd_with_param(0xf8, 0x16);
//		gc9c01_write_cmd_with_param(0xf9, 0x20);/////////123
			
	gc9c01_write_cmd_with_param(param->data_port, 0x56, 0x43);
	gc9c01_write_cmd_with_param(param->data_port, 0x57, 0x42);
	gc9c01_write_cmd_with_param(param->data_port, 0x58, 0x3c);
	gc9c01_write_cmd_with_param(param->data_port, 0x59, 0x64);
	gc9c01_write_cmd_with_param(param->data_port, 0x5a, 0x41);		
	gc9c01_write_cmd_with_param(param->data_port, 0x5b, 0x3c);
	gc9c01_write_cmd_with_param(param->data_port, 0x5c, 0x02);
	gc9c01_write_cmd_with_param(param->data_port, 0x5d, 0x3c);
	gc9c01_write_cmd_with_param(param->data_port, 0x5e, 0x1f);
	gc9c01_write_cmd_with_param(param->data_port, 0x60, 0x80);
	gc9c01_write_cmd_with_param(param->data_port, 0x61, 0x3f);
	gc9c01_write_cmd_with_param(param->data_port, 0x62, 0x21);
	gc9c01_write_cmd_with_param(param->data_port, 0x63, 0x07);
	gc9c01_write_cmd_with_param(param->data_port, 0x64, 0xe0);
	gc9c01_write_cmd_with_param(param->data_port, 0x65, 0x02);
	gc9c01_write_cmd_with_param(param->data_port, 0xca, 0x20);
	gc9c01_write_cmd_with_param(param->data_port, 0xcb, 0x52);
	gc9c01_write_cmd_with_param(param->data_port, 0xcc, 0x10);
	gc9c01_write_cmd_with_param(param->data_port, 0xcd, 0x42);
	gc9c01_write_cmd_with_param(param->data_port, 0xd0, 0x20);
	gc9c01_write_cmd_with_param(param->data_port, 0xd1, 0x52);
	gc9c01_write_cmd_with_param(param->data_port, 0xd2, 0x10);
	gc9c01_write_cmd_with_param(param->data_port, 0xd3, 0x42);
	gc9c01_write_cmd_with_param(param->data_port, 0xd4, 0x0a);
	gc9c01_write_cmd_with_param(param->data_port, 0xd5, 0x32);
	gc9c01_write_cmd_with_param(param->data_port, 0x80, 0x00);   ///
	gc9c01_write_cmd_with_param(param->data_port, 0xa0, 0x00);  ///
	gc9c01_write_cmd_with_param(param->data_port, 0x81, 0x06);  ////
	gc9c01_write_cmd_with_param(param->data_port, 0xa1, 0x08);  ///
	gc9c01_write_cmd_with_param(param->data_port, 0x82, 0x03); ///
	gc9c01_write_cmd_with_param(param->data_port, 0xa2, 0x03); ///
	gc9c01_write_cmd_with_param(param->data_port, 0x86, 0x14);////
	gc9c01_write_cmd_with_param(param->data_port, 0xa6, 0x14);///
	gc9c01_write_cmd_with_param(param->data_port, 0x87, 0x2c);///
	gc9c01_write_cmd_with_param(param->data_port, 0xa7, 0x26);///
	gc9c01_write_cmd_with_param(param->data_port, 0x83, 0x37); //
	gc9c01_write_cmd_with_param(param->data_port, 0xa3, 0x37); //
	gc9c01_write_cmd_with_param(param->data_port, 0x84, 0x35);  //
	gc9c01_write_cmd_with_param(param->data_port, 0xa4, 0x35); //
	gc9c01_write_cmd_with_param(param->data_port, 0x85, 0x3f); //
	gc9c01_write_cmd_with_param(param->data_port, 0xa5, 0x3f); //
	gc9c01_write_cmd_with_param(param->data_port, 0x88, 0x0a); //
	gc9c01_write_cmd_with_param(param->data_port, 0xa8, 0x0a);  //
	gc9c01_write_cmd_with_param(param->data_port, 0x89, 0x13); //
	gc9c01_write_cmd_with_param(param->data_port, 0xa9, 0x12); //
	gc9c01_write_cmd_with_param(param->data_port, 0x8a, 0x18); //
	gc9c01_write_cmd_with_param(param->data_port, 0xaa, 0x19); //
	gc9c01_write_cmd_with_param(param->data_port, 0x8b, 0x0a); //
	gc9c01_write_cmd_with_param(param->data_port, 0xab, 0x0a); //
	gc9c01_write_cmd_with_param(param->data_port, 0x8c, 0x17);//
	gc9c01_write_cmd_with_param(param->data_port, 0xac, 0x0b);//
	gc9c01_write_cmd_with_param(param->data_port, 0x8d, 0x1a); //
	gc9c01_write_cmd_with_param(param->data_port, 0xad, 0x09); //
	gc9c01_write_cmd_with_param(param->data_port, 0x8e, 0x1a); //
	gc9c01_write_cmd_with_param(param->data_port, 0xae, 0x08); //
	gc9c01_write_cmd_with_param(param->data_port, 0x8f, 0x1f);  //
	gc9c01_write_cmd_with_param(param->data_port, 0xaf, 0x00); //
	
	gc9c01_write_cmd_with_param(param->data_port, 0x90, 0x08); //
	gc9c01_write_cmd_with_param(param->data_port, 0xb0, 0x00); //
	gc9c01_write_cmd_with_param(param->data_port, 0x91, 0x10); //
	gc9c01_write_cmd_with_param(param->data_port, 0xb1, 0x06); //
	gc9c01_write_cmd_with_param(param->data_port, 0x92, 0x19); //
	gc9c01_write_cmd_with_param(param->data_port, 0xb2, 0x15); //
	gc9c01_write_cmd_with_param(param->data_port, 0xff, 0x00); //
	
	gc9c01_write_cmd_with_param(param->data_port, 0x11, 0x00); //
	PLATFORM_DELAY_MS(700);
	gc9c01_write_cmd_with_param(param->data_port, 0x29, 0x00); //
	PLATFORM_DELAY_MS(100);
	
	return vd_true;
}

/**
 * @brief : 设置位置
 *
 * @return vd_bool_t
 */
vd_bool_t gc9c01_set_pos(lcd_param_t *param, lcd_point_t *point)
{
	vd_check_return_val_error(param == vd_null,  vd_false, "func = %s, line = %d\n", __func__, __LINE__);
	vd_check_return_val_error(point == vd_null,  vd_false, "func = %s, line = %d\n", __func__, __LINE__);
	vd_check_return_val_error(gc9c01 == vd_null, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__);
	
	return vd_true;
}

/**
 * @brief : 显示
 *
 * @return vd_bool_t
 */
vd_bool_t gc9c01_display(lcd_param_t *param, lcd_point_t *point, vd_uint8_t *color)
{
	static unsigned char ddd=0;
	uint16_t *color1 = (uint16_t *)color;
	int i=0;
	unsigned int x,y;
	vd_uint8_t display_idx = 0;
	vd_int32_t ret;

	vd_check_return_val_error(gc9c01 == vd_null, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__);
	vd_check_return_val_error(gc9c01->send_flag != 0, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__); 		 
	vd_check_return_val_error(gc9c01->irq_en == 0, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__); 		 
	
	PLATFORM_DELAY_MS(2);
	gc9c01->send_flag = 1;

	gc9c01_set_window(param->data_port, point->x1, point->x2, point->y1, point->y2);      //设置刷图位置

	for(y = point->y1; y < point->y2 + 4; y++){
		if(y != point->y1){

			gc9c01_flush(param, gc9c01->display_buf[display_idx], (point->x2 + 1 - point->x1) * 2 * 2);   //DMA刷图

			display_idx++;
			if(display_idx >= sizeof(gc9c01->display_buf) / sizeof(gc9c01->display_buf[0])){
				display_idx = 0;
			}
		}
		
		if(y != point->y1 + 1){
			memcpy(gc9c01->display_buf[display_idx], (vd_uint8_t *)color1, (point->x2 + 1 - point->x1) * 2);
			color1 += (point->x2 + 1 - point->x1);
		}
	}

//	for(i = 0; i <= LCD_HEIGHT; i++){
//		if(i != 0){

//			gc9c01_flush(param, gc9c01->display_buf[display_idx], sizeof(gc9c01->display_buf[display_idx]));   //DMA刷图

//			display_idx++;
//			if(display_idx >= sizeof(gc9c01->display_buf) / sizeof(gc9c01->display_buf[0])){
//				display_idx = 0;
//			}
//		}

//		memcpy(gc9c01->display_buf[display_idx], color, sizeof(gc9c01->display_buf[display_idx]));
//		color += sizeof(gc9c01->display_buf[display_idx]);

//	}	

		if(ddd==0){
			ddd=1;
			co_delay_100us(1000);

		}
	gc9c01->send_flag=0;
	gc9c01->irq_en=0;	


	ret = vd_bsp_gpio_irq_enable(GC9C01_EXTI_PIN, VD_GPIO_IRQ_ENABLE);
	vd_check_return_val_error(ret < 0, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__); 		 

	return vd_true;
}

/**
 * @brief : 获取尺寸
 *
 * @return vd_bool_t
 */
vd_bool_t gc9c01_get_size(lcd_size_t *size)
{
	return true;
}

/**
 * @brief : 注册
 *
 * @return vd_bool_t
 */
vd_bool_t lcd_gc9c01_register(lcd_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null,  vd_false, "lcd register ops is null\n");
	
	ops->init = gc9c01_init;
	ops->set_pos = gc9c01_set_pos;
	ops->display = gc9c01_display;

	ops->get_size = gc9c01_get_size;
	
	return vd_true;
}