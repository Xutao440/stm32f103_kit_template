/**
 * @file vdchip40xx_50xx_i2c.c
 * @author liyongshuang
 * @brief 
 * @version 0.1
 * @date 2023-06-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* ======================================================================================
 * includes
 */
#include "vdchip40xx_50xx_i2c/vdchip40xx_50xx_i2c.h"
#include "driver_i2c.h"
#include "driver_system.h"
#include "driver_gpio.h"
#include "vd_log.h"

/* ======================================================================================
 * types
 */

/* 定义i2c驱动对象 */
typedef struct{
	I2C_HandleTypeDef 								handle;		/* i2c句柄 */
}vdchip_40xx_50xx_i2c_t;

vdchip_40xx_50xx_i2c_t i2c0 = {0};/* i2c0驱动对象 */
vdchip_40xx_50xx_i2c_t i2c1 = {0};/* i2c1驱动对象 */

/* ======================================================================================
 * declaration
 */

/*平台接口*/
static vd_bool_t vdchip_40xx_50xx_i2c_init(vd_bsp_i2c_t *i2c);/* 初始化 */
static vd_int32_t vdchip_40xx_50xx_i2c_master_read(vd_bsp_i2c_t *i2c, i2c_map_t *i2c_map, vd_uint16_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);/* 主机读 */
static vd_int32_t vdchip_40xx_50xx_i2c_master_write(vd_bsp_i2c_t *i2c, i2c_map_t *i2c_map, vd_uint16_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len);/* 主机写 */
static vd_int32_t vdchip_40xx_50xx_i2c_slave_read(vd_bsp_i2c_t *i2c, vd_uint8_t* data, vd_uint16_t data_len);/* 从机读 */
static vd_int32_t vdchip_40xx_50xx_i2c_slave_write(vd_bsp_i2c_t *i2c, vd_uint8_t* data, vd_uint16_t data_len);/* 从机写 */


/* ======================================================================================
 * helper
 */

/**
 * @brief i2c平台注册
 *
 * @param ops		i2c平台接口
 * @return vd_void_t			注册结果
 */
