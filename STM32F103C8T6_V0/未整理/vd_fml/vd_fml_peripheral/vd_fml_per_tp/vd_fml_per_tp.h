#ifndef __VD_FML_PER_TP_H
#define __VD_FML_PER_TP_H

#include "vd_fml_per_tp_type.h"




/**
 * @brief : tp��ʼ��
 *
 * @return vd_true
 * @return vd_false
 */
vd_bool_t vd_tp_init(vd_void_t);

/**
 * @brief : tp��ѯ��TP��ȡ����,���ݴ������
 *
 * @return vd_void_t
 */
vd_void_t vd_tp_loop(vd_void_t);

/**
 * @brief :�Ӷ��л�ȡtp����,��Ӧ�ò���ã�
 *
 * @param point: tp����
 * @return vd_bool_t
 */
vd_bool_t vd_tp_read(tp_point_t* point);

#endif
