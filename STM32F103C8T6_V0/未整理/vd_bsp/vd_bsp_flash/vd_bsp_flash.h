/*
 * Project: MCU_kit
 * Module: DRV_FLASH
 * File: vd_bsp_flash.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_flash头文件
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

//flash读写单元枚举
typedef enum{
	FLASH_RW_UNIT_BYTE_1 = 1,
	FLASH_RW_UNIT_BYTE_2 = 2,
	FLASH_RW_UNIT_BYTE_4 = 4,
	FLASH_RW_UNIT_BYTE_8 = 8,
}flash_rw_unit_e;

//flash参数
typedef struct{
	vd_uint32_t			base_addr;//flash基地址
	vd_uint32_t			size;			//flash大小
	vd_uint32_t			page_size;//页（扇区）大小
	flash_rw_unit_e rw_unit;//读写单元
}flash_param_t;

/* 定义FLASH平台接口 */
typedef struct{
	vd_bool_t (*get_param)(flash_param_t *param);//获取flash参数
	vd_bool_t (*lock)(vd_void_t);/* 上锁 */
	vd_bool_t (*unlock)(vd_void_t);/* 解锁 */
	vd_bool_t (*read)(vd_uint32_t addr, uint8_t* pbuf, vd_uint32_t len);/* 读 */
	vd_bool_t (*write)(vd_uint32_t addr, uint8_t* pbuf, vd_uint32_t len);/* 写 */
	vd_bool_t (*page_erase)(vd_uint32_t addr);/* 擦除 */
}flash_ops_t;

/* 平台注册接口 */
typedef vd_bool_t (*flash_plat_register_t)(flash_ops_t *ops);

/* ======================================================================================
 * declaration
 */

/**
 * @brief  初始化FLASH
 *
 * @return vd_int32_t 	返回读取的结果
 */
vd_int32_t vd_bsp_flash_init(vd_void_t);

/**
 * @brief  读取FLASH数据
 *
 * @param ReadAddr 			地址
 * @param pBuffer 			数据缓存
 * @param NumToRead 			数据长度
 * @return vd_bool_t 	返回读取的结果
 */
vd_int32_t vd_bsp_flash_read(vd_uint32_t ReadAddr, vd_uint8_t *pBuffer, vd_uint32_t NumToRead);

/**
 * @brief  写FLASH数据
 *
 * @param WriteAddr 	地址
 * @param pBuffer 		数据缓存
 * @param NumToWrite 	数据长度
 * @return vd_bool_t 	错误码  @ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_flash_write(vd_uint32_t WriteAddr, vd_uint8_t *pBuffer, vd_uint32_t NumToWrite);

/**
 * @brief  FLASH擦除
 *
 * @param offset 			位置偏移量
 * @param size 				擦除长度
 * @return vd_bool_t 	返回擦除的结果
 */
vd_int32_t vd_bsp_flash_erase(vd_uint32_t offset, vd_uint32_t size);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // _DRV_FLASH_H
