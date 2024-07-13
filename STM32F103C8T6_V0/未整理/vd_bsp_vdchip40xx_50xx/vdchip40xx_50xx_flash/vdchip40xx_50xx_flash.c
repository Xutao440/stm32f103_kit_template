/**
 * @file vdchip40xx_50xx_flash.c
 * @author liyongshaung
 * @brief 
 * @version 0.1
 * @date 2023-06-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
/* ======================================================================================
 * includes
 */
#include "vdchip40xx_50xx_flash/vdchip40xx_50xx_flash.h"
#include "driver_flash.h"
#include "vd_log.h"

/* ======================================================================================
 * macros
 */
#define LOG_ENABLE_DEBUG	(0)
#define FLASH_BASE_ADDR	0x10000000
#define FLASH_SECTOR_SIZE	4096

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */
/*平台接口*/
static vd_bool_t vdchip_40xx_50xx_flash_get_param(flash_param_t *param);//获取参数
static vd_bool_t vdchip_40xx_50xx_flash_lock(vd_void_t);/* 上锁 */
static vd_bool_t vdchip_40xx_50xx_flash_unlock(vd_void_t);/* 解锁 */
static vd_bool_t vdchip_40xx_50xx_flash_read(vd_uint32_t addr, vd_uint8_t* pbuf, vd_uint32_t len);/* 读 */
static vd_bool_t vdchip_40xx_50xx_flash_write(vd_uint32_t addr, vd_uint8_t* pbuf, vd_uint32_t len);/* 写 */
static vd_bool_t vdchip_40xx_50xx_flash_page_erase(vd_uint32_t addr);/* 擦除 */



/* ======================================================================================
 * implementation
 */

/**
 * @brief flash平台注册
 *
 * @param ops		flash平台接口
 * @return vd_void_t			注册结果
 */
vd_bool_t vdchip_40xx_50xx_flash_register(flash_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/* 接口注册 */
	ops->get_param = vdchip_40xx_50xx_flash_get_param;
	ops->lock = vdchip_40xx_50xx_flash_lock;
	ops->unlock = vdchip_40xx_50xx_flash_unlock;
	ops->read = vdchip_40xx_50xx_flash_read;
	ops->write = vdchip_40xx_50xx_flash_write;
	ops->page_erase = vdchip_40xx_50xx_flash_page_erase;
	
	return vd_true;
}


/* ======================================================================================
 * private implementation
 */
/**
 * @brief flash获取参数
 * @return vd_bool_t			结果
 */
static vd_bool_t vdchip_40xx_50xx_flash_get_param(flash_param_t *param)
{
	param->base_addr = FLASH_BASE_ADDR;//flash基地址
	param->size = 512 * 1024;//flash大小
	param->page_size = FLASH_SECTOR_SIZE;//扇区大小
	param->rw_unit = 1;//读写最小单位一字节
	
	return true;
}

/**
 * @brief flash上锁
 * @return vd_bool_t			结果
 */
static vd_bool_t vdchip_40xx_50xx_flash_lock(vd_void_t)
{
	uint16_t status;

#if (LOG_ENABLE_DEBUG)
	log_debug("flash lock\n");
#endif
	status = flash_read_status(false);
	flash_write_status(status, false);           // 关锁
	
	return vd_true;
}

/**
 * @brief flash解锁
 * @return vd_bool_t			结果
 */
static vd_bool_t vdchip_40xx_50xx_flash_unlock(vd_void_t)
{
	uint16_t status;

#if (LOG_ENABLE_DEBUG)
	log_debug("flash unlock\n");
#endif
	status = flash_read_status(false);
	flash_write_status(status & (0xff), false);  // 开锁
	
	return vd_true;
}

/**
 * @brief  读FLASH数据
 *
 * @param offset 			位置偏移量
 * @param buffer 			数据缓存
 * @param length 			数据长度
 * @return vd_bool_t 	返回读取的结果
 */
static vd_bool_t vdchip_40xx_50xx_flash_read(vd_uint32_t addr, vd_uint8_t* pbuf, vd_uint32_t len)
{
#if (LOG_ENABLE_DEBUG)
	log_debug("================= %s ==================\r\n", __func__);
#endif
	
	vd_check_return_val(len == 0, vd_true);

#if (LOG_ENABLE_DEBUG)
	log_debug("read addr = %x, len = %d\r\n", addr, len);
#endif
	flash_read(addr - FLASH_BASE_ADDR, len, pbuf);
	
#if (LOG_ENABLE_DEBUG)
	log_debug("================= %s end ==================\r\n\r\n", __func__);
#endif
	
  return vd_true;
}
/**
 * @brief  写FLASH数据
 *
 * @param offset 			位置偏移量
 * @param buffer 			数据缓存
 * @param length 			数据长度
 * @return vd_bool_t 	返回写入的结果
 */
static vd_bool_t vdchip_40xx_50xx_flash_write(vd_uint32_t addr, vd_uint8_t* pbuf, vd_uint32_t len)
{
#if (LOG_ENABLE_DEBUG)
	log_debug("================= %s ==================\r\n", __func__);
#endif
	
	vd_check_return_val(len == 0, vd_true);

	GLOBAL_INT_DISABLE();
	
#if (LOG_ENABLE_DEBUG)
	log_debug("write addr = %x, len = %d\r\n", addr, len);
#endif
	
	flash_write(addr - FLASH_BASE_ADDR, len, pbuf);

	GLOBAL_INT_RESTORE();
	
#if (LOG_ENABLE_DEBUG)
	log_debug("================= %s end ==================\r\n\r\n", __func__);
#endif
	
  return vd_true;
}

/**
 * @brief  FLASH按页擦除
 * @param addr 			地址
 * @return vd_bool_t 	返回擦除的结果
 */
static vd_bool_t vdchip_40xx_50xx_flash_page_erase(vd_uint32_t addr)
{
#if (LOG_ENABLE_DEBUG)
	log_debug("================= %s ==================\r\n", __func__);
#endif
	
	GLOBAL_INT_DISABLE();
	
#if (LOG_ENABLE_DEBUG)
	log_debug("erase addr = %x\r\n", addr);
#endif
	
	flash_erase(addr - FLASH_BASE_ADDR, FLASH_SECTOR_SIZE);
	
	GLOBAL_INT_RESTORE();
	
#if (LOG_ENABLE_DEBUG)
	log_debug("================= %s end ==================\r\n\r\n", __func__);
#endif
	
	return vd_true;
}


