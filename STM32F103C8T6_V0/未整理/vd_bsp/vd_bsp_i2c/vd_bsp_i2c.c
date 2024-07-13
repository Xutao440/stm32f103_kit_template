/*
 * Project: MCU_kit
 * Module: vd_bsp_i2c.c
 * File: vd_bsp_i2c.c
 * Created Date: 2022-03-24 15:19:11
 * Author: GanWenXu
 * Description: I2C����
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
#include "vd_bsp_i2c/vd_bsp_i2c.h"
#include "platform_config.h"
#include "vd_log.h"

/* ======================================================================================
 * macros
 */
#ifndef PLATFORM_I2C_REGISTER
#define PLATFORM_I2C_REGISTER vd_null
#endif

#define IIC_MAP_MAX   5

/* ======================================================================================
 * types
 */

/* I2C �������� */
typedef struct{
	i2c_plat_register_t 	plat_register;		/* I2C ƽ̨ע��ӿ� */
	i2c_ops_t 						ops;				/* I2C ƽ̨�ӿ� */
	vd_uint8_t						map_len;
}i2c_drv_t;

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
 

/* ���� I2C �������� */
static i2c_drv_t i2c_drv = {
	.plat_register = PLATFORM_I2C_REGISTER,
	.ops = {0},
};

/* I2C �����б� */
static vd_bsp_i2c_t g_vd_bsp_i2c[] = {
	#if (PLATFORM_I2C1_EN == ON)
		{VD_PORT_I2C1, PLATFORM_I2C1_SCL, PLATFORM_I2C1_SDA, PLATFORM_I2C1_MODE, PLATFORM_I2C1_SLAVEADDR},
	#else
		0
	#endif
};

/*i2c map*/
static i2c_map_t g_i2c_map[IIC_MAP_MAX]={0};
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */



/**
 * @brief ���ݴӻ���ַ����ӳ���
 *
 * @param i2c_addr 		I2C��Ӧ��addr�ӻ���ַ
 * @return g_i2c_map*
 */
static i2c_map_t* vd_bsp_i2c_addr_find(vd_uint32_t i2c_addr)
{
    vd_int_t i = 0;
    for ( i = 0; i < i2c_drv.map_len; i++ )
    {
        if (g_i2c_map[i].device_i2c_addr == i2c_addr)
        {
            return &g_i2c_map[i];
        }
    }
    return vd_null;
}

/**
 * @brief ����ID������������
 *
 * @param i2c_id 		I2C��Ӧ��ID��
 * @return vd_bsp_i2c_t*
 */
static vd_bsp_i2c_t* vd_bsp_i2c_find(vd_bsp_port_e i2c_id) 
{
	for (int i = 0; i < vd_array_size(g_vd_bsp_i2c); i++) {
			if (g_vd_bsp_i2c[i].id == i2c_id) {
					return &g_vd_bsp_i2c[i];
			}
	}
	return vd_null;
}

/* ======================================================================================
 * implementation
 */

/**
 * @brief ���ñ��
 *
 * @param i2c_id 		iic�˿�   slave_addr  �ӻ���ַ  slave_reg_bit �ӻ���ַregbit
 * @return vd_bsp_i2c_t*
 */
static vd_int32_t vd_bsp_i2c_set_map(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, i2c_reg_bit_e slave_reg_bit)
{
	vd_uint8_t i = 0;
	
	for(i = 0; i < i2c_drv.map_len; i++){
		if(g_i2c_map[i].platform_iic_id == i2c_id && g_i2c_map[i].device_i2c_addr == slave_addr){//ע���
			g_i2c_map[i].platform_iic_id = i2c_id;
			g_i2c_map[i].device_i2c_addr = slave_addr;
			g_i2c_map[i].platform_i2c_reg_bit = slave_reg_bit;     
			return VD_BSP_NOERROR;
		}
	}
	
	vd_check_return_val_error(i2c_drv.map_len >= IIC_MAP_MAX, -VD_BSP_ERR_OVER_SIZE, "i2c init map is full");
	
	g_i2c_map[i2c_drv.map_len].platform_iic_id = i2c_id;
	g_i2c_map[i2c_drv.map_len].device_i2c_addr = slave_addr;
	g_i2c_map[i2c_drv.map_len].platform_i2c_reg_bit = slave_reg_bit;     
	i2c_drv.map_len++;  
	
	return VD_BSP_NOERROR;
}

