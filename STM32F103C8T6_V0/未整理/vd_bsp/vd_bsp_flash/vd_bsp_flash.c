/*
 * Project: MCU_kit
 * Module: vd_bsp_flash.c
 * File: vd_bsp_flash.c
 * Created Date: 2022-03-24 15:19:11
 * Author: GanWenXu
 * Description: FLASH驱动
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
 //flash缓存指针（根据不同的flash读写单元选用不同的指针）
typedef union{
	uint8_t *unit_byte_1;
	uint16_t *unit_byte_2;
	uint32_t *unit_byte_4;
	uint64_t *unit_byte_8;
}flash_buf_pointer_u;



/* FLASH 驱动对象 */
typedef struct{
	flash_plat_register_t 	plat_register;		/* FLASH 平台注册接口 */
	flash_ops_t 			ops;				/* FLASH 平台接口 */
	flash_param_t						param;					//flash参数
	vd_uint8_t 							*rw_buf;				//flash读写缓存
}flash_drv_t;

/* ======================================================================================
 * declaration
 */

static vd_bool_t vd_bsp_flash_write_no_check(vd_uint32_t WriteAddr, void *pBuffer, vd_uint32_t NumToWrite);

/* ======================================================================================
 * globals
 */
 

/* 定义 FLASH 驱动对象 */
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
 * @brief  flash无检查写
 * @return vd_int32_t 	返回写入结果
 */
static vd_bool_t vd_bsp_flash_write_no_check(vd_uint32_t WriteAddr, void *pBuffer, vd_uint32_t NumToWrite)
{
	vd_bool_t   ret = vd_false;

	/* 写入 */
	vd_check_return_val_error(flash.ops.write == vd_null, -VD_BSP_ERR_SDK, "flash write port is null\r\n");
	ret = flash.ops.write(WriteAddr, pBuffer, NumToWrite);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash write error\r\n");

	return ret;
}

/**
 * @brief  FLASH擦除一页（扇区）
 * @param PageAddr 		页地址
 * @return vd_bool_t 	返回擦除的结果
 */
static vd_bool_t vd_bsp_flash_page_erase(vd_uint32_t PageAddr)
{
    vd_bool_t   ret = vd_false;

		/* 擦除 */
		vd_check_return_val_error(flash.ops.page_erase == vd_null, -VD_BSP_ERR_SDK, "flash page_erase port is null\r\n");
		ret = flash.ops.page_erase(PageAddr);
		vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash page_erase error\r\n");
	
		return ret;
}

/**
 * @brief  flash上锁
 * @return vd_int32_t 	返回结果
 */
static vd_bool_t vd_bsp_flash_lock(vd_void_t)
{
	vd_bool_t   ret = vd_false;

	/* 上锁 */
	vd_check_return_val_error(flash.ops.lock == vd_null, -VD_BSP_ERR_SDK, "flash lock port is null\r\n");
	ret = flash.ops.lock();
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash lock error\r\n");

	return ret;
}

/**
 * @brief  flash解锁
 * @return vd_int32_t 	返回结果
 */
static vd_bool_t vd_bsp_flash_unlock(vd_void_t)
{
	vd_bool_t   ret = vd_false;

	/* 上锁 */
	vd_check_return_val_error(flash.ops.unlock == vd_null, -VD_BSP_ERR_SDK, "flash unlock port is null\r\n");
	ret = flash.ops.unlock();
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash unlock error\r\n");

	return ret;
}

/**
 * @brief  写FLASH数据
 *
 * @param WriteAddr 		地址
 * @param pBuffer 			待写数据缓存（与写入单元相关）
 * @param NumToWrite 		数据长度（必须是写入单元整数倍）
 * @return vd_bool_t 		返回写入的结果
 */
