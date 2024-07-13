#ifndef __VD_FML_PER_TP_H
#define __VD_FML_PER_TP_H

#include "vd_fml_per_tp_type.h"




/**
 * @brief : tp初始化
 *
 * @return vd_true
 * @return vd_false
 */
vd_bool_t vd_tp_init(vd_void_t);

/**
 * @brief : tp轮询，TP读取数据,数据存入队列
 *
 * @return vd_void_t
 */
vd_void_t vd_tp_loop(vd_void_t);

/**
 * @brief :从队列获取tp坐标,供应用层调用，
 *
 * @param point: tp坐标
 * @return vd_bool_t
 */
vd_bool_t vd_tp_read(tp_point_t* point);

#endif
