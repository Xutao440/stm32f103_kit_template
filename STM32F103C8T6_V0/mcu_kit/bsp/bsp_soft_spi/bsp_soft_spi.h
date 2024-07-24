#ifndef _VD_BSP_SOFT_SPI_H_
#define _VD_BSP_SOFT_SPI_H_


#include "base_type.h"
#include "vd_bsp_type.h"
#include "vd_bsp_gpio.h"
#include "vd_bsp_dwt.h"

#define SLAVE_DEV_MAX   6

#define SCK_PORT    GPIO_PB11
#define MOSI_PORT   GPIO_PB10
#define MISO_PORT   GPIO_PB1

#define SPI_Delay_Us(Us)    vd_bsp_dwt_delay_us(Us)
//#define IIC_Err		1

#define  SPI_W_SS(port, value)    vd_bsp_gpio_write(port, value ? VD_GPIO_HIGH : VD_GPIO_LOW)
#define  SPI_W_SCK(value)         vd_bsp_gpio_write(master_spi.sck_port, value ? VD_GPIO_HIGH : VD_GPIO_LOW)
#define  SPI_W_MOSI(value)        vd_bsp_gpio_write(master_spi.mosi_port, value ? VD_GPIO_HIGH : VD_GPIO_LOW)
#define  SPI_R_MISO()             vd_bsp_gpio_read(master_spi.miso_port)

/* 寄存器地址bit */
typedef enum{
	SPI_00_MODE = 0,		//模式0
	SPI_01_MODE,		    
	SPI_10_MODE,
	SPI_11_MODE,
	
}VD_SPI_MODE_E;


/* spi从机数据结构体*/
typedef struct
{
	GPIO_PORT_E      ss_port;   //数据端口号
	VD_SPI_MODE_E    spi_mode;  //spi的模式
	
}vd_slave_spi_t;


/* spi数据结构体*/
typedef struct
{
	GPIO_PORT_E      sck_port;     //时钟端口号
	GPIO_PORT_E      mosi_port;    //主输出，从输入
	GPIO_PORT_E      miso_port;    //主输入，从输出
	GPIO_PORT_E      ss_port;      //数据端口号
	VD_SPI_MODE_E    spi_mode;     //spi的模式
} vd_soft_spi_t;


vd_uint8_t vd_bsp_spi_send_byte(uint8_t byte);
vd_int32_t vd_bsp_soft_spi_init(vd_soft_spi_t*  master);
vd_int32_t vd_bsp_soft_spi_add_slave_dev(vd_slave_spi_t*  slave_dev);


void SPI_Start(vd_int32_t slave_dev_id);
void SPI_Stop(vd_int32_t slave_dev_id);

#endif

