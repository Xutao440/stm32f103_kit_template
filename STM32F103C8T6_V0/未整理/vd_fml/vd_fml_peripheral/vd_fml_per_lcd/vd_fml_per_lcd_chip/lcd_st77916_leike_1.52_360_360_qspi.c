#include "lcd_st77916_leike_1.52_360_360_qspi.h"
#include "vd_log.h"
#include "common_time.h"
#include "vd_bsp_qspi/vd_bsp_qspi.h"
#include "malloc.h"

#define LCD_WIDTH							360
#define LCD_HEIGHT						360
#define TE_INTERVAL						14									//TE信号时间间隔(ms)
#define LEN_TO_TIME(data_len)	(data_len / 24000)	//根据数据长度计算发送所需时间（QSPI速率48M）（T = (data_len * 8) / (48000000 * 4) * 1000 ms）

#define FLUSH_WITH_DMA_LIST		OFF									//使用8008 DMA链表刷图（调试）
#if (FLUSH_WITH_DMA_LIST == OFF)
#define FLUSH_LEN_MAX					(4000 * 4)				//使用DMA刷图，mem->QSPI 源地址数据宽度32位
																								//8008平台DMA一次最大发送长度单位为4096（参考dma_start_IT函数），此处取4000
//vd_uint8_t draw_buf[2][FLUSH_LEN_MAX]   __attribute__((at(0x22000000 + LCD_WIDTH * LCD_HEIGHT * 2 * 2)));
vd_uint8_t *draw_buf[1];
#endif



typedef enum {
    LCD_CMD = 0,  // 	命令
		LCD_DATA,			//	数据
    LCD_DELAY,    // 	延时
}st77916_param_table_type_e;

typedef struct{
	vd_uint8_t cmd;
	vd_uint8_t data_len;
	vd_uint8_t data[20];
}st77916_param_t;

typedef struct{
	st77916_param_table_type_e 	type;	
	union {
		st77916_param_t 			param;	//参数内容
		vd_uint16_t 					delay;  // 延时时间,ms
	} u;
}st77916_param_table_t;

//LCD驱动
typedef struct{
	vd_uint64_t te_time;					//TE引脚信号到来时间
	vd_uint32_t flush_wait_time_max;//刷图最大等待时间
	vd_uint8_t 	send_flag 	: 1;
	vd_uint8_t 	te_sign			: 1;	//TE信号
	vd_uint8_t 	flush_ok		:	1;
	vd_uint8_t 	reserve			:	5;
}st77916_t;



vd_bool_t st77916_clean(lcd_param_t *param);