/**
 * @brief ��ʼ��I2C
 *
 * @param i2c_id  		I2C ��ʼ����ӦID��
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_init(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, i2c_reg_bit_e slave_reg_bit) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_i2c_t* i2c = vd_null;

	/* ƽ̨ע�� */
	vd_check_return_val_error(i2c_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "platform i2c not bound");
	ret = i2c_drv.plat_register(&i2c_drv.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "platform i2c register fail");

	/* ����У�� */
	vd_check_return_val_error(i2c_id < VD_PORT_I2C0 || i2c_id >= VD_PORT_I2C_MAX, -VD_BSP_ERR_PORT_INVALID, "i2c_id = %d, error", i2c_id);
	
	/* ���������б� */
	i2c = vd_bsp_i2c_find(i2c_id);
	vd_check_return_val_error(i2c == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "i2c[%d] not found", i2c_id - VD_PORT_I2C0);

	/* ����У�� */
	vd_check_return_val_error(i2c->mode >= PLATFORM_I2C_MODE_MAX, -VD_BSP_ERR_PARAMETER, "in func:%s, line:%d", __func__, __LINE__);
	
	/*����iic map��*/
	vd_bsp_i2c_set_map(i2c_id, slave_addr, slave_reg_bit);
	
	/* ��ʼ�� */
	if(i2c->is_init == vd_false){
		vd_check_return_val_error(i2c_drv.ops.init == vd_null,  -VD_BSP_ERR_SDK, "i2c init port is null");
		ret = i2c_drv.ops.init(i2c);
		vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform i2c[%d] init error", i2c_id - VD_PORT_I2C0);
		i2c->is_init = vd_true;
	}

  return VD_BSP_NOERROR;
}

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
vd_int32_t vd_bsp_i2c_master_read(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len)
{
	vd_int32_t   ret = 0;
	vd_bsp_i2c_t* i2c = vd_null;
	i2c_map_t* i2c_map = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(i2c_id < VD_PORT_I2C0 || i2c_id >= VD_PORT_I2C_MAX, -VD_BSP_ERR_PORT_INVALID, "i2c_id = %d, error", i2c_id);
	
	/* ���������б� */
	i2c = vd_bsp_i2c_find(i2c_id);
	vd_check_return_val_error(i2c == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No i2c[%d]", i2c_id - VD_PORT_I2C0);

	i2c_map = vd_bsp_i2c_addr_find(slave_addr);  
	vd_check_return_val_error(i2c_map == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No i2c[%d]", i2c_id - VD_PORT_I2C0);
	
	/* ��ȡ */
	vd_check_return_val_error(i2c_drv.ops.master_read == vd_null,  -VD_BSP_ERR_SDK, "i2c master_read port is null");
	ret = i2c_drv.ops.master_read(i2c, i2c_map, slave_addr, reg, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform i2c[%d] master_read error", i2c_id - VD_PORT_I2C0);

	return ret;
}

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
vd_int32_t vd_bsp_i2c_master_write(vd_bsp_port_e i2c_id, vd_uint32_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len)
{
	vd_int32_t   ret = 0;
	vd_bsp_i2c_t* i2c = vd_null;
	i2c_map_t* i2c_map = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(i2c_id < VD_PORT_I2C0 || i2c_id >= VD_PORT_I2C_MAX, -VD_BSP_ERR_PORT_INVALID, "i2c_id = %d, error", i2c_id);
	
	/* ���������б� */
	i2c = vd_bsp_i2c_find(i2c_id);
	vd_check_return_val_error(i2c == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No i2c[%d]", i2c_id - VD_PORT_I2C0);

	i2c_map = vd_bsp_i2c_addr_find(slave_addr);  
	vd_check_return_val_error(i2c_map == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No i2c[%d]", i2c_id - VD_PORT_I2C0);
	
	/* д���� */
	vd_check_return_val_error(i2c_drv.ops.master_write == vd_null,  -VD_BSP_ERR_SDK, "i2c master_write port is null");
	ret = i2c_drv.ops.master_write(i2c, i2c_map, slave_addr, reg, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform i2c[%d] master_write error", i2c_id - VD_PORT_I2C0);

	return ret;
}

/**
 * @brief  I2C�ӻ���ȡ����
 *
 * @param i2c_id 			I2C����
 * @param data 				��ȡ���ݻ���buff
 * @param data_len 		��Ҫ��ȡ�����ݳ���
 * @return 	�Ǹ�ֵ��	ʵ�ʶ�ȡ�����ݳ���
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_slave_read(vd_bsp_port_e i2c_id, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_i2c_t* i2c = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(i2c_id < VD_PORT_I2C0 || i2c_id >= VD_PORT_I2C_MAX, -VD_BSP_ERR_PORT_INVALID, "i2c_id = %d, error", i2c_id);

	/* ���������б� */
	i2c = vd_bsp_i2c_find(i2c_id);
	vd_check_return_val_error(i2c == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No i2c[%d]", i2c_id - VD_PORT_I2C0);

	/* ��ȡ */
	vd_check_return_val_error(i2c_drv.ops.slave_read == vd_null,  -VD_BSP_ERR_SDK, "i2c slave_read port is null");
	ret = i2c_drv.ops.slave_read(i2c, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform i2c[%d] slave_read error", i2c_id - VD_PORT_I2C0);

	return ret;
}

/**
 * @brief I2C�ӻ�д����
 *
 * @param i2c_id 			I2C����
 * @param data: 			�������ݻ���buff
 * @param data_len: 	�������ݳ���
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_i2c_slave_write(vd_bsp_port_e i2c_id, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_i2c_t* i2c = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(i2c_id < VD_PORT_I2C0 || i2c_id >= VD_PORT_I2C_MAX, -VD_BSP_ERR_PORT_INVALID, "i2c_id = %d, error", i2c_id);
	
	/* ���������б� */
	i2c = vd_bsp_i2c_find(i2c_id);
	vd_check_return_val_error(i2c == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No i2c[%d]", i2c_id - VD_PORT_I2C0);

	/* д���� */
	vd_check_return_val_error(i2c_drv.ops.slave_write == vd_null,  -VD_BSP_ERR_SDK, "i2c slave_write port is null");
	ret = i2c_drv.ops.slave_write(i2c, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform i2c[%d] slave_write error", i2c_id - VD_PORT_I2C0);

	return ret;
}

