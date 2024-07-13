#ifndef __VD_FML_PER_TP_TYPE_H
#define __VD_FML_PER_TP_TYPE_H

#include "base.h"
#include "vd_bsp_type.h"




//坐标
typedef struct {
	vd_uint16_t x;
	vd_uint16_t y;
}tp_point_t;

//tp初始化参数
typedef struct{
	vd_bsp_port_e	power_port;											//电源端口
	vd_bsp_port_e	reset_port;											//复位端口
	vd_bsp_port_e	inter_port;											//中断端口
	vd_bsp_port_e	data_port;											//数据端口
	
	vd_bool_t (*point_report)(tp_point_t *point);//tp数据上报
}tp_init_t;

//TP芯片操作接口
typedef struct{
	vd_bool_t (*init)(tp_init_t *tp_init);
	vd_void_t (*loop)(vd_void_t);
}tp_chip_port_t;

//tp注册函数
typedef vd_bool_t (*tp_chip_port_register_t)(tp_chip_port_t *port);


#endif