static const st77916_param_table_t st77916_init_table[] = {
    {LCD_CMD, 0xf0, 1, 0x08},
    {LCD_CMD, 0xf2, 1, 0x08},
    {LCD_CMD, 0x9b, 1, 0x51},
    {LCD_CMD, 0x86, 1, 0x53},
    {LCD_CMD, 0xf2, 1, 0x80},
    {LCD_CMD, 0xf0, 1, 0x00},
    {LCD_CMD, 0xf0, 1, 0x01},
    {LCD_CMD, 0xf1, 1, 0x01},
    {LCD_CMD, 0xb0, 1, 0x41},
    {LCD_CMD, 0xb1, 1, 0x32},
    {LCD_CMD, 0xb2, 1, 0x27},
    {LCD_CMD, 0xb4, 1, 0x66},
    {LCD_CMD, 0xb5, 1, 0x44},
    {LCD_CMD, 0xb6, 1, 0xa5},
    {LCD_CMD, 0xb7, 1, 0x40},
    {LCD_CMD, 0xba, 1, 0x00},
    {LCD_CMD, 0xbb, 1, 0x08},
    {LCD_CMD, 0xbc, 1, 0x08},
    {LCD_CMD, 0xbd, 1, 0x00},
    {LCD_CMD, 0xc0, 1, 0x80},
    {LCD_CMD, 0xc1, 1, 0x10},
    {LCD_CMD, 0xc2, 1, 0x37},
    {LCD_CMD, 0xc3, 1, 0x80},
    {LCD_CMD, 0xc4, 1, 0x10},
    {LCD_CMD, 0xc5, 1, 0x37},
    {LCD_CMD, 0xc6, 1, 0xa9},
    {LCD_CMD, 0xc7, 1, 0x41},
    {LCD_CMD, 0xc8, 1, 0x51},
    {LCD_CMD, 0xc9, 1, 0xa9},
    {LCD_CMD, 0xca, 1, 0x41},
    {LCD_CMD, 0xcb, 1, 0x51},
    {LCD_CMD, 0xd0, 1, 0x91},
    {LCD_CMD, 0xd1, 1, 0x68},
    {LCD_CMD, 0xd2, 1, 0x69},
    {LCD_CMD, 0xf5, 2, 0x00, 0xa5},
    {LCD_CMD, 0xdd, 1, 0x3f},
    {LCD_CMD, 0xde, 1, 0x3f},
    {LCD_CMD, 0xf1, 1, 0x10},
    {LCD_CMD, 0xf0, 1, 0x00},
    {LCD_CMD, 0xf0, 1, 0x02},
    {LCD_CMD, 0xe0, 14, 0xf0, 0x05, 0x0a, 0x09, 0x08, 0x05, 0x32, 0x44, 0x49, 0x38, 0x14, 0x14, 0x2d, 0x34},
    {LCD_CMD, 0xe1, 14, 0xf0, 0x05, 0x0a, 0x09, 0x08, 0x15, 0x31, 0x43, 0x49, 0x17, 0x14, 0x14, 0x2d, 0x33},
    {LCD_CMD, 0xf0, 1, 0x10},
    {LCD_CMD, 0xf3, 1, 0x10},
    {LCD_CMD, 0xe0, 1, 0x0a},
    {LCD_CMD, 0xe1, 1, 0x00},
    {LCD_CMD, 0xe2, 1, 0x00},
    {LCD_CMD, 0xe3, 1, 0x00},
    {LCD_CMD, 0xe4, 1, 0xe0},
    {LCD_CMD, 0xe5, 1, 0x06},
    {LCD_CMD, 0xe6, 1, 0x21},
    {LCD_CMD, 0xe7, 1, 0x00},
    {LCD_CMD, 0xe8, 1, 0x05},
    {LCD_CMD, 0xe9, 1, 0x82},
    {LCD_CMD, 0xea, 1, 0xdf},
    {LCD_CMD, 0xeb, 1, 0x89},
    {LCD_CMD, 0xec, 1, 0x20},
    {LCD_CMD, 0xed, 1, 0x14},
    {LCD_CMD, 0xee, 1, 0xff},
    {LCD_CMD, 0xef, 1, 0x00},
    {LCD_CMD, 0xf8, 1, 0xff},
    {LCD_CMD, 0xf9, 1, 0x00},
    {LCD_CMD, 0xfa, 1, 0x00},
    {LCD_CMD, 0xfb, 1, 0x30},
    {LCD_CMD, 0xfc, 1, 0x00},
    {LCD_CMD, 0xfd, 1, 0x00},
    {LCD_CMD, 0xfe, 1, 0x00},
    {LCD_CMD, 0xff, 1, 0x00},
    {LCD_CMD, 0x60, 1, 0x42},
    {LCD_CMD, 0x61, 1, 0xe0},
    {LCD_CMD, 0x62, 1, 0x40},
    {LCD_CMD, 0x63, 1, 0x40},
    {LCD_CMD, 0x64, 1, 0x02},
    {LCD_CMD, 0x65, 1, 0x00},
    {LCD_CMD, 0x66, 1, 0x40},
    {LCD_CMD, 0x67, 1, 0x03},
    {LCD_CMD, 0x68, 1, 0x00},
    {LCD_CMD, 0x69, 1, 0x00},
    {LCD_CMD, 0x6a, 1, 0x00},
    {LCD_CMD, 0x6b, 1, 0x00},
    {LCD_CMD, 0x70, 1, 0x42},
    {LCD_CMD, 0x71, 1, 0xe0},
    {LCD_CMD, 0x72, 1, 0x40},
    {LCD_CMD, 0x73, 1, 0x40},
    {LCD_CMD, 0x74, 1, 0x02},
    {LCD_CMD, 0x75, 1, 0x00},
    {LCD_CMD, 0x76, 1, 0x40},
    {LCD_CMD, 0x77, 1, 0x03},
    {LCD_CMD, 0x78, 1, 0x00},
    {LCD_CMD, 0x79, 1, 0x00},
    {LCD_CMD, 0x7a, 1, 0x00},
    {LCD_CMD, 0x7b, 1, 0x00},
    {LCD_CMD, 0x80, 1, 0x48},
    {LCD_CMD, 0x81, 1, 0x00},
    {LCD_CMD, 0x82, 1, 0x05},
    {LCD_CMD, 0x83, 1, 0x02},
    {LCD_CMD, 0x84, 1, 0xdd},
    {LCD_CMD, 0x85, 1, 0x00},
    {LCD_CMD, 0x86, 1, 0x00},
    {LCD_CMD, 0x87, 1, 0x00},
    {LCD_CMD, 0x88, 1, 0x48},
    {LCD_CMD, 0x89, 1, 0x00},
    {LCD_CMD, 0x8a, 1, 0x07},
    {LCD_CMD, 0x8b, 1, 0x02},
    {LCD_CMD, 0x8c, 1, 0xdf},
    {LCD_CMD, 0x8d, 1, 0x00},
    {LCD_CMD, 0x8e, 1, 0x00},
    {LCD_CMD, 0x8f, 1, 0x00},
    {LCD_CMD, 0x90, 1, 0x48},
    {LCD_CMD, 0x91, 1, 0x00},
    {LCD_CMD, 0x92, 1, 0x09},
    {LCD_CMD, 0x93, 1, 0x02},
    {LCD_CMD, 0x94, 1, 0xe1},
    {LCD_CMD, 0x95, 1, 0x00},
    {LCD_CMD, 0x96, 1, 0x00},
    {LCD_CMD, 0x97, 1, 0x00},
    {LCD_CMD, 0x98, 1, 0x48},
    {LCD_CMD, 0x99, 1, 0x00},
    {LCD_CMD, 0x9a, 1, 0x0b},
    {LCD_CMD, 0x9b, 1, 0x02},
    {LCD_CMD, 0x9c, 1, 0xe3},
    {LCD_CMD, 0x9d, 1, 0x00},
    {LCD_CMD, 0x9e, 1, 0x00},
    {LCD_CMD, 0x9f, 1, 0x00},
    {LCD_CMD, 0xa0, 1, 0x48},
    {LCD_CMD, 0xa1, 1, 0x00},
    {LCD_CMD, 0xa2, 1, 0x04},
    {LCD_CMD, 0xa3, 1, 0x02},
    {LCD_CMD, 0xa4, 1, 0xdc},
    {LCD_CMD, 0xa5, 1, 0x00},
    {LCD_CMD, 0xa6, 1, 0x00},
    {LCD_CMD, 0xa7, 1, 0x00},
    {LCD_CMD, 0xa8, 1, 0x48},
    {LCD_CMD, 0xa9, 1, 0x00},
    {LCD_CMD, 0xaa, 1, 0x06},
    {LCD_CMD, 0xab, 1, 0x02},
    {LCD_CMD, 0xac, 1, 0xde},
    {LCD_CMD, 0xad, 1, 0x00},
    {LCD_CMD, 0xae, 1, 0x00},
    {LCD_CMD, 0xaf, 1, 0x00},
    {LCD_CMD, 0xb0, 1, 0x48},
    {LCD_CMD, 0xb1, 1, 0x00},
    {LCD_CMD, 0xb2, 1, 0x08},
    {LCD_CMD, 0xb3, 1, 0x02},
    {LCD_CMD, 0xb4, 1, 0xe0},
    {LCD_CMD, 0xb5, 1, 0x00},
    {LCD_CMD, 0xb6, 1, 0x00},
    {LCD_CMD, 0xb7, 1, 0x00},
    {LCD_CMD, 0xb8, 1, 0x48},
    {LCD_CMD, 0xb9, 1, 0x00},
    {LCD_CMD, 0xba, 1, 0x0a},
    {LCD_CMD, 0xbb, 1, 0x02},
    {LCD_CMD, 0xbc, 1, 0xe2},
    {LCD_CMD, 0xbd, 1, 0x00},
    {LCD_CMD, 0xbe, 1, 0x00},
    {LCD_CMD, 0xbf, 1, 0x00},
    {LCD_CMD, 0xc0, 1, 0x12},
    {LCD_CMD, 0xc1, 1, 0xaa},
    {LCD_CMD, 0xc2, 1, 0x65},
    {LCD_CMD, 0xc3, 1, 0x74},
    {LCD_CMD, 0xc4, 1, 0x47},
    {LCD_CMD, 0xc5, 1, 0x56},
    {LCD_CMD, 0xc6, 1, 0x00},
    {LCD_CMD, 0xc7, 1, 0x88},
    {LCD_CMD, 0xc8, 1, 0x99},
    {LCD_CMD, 0xc9, 1, 0x33},
    {LCD_CMD, 0xd0, 1, 0x21},
    {LCD_CMD, 0xd1, 1, 0xaa},
    {LCD_CMD, 0xd2, 1, 0x65},
    {LCD_CMD, 0xd3, 1, 0x74},
    {LCD_CMD, 0xd4, 1, 0x47},
    {LCD_CMD, 0xd5, 1, 0x56},
    {LCD_DELAY,10},
    {LCD_CMD, 0xd6, 1, 0x00},
    {LCD_DELAY,10},
    {LCD_CMD, 0xd7, 1, 0x88},
    {LCD_DELAY,10},
    {LCD_CMD, 0xd8, 1, 0x99},
    {LCD_DELAY,10},
    {LCD_CMD, 0xd9, 1, 0x33},
    {LCD_DELAY,10},
    {LCD_CMD, 0xf3, 1, 0x01},
    {LCD_DELAY,10},
    {LCD_CMD, 0xf0, 1, 0x00},
    {LCD_DELAY,10},
    {LCD_CMD, 0xf0, 1, 0x01},
    {LCD_DELAY,10},
    {LCD_CMD, 0xf1, 1, 0x01},
    {LCD_DELAY,10},
    {LCD_CMD, 0xa0, 1, 0x0b},
    {LCD_DELAY,10},
    {LCD_CMD, 0xa3, 1, 0x2a},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_CMD, 0xa3, 1, 0x2b},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_DELAY,1},
    {LCD_CMD, 0xa3, 1, 0x2c},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_DELAY,1},
    {LCD_CMD, 0xa3, 1, 0x2d},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_DELAY,1},
    {LCD_CMD, 0xa3, 1, 0x2e},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_DELAY,1},
    {LCD_CMD, 0xa3, 1, 0x2f},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_DELAY,1},
    {LCD_CMD, 0xa3, 1, 0x30},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_DELAY,1},
    {LCD_CMD, 0xa3, 1, 0x31},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_DELAY,1},
    {LCD_CMD, 0xa3, 1, 0x32},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_DELAY,1},
    {LCD_CMD, 0xa3, 1, 0x33},
    {LCD_CMD, 0xa5, 1, 0xc3},
    {LCD_DELAY,1},
    {LCD_CMD, 0xa0, 1, 0x09},
    {LCD_CMD, 0xf1, 1, 0x10},
    {LCD_CMD, 0xf0, 1, 0x00},
    {LCD_CMD, 0x2a, 4, 0x00, 0x00, 0x01, 0x67},
    {LCD_CMD, 0x2b, 4, 0x01, 0x68, 0x01, 0x68},
    {LCD_CMD, 0x4d, 1, 0x00},
    {LCD_CMD, 0x4e, 1, 0x00},
    {LCD_CMD, 0x4f, 1, 0x00},
    {LCD_CMD, 0x4c, 1, 0x01},
    {LCD_DELAY,10},
    {LCD_CMD, 0x4c, 1, 0x00},
    {LCD_CMD, 0x2a, 4, 0x00, 0x00, 0x01, 0x67},
    {LCD_CMD, 0x2b, 4, 0x00, 0x00, 0x01, 0x67},
    {LCD_CMD, 0x21},
    {LCD_CMD, 0x3a, 1, 0x05},
    {LCD_CMD, 0x36, 1, 0x00},	
		{LCD_CMD, 0x35, 1, 0x00},
    {LCD_CMD, 0x11},
    {LCD_DELAY,120},
    {LCD_CMD, 0x29},
    {LCD_CMD, 0x2c},
};

