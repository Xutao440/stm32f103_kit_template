/*
 * Project: mcu-kit
 * Module: vd_fml_per_data_backup
 * File: vd_fml_per_data_backup.c
 * Created Date: 2022年7月23日14:06:41
 * Author: gwx
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)

/* ======================================================================================
 * includes
 */
#include "vd_fml_per_data_backup.h"
#include "vd_bsp_flash/vd_bsp_flash.h"
#include "vd_log.h"
#include "string.h"

/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

//static vd_uint8_t read_data[512];
//static vd_uint8_t data_backup_read_data[512];
//static vd_uint8_t save_data_check_add=0x00;


/**
 * @brief  初始化
 *
 * @return vd_bool_t 	
 */
vd_bool_t vd_fml_per_data_backup_init(vd_void_t)
{
	if(vd_bsp_flash_init() < 0){
		return false;
	}
	
	return true;
}

/**
 * @brief  写入备份数据
 * @param data_backup_address 地址
 * @param data 数据
 * @param data_len 备份数据长度
 * @return vd_bool_t 备份结果
 */
vd_bool_t vd_fml_per_data_backup_write_data(vd_uint32_t data_backup_address, vd_uint8_t *data, vd_uint16_t data_len)
{  
	vd_bool_t ret;
  vd_uint16_t i;
  vd_uint8_t read_result = 0;
	vd_uint8_t *backup_buf = vd_null;
	
	backup_buf = PLATFORM_MALLOC(data_len + 2);//备份数据需要添加头部数据长度与尾部校验码
	vd_check_return_val_error(backup_buf == vd_null, 0, "backup_buf malloc error\r\n");
	
	backup_buf[0] = data_len + 2;//数据长度包括头部与校验码
	memcpy(&backup_buf[1], data, data_len);
	
	backup_buf[data_len + 1] = 0;//校验位清0
	for(i = 0; i < backup_buf[0] - 1; i++){//计算校验和放到尾部
		backup_buf[data_len + 1] += backup_buf[i];
	}
	log_array("backup write data", backup_buf, data_len + 2);

	//备份数据
	vd_bsp_flash_write(data_backup_address, backup_buf, data_len + 2);
	
	//尝试读出数据看是否备份成功
	memset(backup_buf, 0, data_len + 2);
	read_result = vd_fml_per_data_backup_read(data_backup_address, backup_buf, data_len);
	if(read_result == data_len){
		log_debug("vd_fml_per_data_backup_write_data is ok!!!\r\n");
		ret = vd_true;
	}else {
		vd_bsp_flash_erase(data_backup_address, data_len + 2);//擦除这片区域
		log_error("vd_fml_per_data_backup_write_data is fail!!!\r\n");
		ret = vd_false;
	}
	
	PLATFORM_FREE(backup_buf);
	
	return ret;
}



/**
 * @brief  读取备份数据
 * @param data_backup_address 地址
 * @param buffer 数据缓存
 * @param length 读取长度
 * @return vd_uint16_t 实际读取长度 	
 */
vd_uint16_t vd_fml_per_data_backup_read(vd_uint32_t data_backup_address, vd_uint8_t* buffer, vd_uint32_t length)
{    
  vd_uint16_t ret, i = 0;
	vd_uint8_t save_data_check_add=0x00;
	vd_uint8_t *backup_buf = vd_null;
	
	backup_buf = PLATFORM_MALLOC(length + 2);//备份数据包括头部数据长度与尾部校验码
	vd_check_return_val_error(backup_buf == vd_null, 0, "backup_buf malloc error\r\n");
	
	//读出数据
	vd_bsp_flash_read(data_backup_address, backup_buf, length + 2);
	log_array("backup read data", backup_buf, length + 2);
	
	//长度校验
	if(backup_buf[0] != length + 2){//数据长度不匹配
		log_error("data invalid\r\n");
		ret = 0;
	}else{
		//计算校验和
		for(i = 0; i < backup_buf[0] - 1; i++){
			save_data_check_add += backup_buf[i];
		}
		
		if(save_data_check_add == backup_buf[backup_buf[0] - 1]){//校验通过
			memcpy(buffer, &backup_buf[1], length);//拷贝数据
			ret = length;
		}else{
			log_error("data sum check error: data code = %02x, check code = %02x\r\n", backup_buf[backup_buf[0] - 1], save_data_check_add);
			ret = 0;
		}	
	}
	
	PLATFORM_FREE(backup_buf);
	
	return ret;
}



