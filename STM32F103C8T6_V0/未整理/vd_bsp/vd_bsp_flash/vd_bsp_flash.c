/*
 * Project: MCU_kit
 * Module: vd_bsp_flash.c
 * File: vd_bsp_flash.c
 * Created Date: 2022-03-24 15:19:11
 * Author: GanWenXu
 * Description: FLASH����
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */

#define LOG_ENABLE_DEBUG (0)

/* ======================================================================================
 * includes
 */
#include "vd_bsp_flash/vd_bsp_flash.h"
#include "platform_config.h"
#include "vd_log.h"
#include "string.h"

/* ======================================================================================
 * macros
 */
 
#ifndef PLATFORM_FLASH_REGISTER
#define PLATFORM_FLASH_REGISTER vd_null
#endif

/* ======================================================================================
 * types
 */
 //flash����ָ�루���ݲ�ͬ��flash��д��Ԫѡ�ò�ͬ��ָ�룩
typedef union{
	uint8_t *unit_byte_1;
	uint16_t *unit_byte_2;
	uint32_t *unit_byte_4;
	uint64_t *unit_byte_8;
}flash_buf_pointer_u;



/* FLASH �������� */
typedef struct{
	flash_plat_register_t 	plat_register;		/* FLASH ƽ̨ע��ӿ� */
	flash_ops_t 			ops;				/* FLASH ƽ̨�ӿ� */
	flash_param_t						param;					//flash����
	vd_uint8_t 							*rw_buf;				//flash��д����
}flash_drv_t;

/* ======================================================================================
 * declaration
 */

static vd_bool_t vd_bsp_flash_write_no_check(vd_uint32_t WriteAddr, void *pBuffer, vd_uint32_t NumToWrite);

/* ======================================================================================
 * globals
 */
 

/* ���� FLASH �������� */
static flash_drv_t flash = {
	.plat_register = PLATFORM_FLASH_REGISTER,
	.ops = {0},
};

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/* ======================================================================================
 * implementation
 */

/**
 * @brief  flash�޼��д
 * @return vd_int32_t 	����д����
 */
static vd_bool_t vd_bsp_flash_write_no_check(vd_uint32_t WriteAddr, void *pBuffer, vd_uint32_t NumToWrite)
{
	vd_bool_t   ret = vd_false;

	/* д�� */
	vd_check_return_val_error(flash.ops.write == vd_null, -VD_BSP_ERR_SDK, "flash write port is null\r\n");
	ret = flash.ops.write(WriteAddr, pBuffer, NumToWrite);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash write error\r\n");

	return ret;
}

/**
 * @brief  FLASH����һҳ��������
 * @param PageAddr 		ҳ��ַ
 * @return vd_bool_t 	���ز����Ľ��
 */
static vd_bool_t vd_bsp_flash_page_erase(vd_uint32_t PageAddr)
{
    vd_bool_t   ret = vd_false;

		/* ���� */
		vd_check_return_val_error(flash.ops.page_erase == vd_null, -VD_BSP_ERR_SDK, "flash page_erase port is null\r\n");
		ret = flash.ops.page_erase(PageAddr);
		vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash page_erase error\r\n");
	
		return ret;
}

/**
 * @brief  flash����
 * @return vd_int32_t 	���ؽ��
 */
static vd_bool_t vd_bsp_flash_lock(vd_void_t)
{
	vd_bool_t   ret = vd_false;

	/* ���� */
	vd_check_return_val_error(flash.ops.lock == vd_null, -VD_BSP_ERR_SDK, "flash lock port is null\r\n");
	ret = flash.ops.lock();
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash lock error\r\n");

	return ret;
}

/**
 * @brief  flash����
 * @return vd_int32_t 	���ؽ��
 */
static vd_bool_t vd_bsp_flash_unlock(vd_void_t)
{
	vd_bool_t   ret = vd_false;

	/* ���� */
	vd_check_return_val_error(flash.ops.unlock == vd_null, -VD_BSP_ERR_SDK, "flash unlock port is null\r\n");
	ret = flash.ops.unlock();
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash unlock error\r\n");

	return ret;
}

/**
 * @brief  дFLASH����
 *
 * @param WriteAddr 		��ַ
 * @param pBuffer 			��д���ݻ��棨��д�뵥Ԫ��أ�
 * @param NumToWrite 		���ݳ��ȣ�������д�뵥Ԫ��������
 * @return vd_bool_t 		����д��Ľ��
 */
