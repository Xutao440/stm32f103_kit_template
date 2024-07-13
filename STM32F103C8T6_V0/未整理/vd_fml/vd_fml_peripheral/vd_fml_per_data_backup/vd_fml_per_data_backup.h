/*
* Project: mcu-kit
 * Module: vd_fml_per_data_backup
 * File: vd_fml_per_data_backup.h
 * Created Date: 2022��7��23��14:20:58
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
1.MR88��Ƭ���ڴ��СΪ128K����0x20000
������������Ԥ�����8K��Ϊ���ݴ洢������
��ʼ��ַΪ 0x1E000
��СΪ0x2000
һ������������һ�������ݱ�����
�����Ҫ��OTA����
ǰ8K��ʱ��BootLoader��
APP����112K�������Ҫ��APP���ݣ�APP���ܳ���56K��С
*/
//#define DATA_BACKUP_FLASH_START_ADDR 0x1E000			//��ʼ��ַ
//#define DATA_BACKUP_FLASH_SIZE 0x2000	//�ܴ�С
//#define DATA_BACKUP_FLASH_CHECK_SIZE 0x1000	//���ݶԱȼ������С

//FR8008,����ĿͼƬ��Դ�ļ���0x1500000��ʼ
#define DATA_BACKUP_FLASH_SIZE 0x2000   //�ܴ�С
#define DATA_BACKUP_FLASH_CHECK_SIZE 0x1000 //���ݶԱȼ������С
#define DATA_BACKUP_FLASH_WRITE_ADRESS_A 0x7FE000			//��ʼ��ַ
#define DATA_BACKUP_FLASH_WRITE_ADRESS_B 0x7FF000			//��ʼ��ַ
/* ======================================================================================
 * extern 
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
 //��0x1E000��ʼ��������
// #define VD_FML_PER_DATA_BACKUP_DATA_01  3  //3���ֽڵ�ʱ�䱣������
 

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
 * @brief  ��ʼ��
 * @return vd_bool_t 	
 */
vd_bool_t vd_fml_per_data_backup_init(vd_void_t);

/**
 * @brief  ��ȡ��������
 * @param data_backup_address ��ַ
 * @param buffer ���ݻ���
 * @param length ��ȡ����
 * @return vd_uint16_t ʵ�ʶ�ȡ���� 	
 */
vd_uint16_t vd_fml_per_data_backup_read(vd_uint32_t data_backup_address, vd_uint8_t* buffer, vd_uint32_t length);

/**
 * @brief  д�뱸������
 * @param data_backup_address ��ַ
 * @param data ����
 * @param data_len �������ݳ���
 * @return vd_bool_t ���ݽ��
 */
vd_bool_t vd_fml_per_data_backup_write_data(vd_uint32_t data_backup_address, vd_uint8_t *data, vd_uint16_t data_len);


__vd_extern_c_leave__

#endif // _VD_FML_PER_DATA_BACKUP_H