// 设置坐标命令表
static st77916_param_table_t st77916_set_pos_table[] = {
	{LCD_CMD, 0x2a},
	{LCD_CMD, 0x2b},
	{LCD_CMD, 0x2c},
};

st77916_t *st77916 = vd_null;

/**
* @brief : lcd写命令
 *
 * @return vd_bool_t
 */
static vd_bool_t st77916_write_param_cmd(lcd_param_t *lcd_param, vd_uint8_t cmd, const vd_uint8_t *data, vd_uint16_t data_len)
{
	vd_int32_t ret;
	vd_qspi_tx_cfg_t qspi_cfg = {0};	
	vd_uint8_t 	my_data[64] = {0};
	vd_uint16_t buf_len;
	
	qspi_cfg.line = QSPI_LINE_X1;
	
	buf_len = 4;
	my_data[0] = 0x02;
	my_data[1] = 0x00;
	my_data[2] = cmd;
	my_data[3] = 0x00;
	if(data_len){		
		
		vd_check_return_val_error(data_len > sizeof(my_data) - 4,  vd_false, "data_len > buf len\n");
		vd_check_return_val_error(data == vd_null,  vd_false, "data_len > 0 but data is null\n");
		
		memcpy(&my_data[4], data, data_len);
		buf_len += data_len;		
	}
	
	vd_bsp_qspim_cs_control(lcd_param->data_port, 0);
	
	ret = vd_bsp_qspim_write(lcd_param->data_port, &qspi_cfg, my_data, buf_len);
	
	vd_bsp_qspim_cs_control(lcd_param->data_port, 1);
	
	vd_check_return_val_error(ret < 0,  vd_false, "lcd write cmd error!!! ret = %d\n", ret);
	
	return vd_true;
}

