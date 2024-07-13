#ifndef __VDCHIP40XX_50XX_PM_H
#define __VDCHIP40XX_50XX_PM_H

#include "vd_bsp_pm/vd_bsp_pm.h"

vd_bool_t fr8000_suspend(vd_void_t);
vd_bool_t fr8000_resume(vd_void_t);
vd_void_t fr8000_pm_register(struct vd_bsp_pm *ops);

#endif


