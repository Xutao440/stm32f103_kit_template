/*
 * Project: MCU_kit
 * Module: DRV_FLASH
 * File: vd_bsp_flash.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_flashͷ�ļ�
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_FLASH_H
#define _DRV_FLASH_H

/* ======================================================================================
 * includes
 */
#include "base.h"
/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
 


/* ======================================================================================
 * types
 */

//flash��д��Ԫö��
typedef enum{
	FLASH_RW_UNIT_BYTE_1 = 1,
	FLASH_RW_UNIT_BYTE_2 = 2,
	FLASH_RW_UNIT_BYTE_4 = 4,
	FLASH_RW_UNIT_BYTE_8 = 8,
}flash_rw_unit_e;

//flash����
typedef struct{
	vd_uint32_t			base_addr;//flash����ַ
	vd_uint32_t			size;			//flash��С
	vd_uint32_t			page_size;//ҳ����������С
	flash_rw_unit_e rw_unit;//��д��Ԫ
}flash_param_t;

/* ����FLASHƽ̨�ӿ� */
typedef struct{
	vd_bool_t (*get_param)(flash_param_t *param);//��ȡflash����
	vd_bool_t (*lock)(vd_void_t);/* ���� */
	vd_bool_t (*unlock)(vd_void_t);/* ���� */
	vd_bool_t (*read)(vd_uint32_t addr, uint8_t* pbuf, vd_uint32_t len);/* �� */
	vd_bool_t (*write)(vd_uint32_t addr, uint8_t* pbuf, vd_uint32_t len);/* д */
	vd_bool_t (*page_erase)(vd_uint32_t addr);/* ���� */
}flash_ops_t;

/* ƽ̨ע��ӿ� */
typedef vd_bool_t (*flash_plat_register_t)(flash_ops_t *ops);

/* ======================================================================================
 * declaration
 */

/**
 * @brief  ��ʼ��FLASH
 *
 * @return vd_int32_t 	���ض�ȡ�Ľ��
 */
vd_int32_t vd_bsp_flash_init(vd_void_t);

/**
 * @brief  ��ȡFLASH����
 *
 * @param ReadAddr 			��ַ
 * @param pBuffer 			���ݻ���
 * @param NumToRead 			���ݳ���
 * @return vd_bool_t 	���ض�ȡ�Ľ��
 */
vd_int32_t vd_bsp_flash_read(vd_uint32_t ReadAddr, vd_uint8_t *pBuffer, vd_uint32_t NumToRead);

/**
 * @brief  дFLASH����
 *
 * @param WriteAddr 	��ַ
 * @param pBuffer 		���ݻ���
 * @param NumToWrite 	���ݳ���
 * @return vd_bool_t 	������  @ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_flash_write(vd_uint32_t WriteAddr, vd_uint8_t *pBuffer, vd_uint32_t NumToWrite);

/**
 * @brief  FLASH����
 *
 * @param offset 			λ��ƫ����
 * @param size 				��������
 * @return vd_bool_t 	���ز����Ľ��
 */
vd_int32_t vd_bsp_flash_erase(vd_uint32_t offset, vd_uint32_t size);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // _DRV_FLASH_H