/**
* @brief : lcd传输完成回调
 *
 * @return vd_void_t
 */
static vd_void_t st77916_tx_cplt_callback(vd_void_t *arg)
{
	lcd_param_t *lcd_param = (lcd_param_t *)arg;
	
	vd_bsp_qspim_cs_control(lcd_param->data_port, 1);
	
	st77916->flush_ok = vd_true;
}

/**
* @brief : lcd写数据
 *
 * @return vd_bool_t
 */
static vd_bool_t st77916_write_param_data(lcd_param_t *lcd_param, const vd_uint8_t *data, vd_uint32_t data_len)
{
	vd_int32_t ret;
	vd_uint64_t t;
	vd_qspi_tx_cfg_t qspi_cfg = {0};	
	
	qspi_cfg.address = 0x003C00;
	qspi_cfg.address_len = 3;
	qspi_cfg.instruct = 0x32;
	qspi_cfg.instruct_len = 1;
	qspi_cfg.line = QSPI_LINE_X4;	
	
	t = common_time_get_current_time_ms();
	while(st77916->flush_ok == vd_false){
	if(common_time_get_current_time_ms() - t > st77916->flush_wait_time_max){
		break;
	}
	}
	
	vd_bsp_qspim_cs_control(lcd_param->data_port, 0);
	
	st77916->flush_ok = vd_false;
		
	ret = vd_bsp_qspim_write_dma(lcd_param->data_port, &qspi_cfg, data, data_len, st77916_tx_cplt_callback, lcd_param);
	if(ret < 0){
		log_error("lcd write data error!!! ret = %d\n", ret);
		vd_bsp_qspim_cs_control(lcd_param->data_port, 1);
		return vd_false;
	}
	
	return vd_true;
}

