#ifndef _VDCHIP_40XX_50XX_GLOBAL_SWITCH_H
#define _VDCHIP_40XX_50XX_GLOBAL_SWITCH_H

#include "vd_bsp_global_switch/vd_bsp_global_switch.h"

vd_bool_t fr8000_global_int_start(vd_void_t);
vd_bool_t fr8000_global_int_stop(vd_void_t);
vd_bool_t fr8000_global_int_disable(vd_void_t);
vd_bool_t fr8000_global_int_restore(vd_void_t);
vd_void_t fr8000_global_switch_register(struct vd_bsp_global_switch *ops);
#endif // !_VDCHIP_40XX_50XX_GLOBAL_SWITCH_H
