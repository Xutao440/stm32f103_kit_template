#ifndef __VD_FML_PER_TP_TYPE_H
#define __VD_FML_PER_TP_TYPE_H

#include "base.h"
#include "vd_bsp_type.h"




//����
typedef struct {
	vd_uint16_t x;
	vd_uint16_t y;
}tp_point_t;

//tp��ʼ������
typedef struct{
	vd_bsp_port_e	power_port;											//��Դ�˿�
	vd_bsp_port_e	reset_port;											//��λ�˿�
	vd_bsp_port_e	inter_port;											//�ж϶˿�
	vd_bsp_port_e	data_port;											//���ݶ˿�
	
	vd_bool_t (*point_report)(tp_point_t *point);//tp�����ϱ�
}tp_init_t;

//TPоƬ�����ӿ�
typedef struct{
	vd_bool_t (*init)(tp_init_t *tp_init);
	vd_void_t (*loop)(vd_void_t);
}tp_chip_port_t;

//tpע�ắ��
typedef vd_bool_t (*tp_chip_port_register_t)(tp_chip_port_t *port);


#endif