/**
* @brief : lcd通过参数表写参数
 *
 * @return vd_bool_t
 */
static vd_bool_t st77916_write_param(lcd_param_t *lcd_param, const st77916_param_table_t *param_table, vd_uint16_t table_size)
{
	vd_bool_t		ret = vd_false;
	vd_uint16_t i;	
	
	for(i = 0; i < table_size; i++){
		switch(param_table[i].type){
				case LCD_DELAY:{
												PLATFORM_DELAY_MS(param_table[i].u.delay);
												ret = vd_true;
				}break;
				case LCD_CMD:{
												ret = st77916_write_param_cmd(	lcd_param, \
																												param_table[i].u.param.cmd, \
																												param_table[i].u.param.data, \
																												param_table[i].u.param.data_len);
				}break;
				case LCD_DATA:{
												ret = st77916_write_param_data(lcd_param, param_table[i].u.param.data, param_table[i].u.param.data_len);
				}
				default:{
												ret = vd_false;
				}break;
		}
		
		if(ret == vd_false){
			break;
		}
	}
	
	return ret;
}

/**
* @brief : lcd TE引脚回调
 *
 * @return vd_void_t
 */
static vd_void_t st77916_te_callback(vd_void_t *arg)
{
	vd_check_return_error(st77916 == vd_null, "func = %s, line = %d\r\n", __func__, __LINE__);
	
	st77916->te_time = common_time_get_current_time_ms();
	st77916->te_sign = vd_true;
}

