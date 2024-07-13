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
/*ƽ̨�ӿ�*/
static vd_bool_t vdchip_40xx_50xx_flash_get_param(flash_param_t *param);//��ȡ����
static vd_bool_t vdchip_40xx_50xx_flash_lock(vd_void_t);/* ���� */
static vd_bool_t vdchip_40xx_50xx_flash_unlock(vd_void_t);/* ���� */
static vd_bool_t vdchip_40xx_50xx_flash_read(vd_uint32_t addr, vd_uint8_t* pbuf, vd_uint32_t len);/* �� */
static vd_bool_t vdchip_40xx_50xx_flash_write(vd_uint32_t addr, vd_uint8_t* pbuf, vd_uint32_t len);/* д */
static vd_bool_t vdchip_40xx_50xx_flash_page_erase(vd_uint32_t addr);/* ���� */



/* ======================================================================================
 * implementation
 */

/**
 * @brief flashƽ̨ע��
 *
 * @param ops		flashƽ̨�ӿ�
 * @return vd_void_t			ע����
 */
vd_bool_t vdchip_40xx_50xx_flash_register(flash_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/* �ӿ�ע�� */
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
 * @brief flash��ȡ����
 * @return vd_bool_t			���
 */
static vd_bool_t vdchip_40xx_50xx_flash_get_param(flash_param_t *param)
{
	param->base_addr = FLASH_BASE_ADDR;//flash����ַ
	param->size = 512 * 1024;//flash��С
	param->page_size = FLASH_SECTOR_SIZE;//������С
	param->rw_unit = 1;//��д��С��λһ�ֽ�
	
	return true;
}

/**
 * @brief flash����
 * @return vd_bool_t			���
 */
static vd_bool_t vdchip_40xx_50xx_flash_lock(vd_void_t)
{
	uint16_t status;

#if (LOG_ENABLE_DEBUG)
	log_debug("flash lock\n");
#endif
	status = flash_read_status(false);
	flash_write_status(status, false);           // ����
	
	return vd_true;
}

/**
 * @brief flash����
 * @return vd_bool_t			���
 */
static vd_bool_t vdchip_40xx_50xx_flash_unlock(vd_void_t)
{
	uint16_t status;

#if (LOG_ENABLE_DEBUG)
	log_debug("flash unlock\n");
#endif
	status = flash_read_status(false);
	flash_write_status(status & (0xff), false);  // ����
	
	return vd_true;
}

/**
 * @brief  ��FLASH����
 *
 * @param offset 			λ��ƫ����
 * @param buffer 			���ݻ���
 * @param length 			���ݳ���
 * @return vd_bool_t 	���ض�ȡ�Ľ��
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
 * @brief  дFLASH����
 *
 * @param offset 			λ��ƫ����
 * @param buffer 			���ݻ���
 * @param length 			���ݳ���
 * @return vd_bool_t 	����д��Ľ��
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
 * @brief  FLASH��ҳ����
 * @param addr 			��ַ
 * @return vd_bool_t 	���ز����Ľ��
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


