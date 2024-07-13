#ifndef _VD_BSP_GLOBAL_SWITCH_H
#define _VD_BSP_GLOBAL_SWITCH_H

#include "base_type.h"
struct vd_bsp_global_switch
{
    vd_bool_t (*INT_START)(vd_void_t);
    vd_bool_t (*INT_STOP)(vd_void_t);
    vd_bool_t (*INT_DISABLE)(vd_void_t);
    vd_bool_t (*INT_RESTORE)(vd_void_t);
   
};

vd_bool_t vd_global_int_start(vd_void_t);
vd_bool_t vd_global_int_stop(vd_void_t);
vd_bool_t vd_global_int_disable(vd_void_t);
vd_bool_t vd_global_int_restore(vd_void_t);

typedef vd_void_t (*vd_bsp_global_switch_register_t)(struct vd_bsp_global_switch *ops);

#endif // !_VD_BSP_GLOBAL_SWITCH_H