static vd_bool_t vd_bsp_flash_write_with_check(vd_uint32_t WriteAddr, void *pBuffer, vd_uint32_t NumToWrite) 
{
	vd_bool_t ret;
		vd_uint32_t secpos;	   //扇区地址
		vd_uint16_t secoff;	   //扇区内偏移地址(16位字计算)
		vd_uint16_t secremain; //扇区内剩余地址(16位字计算)	   
		vd_uint16_t i;    
		vd_uint32_t offaddr;   //去掉flash基地址后的地址
	void *p_flash_buf = flash.rw_buf;

#if (LOG_ENABLE_DEBUG)	
	log_debug("================= %s ==================\r\n", __func__);
#endif
	
		vd_bsp_flash_unlock();					//解锁
	
		offaddr = WriteAddr - flash.param.base_addr;		//实际偏移地址.
		secpos = offaddr / flash.param.page_size;			//扇区地址 
		secoff = (offaddr % flash.param.page_size) / flash.param.rw_unit;		//在扇区内的偏移
		secremain = flash.param.page_size / flash.param.rw_unit - secoff;		//扇区剩余空间大小  
	
#if (LOG_ENABLE_DEBUG)
	log_debug("WriteAddr = 0x%x\r\n", WriteAddr);
	log_debug("NumToWrite = %d\r\n", NumToWrite);
	log_debug("offaddr = 0x%x\r\n", offaddr);
	log_debug("secpos = %d\r\n", secpos);
	log_debug("secoff = %d\r\n", secoff);
	log_debug("secremain = %d\r\n", secremain);
#endif

		if(NumToWrite <= secremain){//不大于该扇区范围
			secremain = NumToWrite;
		}
		
		while(1){
			//读出整个扇区的内容
			if(vd_bsp_flash_read(secpos * flash.param.page_size + flash.param.base_addr, p_flash_buf, flash.param.page_size / flash.param.rw_unit) < 0){//读出错误
				goto FLASH_WRITE_ERROR;
			}
//			log_debug("读出整个扇区内容\r\n");
//			log_array(vd_null, flash.rw_buf, flash.param.page_size / flash.param.rw_unit);
			
			//校验数据
			switch(flash.param.rw_unit){
				case FLASH_RW_UNIT_BYTE_1:{
					for(i = 0; i < secremain; i++){	
						if(((uint8_t *)p_flash_buf)[secoff + i] != 0xff){//需要擦除  
							break;	 
						}					
					}
				}break;
				case FLASH_RW_UNIT_BYTE_2:{
					for(i = 0; i < secremain; i++){	
						if(((uint16_t *)p_flash_buf)[secoff + i] != 0xffff){//需要擦除  
							break;	 
						}					
					}
				}break;
				case FLASH_RW_UNIT_BYTE_4:{
					for(i = 0; i < secremain; i++){	
						if(((uint32_t *)p_flash_buf)[secoff + i] != 0xffffffff){//需要擦除  
							break;	 
						}					
					}
				}break;
				case FLASH_RW_UNIT_BYTE_8:{
					for(i = 0; i < secremain; i++){	
						if(((uint64_t *)p_flash_buf)[secoff + i] != 0xffffffffffffffff){//需要擦除  
							break;	 
						}					
					}
				}break;
				default:goto FLASH_WRITE_ERROR;
			}
			
			if(i < secremain){				//需要擦除
				//擦除这个扇区
				if(!vd_bsp_flash_page_erase(secpos * flash.param.page_size + flash.param.base_addr)){//擦除错误
					goto FLASH_WRITE_ERROR;
				}
				
				//拷贝数据				
				switch(flash.param.rw_unit){
					case FLASH_RW_UNIT_BYTE_1:{
						for(i = 0; i < secremain; i++){//复制
							((uint8_t *)p_flash_buf)[i + secoff] = ((uint8_t *)pBuffer)[i];	  
						}
					}break;
					case FLASH_RW_UNIT_BYTE_2:{
						for(i = 0; i < secremain; i++){//复制
							((uint16_t *)p_flash_buf)[i + secoff] = ((uint16_t *)pBuffer)[i];	  
						}
					}break;
					case FLASH_RW_UNIT_BYTE_4:{
						for(i = 0; i < secremain; i++){//复制
							((uint32_t *)p_flash_buf)[i + secoff] = ((uint32_t *)pBuffer)[i];	  
						}
					}break;
					case FLASH_RW_UNIT_BYTE_8:{
						for(i = 0; i < secremain; i++){//复制
							((uint64_t *)p_flash_buf)[i + secoff] = ((uint64_t *)pBuffer)[i];	  
						}
					}break;
					default:goto FLASH_WRITE_ERROR;
				}
				
				//写入数据
				ret = vd_bsp_flash_write_no_check(secpos * flash.param.page_size + flash.param.base_addr, p_flash_buf, flash.param.page_size / flash.param.rw_unit);//写入整个扇区  
			}else{
				ret = vd_bsp_flash_write_no_check(WriteAddr, pBuffer, secremain);//写已经擦除了的,直接写入扇区剩余区间. 
			}
			if(!ret){//flash写入失败
FLASH_WRITE_ERROR:
				vd_bsp_flash_lock();		//上锁
#if (LOG_ENABLE_DEBUG)
				log_debug("flash write fail\r\n");
				log_debug("================= %s end ==================\r\n\r\n", __func__);
#endif
				return vd_false;
			}
			
			if(NumToWrite == secremain){//写入结束了
				break;
			}else{//写入未结束
				secpos++;				//扇区地址增1
				secoff = 0;				//偏移位置为0 	 						
				//指针偏移
				switch(flash.param.rw_unit){
					case FLASH_RW_UNIT_BYTE_1:{
						pBuffer = (uint8_t *)pBuffer + secremain;  	//指针偏移
					}break;
					case FLASH_RW_UNIT_BYTE_2:{
						pBuffer = (uint16_t *)pBuffer + secremain;  	//指针偏移
					}break;
					case FLASH_RW_UNIT_BYTE_4:{
						pBuffer = (uint32_t *)pBuffer + secremain;  	//指针偏移
					}break;
					case FLASH_RW_UNIT_BYTE_8:{
						pBuffer = (uint64_t *)pBuffer + secremain;  	//指针偏移
					}break;
					default:goto FLASH_WRITE_ERROR;
				}
				WriteAddr += secremain * flash.param.rw_unit;	//写地址偏移(根据不同写入单位偏移相应数量)	   
				NumToWrite -= secremain;	//写入数递减(与写入单位有关)
				
				if(NumToWrite > (flash.param.page_size / flash.param.rw_unit)){//下一个扇区还是写不完
					secremain = flash.param.page_size / flash.param.rw_unit;
				}else{//下一个扇区可以写完了
					secremain = NumToWrite;
				}
			}	 
		}	
		vd_bsp_flash_lock();		//上锁
#if (LOG_ENABLE_DEBUG)
		log_debug("================= %s end ==================\r\n\r\n", __func__);
#endif
		return vd_true;
}


