/*
* Project: mcu-kit
 * Module: vd_fml_per_data_backup
 * File: vd_fml_per_data_backup.h
 * Created Date: 2022年7月23日14:20:58
 * Author: gwx
 * Description: description
 * -----
 * todo: modified 
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _VD_FML_PER_DATA_BACKUP_H
#define _VD_FML_PER_DATA_BACKUP_H

/* ======================================================================================
 * includes
 */
#include "base.h"

/*
1.MR88单片机内存大小为128K，即0x20000
经过分析，将预留最后8K作为数据存储区即：
起始地址为 0x1E000
大小为0x2000
一半做数据区，一半做数据备份区
如果需要带OTA功能
前8K暂时做BootLoader区
APP区有112K，如果需要做APP备份，APP不能超过56K大小
*/
//#define DATA_BACKUP_FLASH_START_ADDR 0x1E000			//起始地址
//#define DATA_BACKUP_FLASH_SIZE 0x2000	//总大小
//#define DATA_BACKUP_FLASH_CHECK_SIZE 0x1000	//备份对比检查区大小

//FR8008,该项目图片资源文件是0x1500000开始
#define DATA_BACKUP_FLASH_SIZE 0x2000   //总大小
#define DATA_BACKUP_FLASH_CHECK_SIZE 0x1000 //备份对比检查区大小
#define DATA_BACKUP_FLASH_WRITE_ADRESS_A 0x7FE000			//起始地址
#define DATA_BACKUP_FLASH_WRITE_ADRESS_B 0x7FF000			//起始地址
/* ======================================================================================
 * extern 
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
 //从0x1E000开始保存数据
// #define VD_FML_PER_DATA_BACKUP_DATA_01  3  //3个字节的时间保存数据
 

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */ 
 
/**
 * @brief  初始化
 * @return vd_bool_t 	
 */
vd_bool_t vd_fml_per_data_backup_init(vd_void_t);

/**
 * @brief  读取备份数据
 * @param data_backup_address 地址
 * @param buffer 数据缓存
 * @param length 读取长度
 * @return vd_uint16_t 实际读取长度 	
 */
vd_uint16_t vd_fml_per_data_backup_read(vd_uint32_t data_backup_address, vd_uint8_t* buffer, vd_uint32_t length);

/**
 * @brief  写入备份数据
 * @param data_backup_address 地址
 * @param data 数据
 * @param data_len 备份数据长度
 * @return vd_bool_t 备份结果
 */
vd_bool_t vd_fml_per_data_backup_write_data(vd_uint32_t data_backup_address, vd_uint8_t *data, vd_uint16_t data_len);


__vd_extern_c_leave__

#endif // _VD_FML_PER_DATA_BACKUP_H
