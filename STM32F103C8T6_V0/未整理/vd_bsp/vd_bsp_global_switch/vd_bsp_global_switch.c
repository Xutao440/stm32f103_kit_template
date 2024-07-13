#include "vd_bsp_global_switch/vd_bsp_global_switch.h"

#include "platform_config.h"
#ifndef PLATFORM_GLOBAL_SWITCH_REGISTER
#define PLATFORM_GLOBAL_SWITCH_REGISTER vd_null
#endif 

struct vd_bsp_global_switch global_switch_ops;
vd_bsp_global_switch_register_t  vd_bsp_global_switch_register = PLATFORM_GLOBAL_SWITCH_REGISTER;



vd_bool_t vd_global_int_start(vd_void_t)
{
    if(vd_bsp_global_switch_register){
        vd_bsp_global_switch_register(&global_switch_ops);
    }
    global_switch_ops.INT_START();
    return vd_true;
}

vd_bool_t vd_global_int_disable(vd_void_t)
{
    if(vd_bsp_global_switch_register){
        vd_bsp_global_switch_register(&global_switch_ops);
    }
    global_switch_ops.INT_DISABLE();
    return vd_true;
}

vd_bool_t vd_global_int_stop(vd_void_t)
{
    global_switch_ops.INT_STOP();
    return vd_true;
}

vd_bool_t vd_global_int_restore(vd_void_t)
{
    global_switch_ops.INT_RESTORE();
    return vd_true;
}