/**
 * @brief : lcd初始化
 *
 * @return vd_bool_t
 */
vd_bool_t st77916_init(lcd_param_t *param)
{
	vd_int32_t ret;
	vd_uint32_t	i;
	
	//参数校验
	vd_check_return_val_error(param == vd_null,  vd_false, "param is null\n");
	vd_check_return_val_error(param->data_port < VD_PORT_QSPIM0 || param->data_port >= VD_PORT_QSPIM_MAX,  vd_false, "data_port error\n");
	
	st77916 = PLATFORM_MALLOC(sizeof(st77916_t));
	vd_check_return_val_error(st77916 == vd_null,  vd_false, "st77916 driver malloc error\r\n");
	memset(st77916, 0, sizeof(st77916_t));
	st77916->flush_ok = vd_true;

	/* qspi初始化 */
	ret = vd_bsp_qspim_init(param->data_port);
	vd_check_return_val_error(ret < 0,  vd_false, "lcd qspim init error!!! ret = %d\n", ret);
	
	//TE引脚初始化
	ret = vd_bsp_gpio_requst_irq(param->TE_port, VD_GPIO_IRQ_MODE_FALLING, st77916_te_callback, vd_null);
	vd_check_return_val_error(ret < 0,  vd_false, "lcd exti	init error!!! ret = %d\n", ret);
	ret = vd_bsp_gpio_irq_enable(param->TE_port, VD_GPIO_IRQ_ENABLE);
	vd_check_return_val_error(ret < 0, vd_false, "ret = %d, func = %s, line = %d\r\n", ret, __func__, __LINE__); 
	
	//写屏幕初始化参数
	ret = st77916_write_param(param, st77916_init_table, vd_array_size(st77916_init_table));
	vd_check_return_val_error(ret == vd_false,  vd_false, "lcd write init param error!!!\n");
	
	draw_buf[0] = mymalloc(SRAMEX, FLUSH_LEN_MAX);
	
	return vd_true;
}

/**
 * @brief : lcd轮询
 *
 */
vd_void_t st77916_loop(lcd_param_t *param)
{

}
	
/**
 * @brief : lcd设置坐标
 *
 * @return vd_bool_t
 */
static vd_bool_t st77916_set_pos(lcd_param_t *lcd_param, lcd_point_t *point)
{
	vd_check_return_val_error(lcd_param == vd_null, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__);
	vd_check_return_val_error(point == vd_null, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__);
	vd_check_return_val_error(st77916 == vd_null, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__);
	
	st77916_set_pos_table[0].u.param.data[0] = point->x1 >> 8;
  st77916_set_pos_table[0].u.param.data[1] = point->x1 & 0xff;
  st77916_set_pos_table[0].u.param.data[2] = point->x2 >> 8;
  st77916_set_pos_table[0].u.param.data[3] = point->x2 & 0xff;
	st77916_set_pos_table[0].u.param.data_len = 4;
	
	st77916_set_pos_table[1].u.param.data[0] = point->y1 >> 8;
  st77916_set_pos_table[1].u.param.data[1] = point->y1 & 0xff;
  st77916_set_pos_table[1].u.param.data[2] = point->y2 >> 8;
  st77916_set_pos_table[1].u.param.data[3] = point->y2 & 0xff;
	st77916_set_pos_table[1].u.param.data_len = 4;
	
	return st77916_write_param(lcd_param, st77916_set_pos_table, vd_array_size(st77916_set_pos_table));
}

