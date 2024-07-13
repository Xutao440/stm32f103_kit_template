#include "vd_bsp_pm/vd_bsp_pm.h"

#include "platform_config.h"
#ifndef PLATFORM_PM_REGISTER
#define PLATFORM_PM_REGISTER vd_null
#endif // !PLATFORM_PM_REGISTER

struct vd_bsp_pm pm_ops;
vd_bsp_pm_register_t  vd_bsp_pm_register = PLATFORM_PM_REGISTER;


/**
 * @brief sleep mode 
 * 
 * @return ** vd_bool_t 
 */
vd_bool_t vd_bsp_pm_suspend(vd_void_t)
{
     if(vd_bsp_pm_register){
        vd_bsp_pm_register(&pm_ops);
    }
    pm_ops.suspend();
    return vd_true;
}


/**
 * @brief wakeup
 * 
 * @return ** vd_bool_t 
 */
vd_bool_t vd_bsp_pm_resume(vd_void_t)
{
    pm_ops.resume();
    return vd_true;
}