vd_bool_t vdchip_40xx_50xx_i2c_register(i2c_ops_t *ops)
{
	vd_check_return_val_error(ops == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/* 接口注册 */
	ops->init = vdchip_40xx_50xx_i2c_init;
	ops->master_read = vdchip_40xx_50xx_i2c_master_read;
	ops->master_write = vdchip_40xx_50xx_i2c_master_write;
	ops->slave_read = vdchip_40xx_50xx_i2c_slave_read;
	ops->slave_write = vdchip_40xx_50xx_i2c_slave_write;
	
	return vd_true;
}

/* ======================================================================================
 * private implementation
 */

/**
 * @brief i2c初始化
 *
 * @param i2c  					i2c参数
 * @return vd_bool_t		初始化结果
 */
static vd_bool_t vdchip_40xx_50xx_i2c_init(vd_bsp_i2c_t *i2c)
{
	GPIO_InitTypeDef   GPIO_Handle = {0};
	vdchip_40xx_50xx_i2c_t *i2cx = vd_null;
	
	/* 检查i2c iD */
	if(i2c->id == VD_PORT_I2C0){
		i2cx = &i2c0;
		i2cx->handle.I2Cx = I2C0;
		__SYSTEM_I2C0_MASTER_CLK_SELECT_96M();
		__SYSTEM_I2C0_CLK_ENABLE();
	}else if(i2c->id == VD_PORT_I2C1){
		i2cx = &i2c1;
		i2cx->handle.I2Cx = I2C1;
		__SYSTEM_I2C1_MASTER_CLK_SELECT_96M();
		__SYSTEM_I2C1_CLK_ENABLE();
	}
	vd_check_return_val_error(i2cx == vd_null, vd_false, "in func:%s, line:%d", __func__, __LINE__);
	
	/*  I2C引脚初始化  */
	__SYSTEM_GPIO_CLK_ENABLE();
	
	/* I2C IO Init */
	GPIO_Handle.Pin       = 0x01 << (i2c->sclk_pin % 8);
	GPIO_Handle.Mode      = GPIO_MODE_AF_PP;
	GPIO_Handle.Pull      = GPIO_PULLUP;
	GPIO_Handle.Alternate = GPIO_FUNCTION_1;
	gpio_init(i2c->sclk_pin / 8, &GPIO_Handle);
	
	GPIO_Handle.Pin       = 0x01 << (i2c->sda_pin % 8);
	GPIO_Handle.Mode      = GPIO_MODE_AF_PP;
	GPIO_Handle.Pull      = GPIO_PULLUP;
	GPIO_Handle.Alternate = GPIO_FUNCTION_1;
	gpio_init(i2c->sda_pin / 8, &GPIO_Handle);
	
//	system_set_port_mux(i2c->sclk_pin / 8, i2c->sclk_pin % 8, 0x01);//PORTA4_FUNC_I2C0_CLK);	
//	system_set_port_mux(i2c->sda_pin / 8, i2c->sda_pin % 8, 0x01);//PORTA5_FUNC_I2C0_DAT
	
	/*  I2C外设初始化  */
	/* 配置初始化参数 */
	switch(i2c->mode){
		case PLATFORM_I2C_MODE_MASTER_7BIT:{
			i2cx->handle.Init.I2C_Mode = I2C_MODE_MASTER_7BIT;
		}break;
		case PLATFORM_I2C_MODE_MASTER_10BIT:{
			i2cx->handle.Init.I2C_Mode = I2C_MODE_MASTER_10BIT;
		}break;
		case PLATFORM_I2C_MODE_SLAVE_7BIT:{
			i2cx->handle.Init.I2C_Mode = I2C_MODE_SLAVE_7BIT;
		}break;
		case PLATFORM_I2C_MODE_SLAVE_10BIT:{
			i2cx->handle.Init.I2C_Mode = I2C_MODE_SLAVE_10BIT;
		}break;
		default:return vd_false;
	}
	
	i2cx->handle.Init.Slave_Address = i2c->slave_addr;
	i2cx->handle.Init.SCL_HCNT = 10;
	i2cx->handle.Init.SCL_LCNT = 10;
	
	/* i2c初始化 */
	i2c_init(&i2cx->handle);

//	NVIC_EnableIRQ(I2C1_IRQn);/* i2c中断使能 */
	
  return vd_true;
}

/**
 * @brief i2c主机读
 *
 * @param i2c  					i2c参数	
 * @return vd_bool_t		读取结果
 */
static vd_int32_t vdchip_40xx_50xx_i2c_master_read(vd_bsp_i2c_t *i2c, i2c_map_t *i2c_map, vd_uint16_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len)
{
	vd_bool_t ret = 0;
	vdchip_40xx_50xx_i2c_t *i2cx = vd_null;
	
	/* 检查i2c iD */
	if(i2c->id == VD_PORT_I2C0){
		i2cx = &i2c0;
	}else if(i2c->id == VD_PORT_I2C1){
		i2cx = &i2c1;
	}
	vd_check_return_val_error(i2cx == vd_null, -1, "in func:%s, line:%d", __func__, __LINE__);
	vd_check_return_val_error(i2c_map == vd_null, -1, "in func:%s, line:%d", __func__, __LINE__);
	
	/* i2c读 */
	if(i2c_map->platform_i2c_reg_bit == PLATFORM_I2C_REG_BIT_0BIT){
		ret = i2c_master_receive(&i2cx->handle, slave_addr, data, data_len);
	}else{
		ret = i2c_memory_read(&i2cx->handle, slave_addr, (i2c_map->platform_i2c_reg_bit == PLATFORM_I2C_REG_BIT_8BIT) ? (reg & 0xff) : (reg & 0xffff), data, data_len);
	}
	vd_check_return_val_error(ret == vd_false, -1, "in func:%s, line:%d", __func__, __LINE__);
	
  return data_len;
}

/**
 * @brief i2c主机写
 *
 * @param i2c  					i2c参数
 * @return vd_bool_t		写结果
 */
static vd_int32_t vdchip_40xx_50xx_i2c_master_write(vd_bsp_i2c_t *i2c, i2c_map_t *i2c_map, vd_uint16_t slave_addr, vd_uint32_t reg, vd_uint8_t* data, vd_uint16_t data_len)
{
	vd_bool_t ret = vd_false;
	vdchip_40xx_50xx_i2c_t *i2cx = vd_null;
	
	/* 检查i2c iD */
	if(i2c->id == VD_PORT_I2C0){
		i2cx = &i2c0;
	}else if(i2c->id == VD_PORT_I2C1){
		i2cx = &i2c1;
	}
	vd_check_return_val_error(i2cx == vd_null, -1, "in func:%s, line:%d", __func__, __LINE__);
	vd_check_return_val_error(i2c_map == vd_null, -1, "in func:%s, line:%d", __func__, __LINE__);
	
	/* i2c写 */
	log_debug("i2c master write, slave_addr = 0x%x, reg = 0x%x, len = %d\r\n", slave_addr, reg, data_len);
	
	if(i2c_map->platform_i2c_reg_bit == PLATFORM_I2C_REG_BIT_0BIT){
		ret = i2c_master_transmit(&i2cx->handle, slave_addr, data, data_len);
	}else{
		ret = i2c_memory_write(&i2cx->handle, slave_addr, (i2c_map->platform_i2c_reg_bit == PLATFORM_I2C_REG_BIT_8BIT) ? (reg & 0xff) : (reg & 0xffff), data, data_len);
	}
	vd_check_return_val_error(ret == vd_false, -1, "in func:%s, line:%d", __func__, __LINE__);
	
  return data_len;
}

/**
 * @brief i2c从机读
 *
 * @param i2c  					i2c参数	
 * @return vd_bool_t		读取结果
 */
static vd_int32_t vdchip_40xx_50xx_i2c_slave_read(vd_bsp_i2c_t *i2c, vd_uint8_t* data, vd_uint16_t data_len)
{
	vd_bool_t ret = vd_false;
	vdchip_40xx_50xx_i2c_t *i2cx = vd_null;
	
	/* 检查i2c iD */
	if(i2c->id == VD_PORT_I2C0){
		i2cx = &i2c0;
	}else if(i2c->id == VD_PORT_I2C1){
		i2cx = &i2c1;
	}
	vd_check_return_val_error(i2cx == vd_null, -1, "in func:%s, line:%d", __func__, __LINE__);
	
	/* i2c读 */
	ret = i2c_slave_receive(&i2cx->handle, data, data_len);
	vd_check_return_val_error(ret == vd_false, -1, "in func:%s, line:%d", __func__, __LINE__);
	
  return data_len;
}

/**
 * @brief i2c从机写
 *
 * @param i2c  					i2c参数
 * @return vd_bool_t		写结果
 */
static vd_int32_t vdchip_40xx_50xx_i2c_slave_write(vd_bsp_i2c_t *i2c, vd_uint8_t* data, vd_uint16_t data_len)
{
	vd_bool_t ret = vd_false;
	vdchip_40xx_50xx_i2c_t *i2cx = vd_null;
	
	/* 检查i2c iD */
	if(i2c->id == VD_PORT_I2C0){
		i2cx = &i2c0;
	}else if(i2c->id == VD_PORT_I2C1){
		i2cx = &i2c1;
	}
	vd_check_return_val_error(i2cx == vd_null, -1, "in func:%s, line:%d", __func__, __LINE__);
	
	/* i2c写 */
	ret = i2c_slave_transmit(&i2cx->handle, data, data_len);
	vd_check_return_val_error(ret == vd_false, -1, "in func:%s, line:%d", __func__, __LINE__);
	
  return data_len;
}

/*  中断服务函数  */
/**
 * @brief i2c0中断服务函数
 *
 * @param void  					
 * @return void		
 */
void i2c_isr0(void)
{
    i2c_IRQHandler(&i2c0.handle);
}

/**
 * @brief i2c1中断服务函数
 *
 * @param void  					
 * @return void		
 */
void i2c_isr1(void)
{
    i2c_IRQHandler(&i2c1.handle);
}