#if (FLUSH_WITH_DMA_LIST == ON)
/*==================================================== DMA链表刷图 ==============================================================================*/
 #include "driver_dma.h"
 #include "driver_spi.h"
 static dma_LinkParameter_t LinkParameter;
 static DMA_LLI_InitTypeDef Link_Channel[10];
 extern SPI_HandleTypeDef *get_spi_handle(void);
 void *arg;
 void gc9c01_displayeeee(lcd_param_t *param, uint32_t pixel_count, uint16_t *data, void (*callback)(void))
 {
     uint32_t i;
     uint32_t total_count = pixel_count / 2;   // accoding source width
     uint8_t link_count = total_count / 4000;
 		SPI_HandleTypeDef *hspi = get_spi_handle();
 		arg = param;
	
 		while(st77916->flush_ok == vd_false){};
 		st77916->flush_ok = vd_false;
    
     if(total_count % 4000)
     {
         link_count++;
     }
    
     for (i = 0; i < link_count; i++)
     {
         uint8_t all_set = (total_count <= 4000);
        
         LinkParameter.SrcAddr          = (uint32_t)&data[i * 8000];
         LinkParameter.DstAddr          = (uint32_t)&SPIM0->DR;
         if(all_set)
         {
             LinkParameter.NextLink     = 0;
         }
         else
         {
             LinkParameter.NextLink     = (uint32_t)&Link_Channel[i + 1];
         }
         LinkParameter.Data_Flow        = DMA_M2P_DMAC;
         LinkParameter.Request_ID       = 1;
         LinkParameter.Source_Inc       = DMA_ADDR_INC_INC;
         LinkParameter.Desination_Inc   = DMA_ADDR_INC_NO_CHANGE;
         LinkParameter.Source_Width     = DMA_TRANSFER_WIDTH_32;
         LinkParameter.Desination_Width = DMA_TRANSFER_WIDTH_8;
         LinkParameter.Burst_Len        = DMA_BURST_LEN_16; //4
         LinkParameter.Size             = all_set ? (total_count) : 4000;
         LinkParameter.gather_enable    = 0;
         LinkParameter.scatter_enable   = 0;
         total_count -= 4000;

         dma_linked_list_init(&Link_Channel[i], &LinkParameter);
     }

     hspi->Init.Frame_Size              = SPI_FRAME_SIZE_8BIT;
     hspi->MultWireParam.Wire_X2X4      = Wire_X4;
     hspi->MultWireParam.InstructLength = INST_8BIT;
     hspi->MultWireParam.Instruct       = 0x32;
     hspi->MultWireParam.AddressLength  = ADDR_24BIT;
     hspi->MultWireParam.Address        = 0x003C00;
    
     vd_bsp_qspim_cs_control(param->data_port, 0);
     spi_master_transmit_X2X4_DMA(hspi);
     dma_linked_list_start_IT(Link_Channel, &LinkParameter, 0);
 }

 __attribute__((section("ram_code"))) void dma_isr(void)
 {
 		if(dma_get_tfr_Status(0)){
   //  while(__SPI_IS_BUSY(SPIM1));
      while(__SPI_IS_BUSY(SPIM0));
	
     /* Clear Transfer complete status */
     dma_clear_tfr_Status(0);
     /* channel Transfer complete interrupt disable */
     dma_tfr_interrupt_disable(0);

     __SPI_DISABLE(get_spi_handle()->SPIx);

    st77916_tx_cplt_callback(arg);

 	}
 }
/*==================================================== DMA链表刷图 ==============================================================================*/
#endif

/**
 * @brief : lcd显示
 *
 * @return vd_bool_t
 */
