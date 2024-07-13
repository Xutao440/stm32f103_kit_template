#include "vdchip_40xx_50xx_global_switch/vdchip_40xx_50xx_global_switch.h"
#include "plf.h"

vd_bool_t fr8000_global_int_start(vd_void_t)
{
    GLOBAL_INT_START();
}

vd_bool_t fr8000_global_int_stop(vd_void_t)
{
    GLOBAL_INT_STOP();
}
vd_bool_t fr8000_global_int_disable(vd_void_t)
{
    GLOBAL_INT_DISABLE();
}
vd_bool_t fr8000_global_int_restore(vd_void_t)
{
    GLOBAL_INT_RESTORE();
}
vd_void_t fr8000_global_switch_register(struct vd_bsp_global_switch *ops)
{
    ops->INT_START = fr8000_global_int_start;
    ops->INT_STOP = fr8000_global_int_stop;
    ops->INT_DISABLE = fr8000_global_int_disable;
    ops->INT_RESTORE = fr8000_global_int_restore;
}
