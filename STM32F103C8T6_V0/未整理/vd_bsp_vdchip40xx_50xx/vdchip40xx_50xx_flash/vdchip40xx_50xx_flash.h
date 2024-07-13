/**
 * @file vdchip40xx_50xx_flash.h
 * @author liyongshuang
 * @brief 
 * @version 0.1
 * @date 2023-06-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __VDCHIP40XX_50XX_FLASH_H
#define __VDCHIP40XX_50XX_FLASH_H

#include "vd_bsp_flash/vd_bsp_flash.h"

/* 平台注册接口 */
vd_bool_t vdchip_40xx_50xx_flash_register(flash_ops_t *ops);

#endif