vd_bool_t st77916_display(lcd_param_t *param, lcd_point_t *point, vd_uint8_t *color)
{
	vd_uint8_t display_idx = 0;
	vd_int32_t ret;

	vd_check_return_val_error(st77916 == vd_null, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__);
	vd_check_return_val_error(st77916->send_flag != 0, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__); 		 

	st77916->send_flag = 1;
	
#if (FLUSH_WITH_DMA_LIST == ON)
	gc9c01_displayeeee(param, (point->x2 - point->x1 + 1) * (point->y2 - point->y1 + 1), (uint16_t *)color, vd_null);
	while(st77916->flush_ok == vd_false){};
#else
	
	vd_uint32_t data_len = (point->x2 - point->x1 + 1) * (point->y2 - point->y1 + 1) * 2;
	vd_uint32_t tx_cnt = data_len / FLUSH_LEN_MAX;
  vd_uint32_t extra_len = data_len % FLUSH_LEN_MAX;//多余长度
	vd_uint32_t tx_time = LEN_TO_TIME(data_len) * 3 + 1; // * 3 = 实际时间
	vd_uint32_t tx_len;
	vd_uint32_t i;
	vd_uint8_t *p_disp = color;
  
	if(extra_len){
		tx_cnt++;
	}
	
	if(common_time_get_current_time_ms() + tx_time - st77916->te_time > TE_INTERVAL || !st77916->te_time){//需要等TE信号	
		st77916->te_sign = vd_false;	
		while(!st77916->te_sign){};//等待TE信号
	}
	for(i = 0; i < tx_cnt; i++){

		if(extra_len && (i == tx_cnt - 1)){//最后一包发送剩余数据
			tx_len = extra_len;
		}else{
			tx_len = FLUSH_LEN_MAX;
		}

//		display_idx++;
//		if(display_idx >= 2){
//			display_idx = 0;
//		}
		
    if(color){
			if(i == tx_cnt - 1){//最后一包做拷贝
				memcpy(draw_buf[display_idx], color, tx_len);
				p_disp = draw_buf[display_idx];
			}else{
				p_disp = color;
			}
    }else{//清屏
      memset(draw_buf[display_idx], 0, tx_len);
			p_disp = draw_buf[display_idx];
    }
		
		st77916->flush_wait_time_max = LEN_TO_TIME(tx_len) * 3 + 3;//防止卡DMA中断造成死等

    //转换为32位数据宽度对应的长度
    if(tx_len % 4){
      memset(&p_disp[tx_len], 0, 4 - (tx_len % 4));
      tx_len = tx_len / 4 + 1;
    }else{
      tx_len = tx_len / 4;
    }

		ret = st77916_write_param_data(param, p_disp, tx_len);//DMA刷图
		vd_check_return_val_error(ret == vd_false, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__);
//		printf("st77916_write_param_data end\n\r");
		color += FLUSH_LEN_MAX;
	}
#endif
	
	st77916->send_flag = 0;

	return vd_true;
}

/**
* @brief : lcd休眠
 *
 * @return vd_bool_t
 */
vd_bool_t st77916_sleep(lcd_param_t *param)
{	

}

/**
* @brief : lcd唤醒
 *
 * @return vd_bool_t
 */
vd_bool_t st77916_wakeup(lcd_param_t *param)
{

}

/**
 * @brief : lcd清屏
 *
 * @return vd_bool_t
 */
vd_bool_t st77916_clean(lcd_param_t *param)
{
	vd_int32_t ret;
	lcd_point_t point;
#if (FLUSH_WITH_DMA_LIST == ON)
	arg = param;
#endif	 		 

	point.x1 = 0;
	point.y1 = 0;
	point.x2 = LCD_WIDTH - 1;
	point.y2 = LCD_HEIGHT - 1;
	ret = st77916_set_pos(param, &point);      //设置刷图位置
	vd_check_return_val_error(ret == vd_false, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__); 		 

  ret = st77916_display(param, &point, vd_null);
  vd_check_return_val_error(ret == vd_false, vd_false, "func = %s, line = %d\r\n", __func__, __LINE__);

	return vd_true;
}

/**
 * @brief : lcd背光控制
 *
 * @return vd_bool_t
 */
vd_bool_t st77916_backlight_control(lcd_param_t *param, vd_uint32_t val)
{

}

/**
* @brief : lcd获取尺寸
 *
 * @return vd_bool_t
 */
vd_bool_t st77916_backlight_get_size(lcd_size_t *size)
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
vd_bool_t lcd_st77916_register(lcd_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null,  vd_false, "lcd register ops is null\n");
	
	ops->init = st77916_init;
//	ops->loop = st77916_loop;
//	ops->sleep = st77916_sleep;
//	ops->wakeup = st77916_wakeup;
	ops->set_pos = st77916_set_pos;
	ops->display = st77916_display;
//	
	ops->clean = st77916_clean;
//	ops->backlight_control = st77916_backlight_control;
	ops->get_size = st77916_backlight_get_size;
	
	return vd_true;
}
