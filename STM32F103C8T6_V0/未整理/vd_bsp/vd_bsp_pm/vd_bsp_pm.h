#ifndef _VD_BSP_PM_H
#define _VD_BSP_PM_H

#include "base_type.h"

struct vd_bsp_pm
{
    vd_bool_t (*suspend)(vd_void_t);
    vd_bool_t (*resume)(vd_void_t);
};

vd_bool_t vd_bsp_pm_suspend(vd_void_t);
vd_bool_t vd_bsp_pm_resume(vd_void_t);

typedef vd_void_t (*vd_bsp_pm_register_t)(struct vd_bsp_pm *ops);
#endif // !_VD_BSP_PM_H
