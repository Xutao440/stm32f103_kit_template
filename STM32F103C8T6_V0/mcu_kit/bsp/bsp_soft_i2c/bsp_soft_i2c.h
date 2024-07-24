#ifndef _VD_BSP_SOFT_I2C_H_
#define _VD_BSP_SOFT_I2C_H_


#include "base_type.h"
#include "vd_bsp_type.h"
#include "vd_bsp_gpio.h"
#include "vd_bsp_dwt.h"

#define  SLAVE_I2C_OBJ_MAX   5

#define IIC_OK		0
#define IIC_Err		1

#define IIC_Delay()    vd_bsp_dwt_delay_us(master_i2c.speed)

#define SDA_H	       vd_bsp_gpio_write(master_i2c.sda_prot, VD_GPIO_HIGH)
#define SDA_L	       vd_bsp_gpio_write(master_i2c.sda_prot, VD_GPIO_LOW)
#define SDA_R	       vd_bsp_gpio_read(master_i2c.sda_prot)

#define SCL_H	       vd_bsp_gpio_write(master_i2c.scl_prot, VD_GPIO_HIGH)
#define SCL_L	       vd_bsp_gpio_write(master_i2c.scl_prot, VD_GPIO_LOW)

/* �Ĵ�����ַbit */
typedef enum{
	I2C_REG_BIT_0BIT = 0,		/* �޼Ĵ�����ַ */
	I2C_REG_BIT_8BIT,		    /* 8λ�Ĵ�����ַ */
	I2C_REG_BIT_MAX,
}VD_I2C_REG_BIT_E;

/* I2C���� */
typedef enum{
	I2C_SPEED_50K = 10,     //20umһ������
	I2C_SPEED_100K = 5,     //10umһ������
	I2C_SPEED_200K = 3,     //5umһ������
	I2C_SPEED_400K = 2,     //2.5umһ������
	
	I2C_SPEED_MAX,
	
}VD_I2C_SPEED_E;

/* i2c���ݽṹ��*/
typedef struct
{
	GPIO_PORT_E      sda_prot;    //���ݶ˿ں�
	GPIO_PORT_E      scl_prot;    //ʱ�Ӷ˿ں�
	VD_I2C_SPEED_E   speed;       //�����ٶ�
	VD_I2C_REG_BIT_E reg_bit;     //�Ĵ���
	
} vd_master_i2c_t;

vd_int32_t vd_bsp_soft_i2c_init(vd_master_i2c_t* soft_i2c);
vd_bool_t vd_bsp_soft_i2c_write_byte(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t byte);
vd_bool_t vd_bsp_soft_i2c_read_byte(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t* byte);
vd_bool_t vd_bsp_soft_i2c_write_bytes(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len);
vd_bool_t vd_bsp_soft_i2c_read_bytes(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len);

#endif

