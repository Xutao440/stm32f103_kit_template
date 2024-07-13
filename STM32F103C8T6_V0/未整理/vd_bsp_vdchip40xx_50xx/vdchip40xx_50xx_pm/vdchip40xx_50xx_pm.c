#include "vdchip40xx_50xx_pm/vdchip40xx_50xx_pm.h"
#include "driver_system.h"


vd_bool_t fr8000_suspend(vd_void_t)
{
    system_sleep_enable();
}


vd_bool_t fr8000_resume(vd_void_t)
{
    system_sleep_disable();
}


vd_void_t fr8000_pm_register(struct vd_bsp_pm *ops)
{
    ops->suspend = fr8000_suspend;
    ops->resume = fr8000_resume;
}