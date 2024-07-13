/*
 * Project: MCU_kit
 * Module: DRV_I2C
 * File: vd_bsp_i2c.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_i2c头文件
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_I2C_H
#define _DRV_I2C_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "vd_bsp_type.h"

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

/* i2c工作模式 */
typedef enum{
	PLATFORM_I2C_MODE_MASTER_7BIT,		/* 主机，7位地址 */
	PLATFORM_I2C_MODE_MASTER_10BIT,		/* 主机，10位地址 */
	PLATFORM_I2C_MODE_SLAVE_7BIT,			/* 从机，7位地址 */
	PLATFORM_I2C_MODE_SLAVE_10BIT,		/* 从机，10位地址 */
	PLATFORM_I2C_MODE_MAX,
}i2c_mode_e;

//寄存器地址bit
typedef enum{
	PLATFORM_I2C_REG_BIT_0BIT,		/* 无寄存器地址 */
	PLATFORM_I2C_REG_BIT_8BIT,		/* 8位寄存器?地址 */
	PLATFORM_I2C_REG_BIT_16BIT,		/* 16位寄存器?地址 */
	PLATFORM_I2C_REG_BIT_MAX,
}i2c_reg_bit_e;

//I2C速率
typedef enum{
	PLATFORM_I2C_SPEED_50K,
	PLATFORM_I2C_SPEED_100K,
	PLATFORM_I2C_SPEED_200K,
	PLATFORM_I2C_SPEED_400K,
	PLATFORM_I2C_SPEED_MAX,
}i2c_speed_e;

/* i2c参数结构体 */
typedef struct {
	vd_bsp_port_e  	id;					//i2c索引
	vd_int8_t  			sclk_pin;		//时钟引脚
	vd_int8_t  			sda_pin;		//数据引脚
	vd_uint8_t  		mode;				//工作模式，参考 i2c_mode_e
	vd_uint32_t			slave_addr;	//从机地址
	
	vd_bool_t				is_init;		//是否初始化
} vd_bsp_i2c_t;

//i2c映射表
typedef struct{
	vd_bsp_port_e  	platform_iic_id;			//i2c索引
	vd_uint32_t     device_i2c_addr;    	//从机地址
	i2c_reg_bit_e  	platform_i2c_reg_bit;	//寄存器地址位宽
}i2c_map_t;

/* 定义I2C平台接口 */
typedef struct{
	vd_bool_t  (*init)(vd_bsp_i2c_t *i2c);/* 初始化 */
	vd_int32_t (*master_read)(vd_bsp_i2c_t *i2c, i2c_map_t *i2c_map, vd_uint16_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);/* 读 */
	vd_int32_t (*master_write)(vd_bsp_i2c_t *i2c, i2c_map_t *i2c_map, vd_uint16_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);/* 写 */
	vd_int32_t (*slave_read)(vd_bsp_i2c_t *i2c, vd_uint8_t* data, vd_uint16_t data_len);/* 读 */
	vd_int32_t (*slave_write)(vd_bsp_i2c_t *i2c, vd_uint8_t* data, vd_uint16_t data_len);/* 写 */
}i2c_ops_t;

/* 平台注册接口 */
typedef vd_bool_t (*i2c_plat_register_t)(i2c_ops_t *ops);

/* ======================================================================================
 * declaration
 */

/**
 * @brief 初始化I2C
 *
 * @param i2c_id  		I2C 初始化对应ID号
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_init(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, i2c_reg_bit_e slave_reg_bit);

/**
* @brief  I2C主机读取数据
 *
 * @param i2c_id 			I2C索引
 * @param slave_addr 	从机地址
 * @param data 				读取数据缓存buff
 * @param data_len 		需要读取的数据长度
 * @return 	非负值：	实际读取的数据长度
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_master_read(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief I2C主机写数据
 *
 * @param i2c_id 			I2C索引
 * @param slave_addr 	从机地址
 * @param data: 			数据内容缓存buff
 * @param data_len: 	数据内容长度
 * @return 	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_master_write(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief  I2C从机读取数据
 *
 * @param i2c_id 			I2C索引
 * @param data 				读取数据缓存buff
 * @param data_len 		需要读取的数据长度
 * @return 	非负值：	实际读取的数据长度
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_slave_read(vd_bsp_port_e i2c_id, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief I2C从机写数据
 *
 * @param i2c_id 			I2C索引
 * @param data: 			数据内容缓存buff
 * @param data_len: 	数据内容长度
 * @return 	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_slave_write(vd_bsp_port_e i2c_id, vd_uint8_t* data, vd_uint16_t data_len);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // _DRV_I2C_H
