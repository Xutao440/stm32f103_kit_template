/*
 * Project: MCU_kit
 * Module: DRV_I2C
 * File: vd_bsp_i2c.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_i2cͷ�ļ�
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

/* i2c����ģʽ */
typedef enum{
	PLATFORM_I2C_MODE_MASTER_7BIT,		/* ������7λ��ַ */
	PLATFORM_I2C_MODE_MASTER_10BIT,		/* ������10λ��ַ */
	PLATFORM_I2C_MODE_SLAVE_7BIT,			/* �ӻ���7λ��ַ */
	PLATFORM_I2C_MODE_SLAVE_10BIT,		/* �ӻ���10λ��ַ */
	PLATFORM_I2C_MODE_MAX,
}i2c_mode_e;

//�Ĵ�����ַbit
typedef enum{
	PLATFORM_I2C_REG_BIT_0BIT,		/* �޼Ĵ�����ַ */
	PLATFORM_I2C_REG_BIT_8BIT,		/* 8λ�Ĵ���?��ַ */
	PLATFORM_I2C_REG_BIT_16BIT,		/* 16λ�Ĵ���?��ַ */
	PLATFORM_I2C_REG_BIT_MAX,
}i2c_reg_bit_e;

//I2C����
typedef enum{
	PLATFORM_I2C_SPEED_50K,
	PLATFORM_I2C_SPEED_100K,
	PLATFORM_I2C_SPEED_200K,
	PLATFORM_I2C_SPEED_400K,
	PLATFORM_I2C_SPEED_MAX,
}i2c_speed_e;

/* i2c�����ṹ�� */
typedef struct {
	vd_bsp_port_e  	id;					//i2c����
	vd_int8_t  			sclk_pin;		//ʱ������
	vd_int8_t  			sda_pin;		//��������
	vd_uint8_t  		mode;				//����ģʽ���ο� i2c_mode_e
	vd_uint32_t			slave_addr;	//�ӻ���ַ
	
	vd_bool_t				is_init;		//�Ƿ��ʼ��
} vd_bsp_i2c_t;

//i2cӳ���
typedef struct{
	vd_bsp_port_e  	platform_iic_id;			//i2c����
	vd_uint32_t     device_i2c_addr;    	//�ӻ���ַ
	i2c_reg_bit_e  	platform_i2c_reg_bit;	//�Ĵ�����ַλ��
}i2c_map_t;

/* ����I2Cƽ̨�ӿ� */
typedef struct{
	vd_bool_t  (*init)(vd_bsp_i2c_t *i2c);/* ��ʼ�� */
	vd_int32_t (*master_read)(vd_bsp_i2c_t *i2c, i2c_map_t *i2c_map, vd_uint16_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);/* �� */
	vd_int32_t (*master_write)(vd_bsp_i2c_t *i2c, i2c_map_t *i2c_map, vd_uint16_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);/* д */
	vd_int32_t (*slave_read)(vd_bsp_i2c_t *i2c, vd_uint8_t* data, vd_uint16_t data_len);/* �� */
	vd_int32_t (*slave_write)(vd_bsp_i2c_t *i2c, vd_uint8_t* data, vd_uint16_t data_len);/* д */
}i2c_ops_t;

/* ƽ̨ע��ӿ� */
typedef vd_bool_t (*i2c_plat_register_t)(i2c_ops_t *ops);

/* ======================================================================================
 * declaration
 */

/**
 * @brief ��ʼ��I2C
 *
 * @param i2c_id  		I2C ��ʼ����ӦID��
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_init(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, i2c_reg_bit_e slave_reg_bit);

/**
* @brief  I2C������ȡ����
 *
 * @param i2c_id 			I2C����
 * @param slave_addr 	�ӻ���ַ
 * @param data 				��ȡ���ݻ���buff
 * @param data_len 		��Ҫ��ȡ�����ݳ���
 * @return 	�Ǹ�ֵ��	ʵ�ʶ�ȡ�����ݳ���
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_master_read(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief I2C����д����
 *
 * @param i2c_id 			I2C����
 * @param slave_addr 	�ӻ���ַ
 * @param data: 			�������ݻ���buff
 * @param data_len: 	�������ݳ���
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_master_write(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief  I2C�ӻ���ȡ����
 *
 * @param i2c_id 			I2C����
 * @param data 				��ȡ���ݻ���buff
 * @param data_len 		��Ҫ��ȡ�����ݳ���
 * @return 	�Ǹ�ֵ��	ʵ�ʶ�ȡ�����ݳ���
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_slave_read(vd_bsp_port_e i2c_id, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief I2C�ӻ�д����
 *
 * @param i2c_id 			I2C����
 * @param data: 			�������ݻ���buff
 * @param data_len: 	�������ݳ���
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_slave_write(vd_bsp_port_e i2c_id, vd_uint8_t* data, vd_uint16_t data_len);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // _DRV_I2C_H