/**
 * @brief  初始化FLASH
 *
 * @return vd_int32_t 	返回读取的结果
 */
vd_int32_t vd_bsp_flash_init(vd_void_t)
{
	vd_bool_t   ret = vd_false;
	
	//平台注册
	vd_check_return_val_error(flash.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "platform flash not bound\r\n");
	ret = flash.plat_register(&flash.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "platform flash register fail\r\n");
	
	//获取flash参数
	vd_check_return_val_error(flash.ops.get_param == vd_null, -VD_BSP_ERR_SDK, "flash get_param port is null\r\n");
	ret = flash.ops.get_param(&flash.param);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform flash get_param error\r\n");
	
	//申请flash读写缓存空间
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
 * @brief  读取FLASH数据
 *
 * @param ReadAddr 			地址
 * @param pBuffer 			数据缓存
 * @param NumToRead 			数据长度
 * @return vd_bool_t 	返回读取的结果
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
	
	//先按读取单元读取
	ret = flash.ops.read(ReadAddr, pBuffer, NumToRead - (NumToRead % flash.param.rw_unit));
	vd_check_return_val_error(ret == vd_false, vd_false, "platform flash read error\r\n");
	
	if(NumToRead % flash.param.rw_unit){//有多余字节未读取，读取剩余字节
		ret = flash.ops.read(ReadAddr + NumToRead -  (NumToRead % flash.param.rw_unit), pbuf, NumToRead % flash.param.rw_unit);
		vd_check_return_val_error(ret == vd_false, vd_false, "platform flash read error\r\n");	
		
		memcpy(&pBuffer[NumToRead - (NumToRead % flash.param.rw_unit)], pbuf, NumToRead % flash.param.rw_unit);//拷贝到用户buf中
	}

#if (LOG_ENABLE_DEBUG)
	log_debug("================= %s end ==================\r\n\r\n", __func__);
#endif
	
	return VD_BSP_NOERROR;
}

/**
 * @brief  写FLASH数据
 *
 * @param WriteAddr 	地址
 * @param pBuffer 		数据缓存
 * @param NumToWrite 	数据长度
 * @return vd_bool_t 	错误码  @ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_flash_write(vd_uint32_t WriteAddr, vd_uint8_t *pBuffer, vd_uint32_t NumToWrite)
{
	vd_bool_t ret;
	vd_uint8_t *pbuf = vd_null, lack_num;//缺少字节数
	
	if(WriteAddr < flash.param.base_addr || (WriteAddr + NumToWrite - 1 > (flash.param.base_addr + flash.param.size - 1))){//非法地址
		return -VD_BSP_ERR_PARAMETER;
	}
	
	lack_num = NumToWrite % flash.param.rw_unit;//得到当前读写单位情况下的缺少字节数
#if (LOG_ENABLE_DEBUG)
	log_debug("lack_num = %d\r\n", lack_num);
#endif
	
	if(lack_num){//有缺少，写入长度不是读写单位的整数倍，需要补齐
		pbuf = PLATFORM_MALLOC(NumToWrite + lack_num);
		vd_check_return_val_error(pbuf == vd_null, -VD_BSP_ERR_MALLOC, "flash write buf malloc error\r\n");
		
		memcpy(pbuf, pBuffer, NumToWrite);//拷贝待写入数据
		
		//读出空缺字节数据填充到待写入数据之后
		if(vd_bsp_flash_read(WriteAddr + NumToWrite, &pbuf[NumToWrite], lack_num) < 0){
			return -VD_BSP_ERR_SDK;
		}
		
		ret = vd_bsp_flash_write_with_check(WriteAddr, pbuf, NumToWrite / flash.param.rw_unit + lack_num);//写入flash
		
		PLATFORM_FREE(pbuf);
	}else{
		ret = vd_bsp_flash_write_with_check(WriteAddr, pBuffer, NumToWrite / flash.param.rw_unit);//写入flash
	}
	
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "flash write error\r\n");
	
	return ret;
}	



/**
 * @brief  FLASH擦除
 *
 * @param addr 				擦除地址
 * @param size 				擦除长度
 * @return vd_int32_t 错误码
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