static vd_bool_t vd_bsp_flash_write_with_check(vd_uint32_t WriteAddr, void *pBuffer, vd_uint32_t NumToWrite) 
{
	vd_bool_t ret;
		vd_uint32_t secpos;	   //������ַ
		vd_uint16_t secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
		vd_uint16_t secremain; //������ʣ���ַ(16λ�ּ���)	   
		vd_uint16_t i;    
		vd_uint32_t offaddr;   //ȥ��flash����ַ��ĵ�ַ
	void *p_flash_buf = flash.rw_buf;

#if (LOG_ENABLE_DEBUG)	
	log_debug("================= %s ==================\r\n", __func__);
#endif
	
		vd_bsp_flash_unlock();					//����
	
		offaddr = WriteAddr - flash.param.base_addr;		//ʵ��ƫ�Ƶ�ַ.
		secpos = offaddr / flash.param.page_size;			//������ַ 
		secoff = (offaddr % flash.param.page_size) / flash.param.rw_unit;		//�������ڵ�ƫ��
		secremain = flash.param.page_size / flash.param.rw_unit - secoff;		//����ʣ��ռ��С  
	
#if (LOG_ENABLE_DEBUG)
	log_debug("WriteAddr = 0x%x\r\n", WriteAddr);
	log_debug("NumToWrite = %d\r\n", NumToWrite);
	log_debug("offaddr = 0x%x\r\n", offaddr);
	log_debug("secpos = %d\r\n", secpos);
	log_debug("secoff = %d\r\n", secoff);
	log_debug("secremain = %d\r\n", secremain);
#endif

		if(NumToWrite <= secremain){//�����ڸ�������Χ
			secremain = NumToWrite;
		}
		
		while(1){
			//������������������
			if(vd_bsp_flash_read(secpos * flash.param.page_size + flash.param.base_addr, p_flash_buf, flash.param.page_size / flash.param.rw_unit) < 0){//��������
				goto FLASH_WRITE_ERROR;
			}
//			log_debug("����������������\r\n");
//			log_array(vd_null, flash.rw_buf, flash.param.page_size / flash.param.rw_unit);
			
			//У������
			switch(flash.param.rw_unit){
				case FLASH_RW_UNIT_BYTE_1:{
					for(i = 0; i < secremain; i++){	
						if(((uint8_t *)p_flash_buf)[secoff + i] != 0xff){//��Ҫ����  
							break;	 
						}					
					}
				}break;
				case FLASH_RW_UNIT_BYTE_2:{
					for(i = 0; i < secremain; i++){	
						if(((uint16_t *)p_flash_buf)[secoff + i] != 0xffff){//��Ҫ����  
							break;	 
						}					
					}
				}break;
				case FLASH_RW_UNIT_BYTE_4:{
					for(i = 0; i < secremain; i++){	
						if(((uint32_t *)p_flash_buf)[secoff + i] != 0xffffffff){//��Ҫ����  
							break;	 
						}					
					}
				}break;
				case FLASH_RW_UNIT_BYTE_8:{
					for(i = 0; i < secremain; i++){	
						if(((uint64_t *)p_flash_buf)[secoff + i] != 0xffffffffffffffff){//��Ҫ����  
							break;	 
						}					
					}
				}break;
				default:goto FLASH_WRITE_ERROR;
			}
			
			if(i < secremain){				//��Ҫ����
				//�����������
				if(!vd_bsp_flash_page_erase(secpos * flash.param.page_size + flash.param.base_addr)){//��������
					goto FLASH_WRITE_ERROR;
				}
				
				//��������				
				switch(flash.param.rw_unit){
					case FLASH_RW_UNIT_BYTE_1:{
						for(i = 0; i < secremain; i++){//����
							((uint8_t *)p_flash_buf)[i + secoff] = ((uint8_t *)pBuffer)[i];	  
						}
					}break;
					case FLASH_RW_UNIT_BYTE_2:{
						for(i = 0; i < secremain; i++){//����
							((uint16_t *)p_flash_buf)[i + secoff] = ((uint16_t *)pBuffer)[i];	  
						}
					}break;
					case FLASH_RW_UNIT_BYTE_4:{
						for(i = 0; i < secremain; i++){//����
							((uint32_t *)p_flash_buf)[i + secoff] = ((uint32_t *)pBuffer)[i];	  
						}
					}break;
					case FLASH_RW_UNIT_BYTE_8:{
						for(i = 0; i < secremain; i++){//����
							((uint64_t *)p_flash_buf)[i + secoff] = ((uint64_t *)pBuffer)[i];	  
						}
					}break;
					default:goto FLASH_WRITE_ERROR;
				}
				
				//д������
				ret = vd_bsp_flash_write_no_check(secpos * flash.param.page_size + flash.param.base_addr, p_flash_buf, flash.param.page_size / flash.param.rw_unit);//д����������  
			}else{
				ret = vd_bsp_flash_write_no_check(WriteAddr, pBuffer, secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 
			}
			if(!ret){//flashд��ʧ��
FLASH_WRITE_ERROR:
				vd_bsp_flash_lock();		//����
#if (LOG_ENABLE_DEBUG)
				log_debug("flash write fail\r\n");
				log_debug("================= %s end ==================\r\n\r\n", __func__);
#endif
				return vd_false;
			}
			
			if(NumToWrite == secremain){//д�������
				break;
			}else{//д��δ����
				secpos++;				//������ַ��1
				secoff = 0;				//ƫ��λ��Ϊ0 	 						
				//ָ��ƫ��
				switch(flash.param.rw_unit){
					case FLASH_RW_UNIT_BYTE_1:{
						pBuffer = (uint8_t *)pBuffer + secremain;  	//ָ��ƫ��
					}break;
					case FLASH_RW_UNIT_BYTE_2:{
						pBuffer = (uint16_t *)pBuffer + secremain;  	//ָ��ƫ��
					}break;
					case FLASH_RW_UNIT_BYTE_4:{
						pBuffer = (uint32_t *)pBuffer + secremain;  	//ָ��ƫ��
					}break;
					case FLASH_RW_UNIT_BYTE_8:{
						pBuffer = (uint64_t *)pBuffer + secremain;  	//ָ��ƫ��
					}break;
					default:goto FLASH_WRITE_ERROR;
				}
				WriteAddr += secremain * flash.param.rw_unit;	//д��ַƫ��(���ݲ�ͬд�뵥λƫ����Ӧ����)	   
				NumToWrite -= secremain;	//д�����ݼ�(��д�뵥λ�й�)
				
				if(NumToWrite > (flash.param.page_size / flash.param.rw_unit)){//��һ����������д����
					secremain = flash.param.page_size / flash.param.rw_unit;
				}else{//��һ����������д����
					secremain = NumToWrite;
				}
			}	 
		}	
		vd_bsp_flash_lock();		//����
#if (LOG_ENABLE_DEBUG)
		log_debug("================= %s end ==================\r\n\r\n", __func__);
#endif
		return vd_true;
}


/**
 * @brief  ��ʼ��FLASH
 *
 * @return vd_int32_t 	���ض�ȡ�Ľ��
 */
vd_int32_t vd_bsp_flash_init(vd_void_t)
{
	vd_bool_t   ret = vd_false;
	
	//ƽ̨ע��
	vd_check_return_val_error(flash.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "platform flash not bound\r\n");
	ret = flash.plat_register(&flash.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "platform flash register fail\r\n");
	
	//��ȡflash����
	vd_check_return_val_error(flash.ops.get_param == vd_null, -VD_BSP_ERR_SDK, "flash get_param port is null\r\n");
	ret = flash.ops.get_param(&flash.param);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash get_param error\r\n");
	
	//����flash��д����ռ�
	flash.rw_buf = PLATFORM_MALLOC(flash.param.page_size);
	vd_check_return_val_error(flash.rw_buf == vd_null, -VD_BSP_ERR_MALLOC, "flash rw_buf malloc error\r\n");
	
#if (LOG_ENABLE_DEBUG)
	log_debug("flash init success\r\n");
	log_debug("flash base_addr = 0x%x\r\n", flash.param.base_addr);
	log_debug("flash size = %d\r\n", flash.param.size);
	log_debug("flash page_size = %d\r\n", flash.param.page_size);
	log_debug("flash rw_unit = %d\r\n", flash.param.rw_unit);
#endif

	return VD_BSP_NOERROR;
}

/**
 * @brief  ��ȡFLASH����
 *
 * @param ReadAddr 			��ַ
 * @param pBuffer 			���ݻ���
 * @param NumToRead 			���ݳ���
 * @return vd_bool_t 	���ض�ȡ�Ľ��
 */
vd_int32_t vd_bsp_flash_read(vd_uint32_t ReadAddr, vd_uint8_t *pBuffer, vd_uint32_t NumToRead) 
{
  vd_bool_t   ret = vd_false;	
	vd_uint8_t 	pbuf[10] = {0};

#if (LOG_ENABLE_DEBUG)
	log_debug("================= %s ==================\r\n", __func__);
	log_debug("ReadAddr = 0x%x\r\n", ReadAddr);
	log_debug("NumToRead = %d\r\n", NumToRead);
#endif
	
	vd_check_return_val_error(flash.ops.read == vd_null, vd_false, "flash read port is null\r\n");
	
	//�Ȱ���ȡ��Ԫ��ȡ
	ret = flash.ops.read(ReadAddr, pBuffer, NumToRead - (NumToRead % flash.param.rw_unit));
	vd_check_return_val_error(ret == vd_false, vd_false, "platform flash read error\r\n");
	
	if(NumToRead % flash.param.rw_unit){//�ж����ֽ�δ��ȡ����ȡʣ���ֽ�
		ret = flash.ops.read(ReadAddr + NumToRead -  (NumToRead % flash.param.rw_unit), pbuf, NumToRead % flash.param.rw_unit);
		vd_check_return_val_error(ret == vd_false, vd_false, "platform flash read error\r\n");	
		
		memcpy(&pBuffer[NumToRead - (NumToRead % flash.param.rw_unit)], pbuf, NumToRead % flash.param.rw_unit);//�������û�buf��
	}

#if (LOG_ENABLE_DEBUG)
	log_debug("================= %s end ==================\r\n\r\n", __func__);
#endif
	
	return VD_BSP_NOERROR;
}

/**
 * @brief  дFLASH����
 *
 * @param WriteAddr 	��ַ
 * @param pBuffer 		���ݻ���
 * @param NumToWrite 	���ݳ���
 * @return vd_bool_t 	������  @ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_flash_write(vd_uint32_t WriteAddr, vd_uint8_t *pBuffer, vd_uint32_t NumToWrite)
{
	vd_bool_t ret;
	vd_uint8_t *pbuf = vd_null, lack_num;//ȱ���ֽ���
	
	if(WriteAddr < flash.param.base_addr || (WriteAddr + NumToWrite - 1 > (flash.param.base_addr + flash.param.size - 1))){//�Ƿ���ַ
		return -VD_BSP_ERR_PARAMETER;
	}
	
	lack_num = NumToWrite % flash.param.rw_unit;//�õ���ǰ��д��λ����µ�ȱ���ֽ���
#if (LOG_ENABLE_DEBUG)
	log_debug("lack_num = %d\r\n", lack_num);
#endif
	
	if(lack_num){//��ȱ�٣�д�볤�Ȳ��Ƕ�д��λ������������Ҫ����
		pbuf = PLATFORM_MALLOC(NumToWrite + lack_num);
		vd_check_return_val_error(pbuf == vd_null, -VD_BSP_ERR_MALLOC, "flash write buf malloc error\r\n");
		
		memcpy(pbuf, pBuffer, NumToWrite);//������д������
		
		//������ȱ�ֽ�������䵽��д������֮��
		if(vd_bsp_flash_read(WriteAddr + NumToWrite, &pbuf[NumToWrite], lack_num) < 0){
			return -VD_BSP_ERR_SDK;
		}
		
		ret = vd_bsp_flash_write_with_check(WriteAddr, pbuf, NumToWrite / flash.param.rw_unit + lack_num);//д��flash
		
		PLATFORM_FREE(pbuf);
	}else{
		ret = vd_bsp_flash_write_with_check(WriteAddr, pBuffer, NumToWrite / flash.param.rw_unit);//д��flash
	}
	
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "flash write error\r\n");
	
	return ret;
}	



/**
 * @brief  FLASH����
 *
 * @param addr 				������ַ
 * @param size 				��������
 * @return vd_int32_t ������
 */
vd_int32_t vd_bsp_flash_erase(vd_uint32_t addr, vd_uint32_t size)
{
	vd_int32_t ret;
	vd_uint8_t *pbuf = vd_null;
	
	pbuf = PLATFORM_MALLOC(size);
	vd_check_return_val_error(pbuf == vd_null, -VD_BSP_ERR_MALLOC, "system malloc error\r\n");
	
	memset(pbuf, 0xff, size);
	
	ret = vd_bsp_flash_write(addr, pbuf, size);
	vd_check_return_val_error(ret < 0, ret, "flash erase error\r\n");
	
	PLATFORM_FREE(pbuf);
	
	return VD_BSP_NOERROR;
}

