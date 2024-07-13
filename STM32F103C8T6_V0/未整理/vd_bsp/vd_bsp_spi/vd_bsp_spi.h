/*
 * Project: MCU_kit
 * Module: DRV_SPI
 * File: vd_bsp_spi.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_spi头文件
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_SPI_H
#define _DRV_SPI_H

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
 
 
//#define vd_bsp_spim_write_cmd8(spim_id, cmd)          \
//    {                                              \
//        vd_uint8_t write_cmd[1];                   \
//        write_cmd[0] = cmd;                        \
//        vd_bsp_spim_write_cmd(spim_id, write_cmd, 1); \
//    }

//#define vd_bsp_spim_write_cmd16(spim_id, cmd)         \
//    {                                              \
//        vd_uint8_t write_cmd[2];                   \
//        write_cmd[0] = (cmd >> 8) & 0xff;          \
//        write_cmd[1] = cmd & 0xff;                 \
//        vd_bsp_spim_write_cmd(spim_id, write_cmd, 2); \
//    }

//#define vd_bsp_spim_write_data8(spim_id, data)          \
//    {                                                \
//        vd_uint8_t write_data[1];                    \
//        write_data[0] = data;                        \
//        vd_bsp_spim_write_data(spim_id, write_data, 1); \
//    }

//#define vd_bsp_spim_write_data16(spim_id, data)         \
//    {                                                \
//        vd_uint8_t write_data[2];                    \
//        write_data[0] = (data >> 8) & 0xff;          \
//        write_data[1] = data & 0xff;                 \
//        vd_bsp_spim_write_data(spim_id, write_data, 2); \
//    }

/* ======================================================================================
 * types
 */

/* spi工作模式 */
typedef enum  
{
    PALTFORM_SPI_WORK_MODE_0,    /* Idle: Low ; sample: first  edge */
    PALTFORM_SPI_WORK_MODE_1,    /* Idle: Low ; sample: second edge */
    PALTFORM_SPI_WORK_MODE_2,    /* Idle: High; sample: first  edge */
    PALTFORM_SPI_WORK_MODE_3,    /* Idle: High; sample: second edge */
		PALTFORM_SPI_WORK_MODE_MAX,
}plat_spi_mode_e;

/* spi片选模式 */
typedef enum  
{
    PALTFORM_SPI_CS_MODE_SOFTWARE,    /* 软件片选 */
    PALTFORM_SPI_CS_MODE_HARDWARE,    /* 硬件片选 */
		PALTFORM_SPI_CS_MODE_MAX,
}plat_spi_cs_mode_e;

/* spim参数结构体 */
typedef struct {
    vd_bsp_port_e  	id;					//SPI索引 

		vd_int8_t  	sclk_pin;		//时钟引脚
    vd_int8_t  	cs_pin;   	// SPI片选脚
    vd_int8_t  	mosi_pin;		//mosi引脚
		vd_int8_t  	miso_pin;		//miso引脚

		vd_uint8_t	mode;				//工作模式 值 plat_spi_mode_e
    vd_uint8_t  cs_mode;		//片选模式		值 plat_spi_cs_mode_e
    vd_uint8_t  data_bit;		//数据位
    vd_uint32_t speed;			

		vd_bool_t 	use_dma;				//是否使用dma
		vd_uint8_t	dma_channel;		//dma通道选择

} vd_bsp_spim_t;

/* spis参数结构体 */
typedef struct {
    vd_bsp_port_e 	id;
	
		vd_int8_t  	sclk_pin;		//时钟引脚
    vd_int8_t 	cs_pin;			//片选引脚
    vd_int8_t  	mosi_pin;		//mosi引脚，qspi io0
		vd_int8_t  	miso_pin;		//miso引脚，qspi io1
	
    vd_uint8_t 	data_bit;
    vd_uint32_t speed;			
		vd_uint8_t	mode;						//工作模式
	
		vd_bool_t 	use_dma;
		vd_uint8_t	dma_channel;		//dma通道选择
} vd_bsp_spis_t;


/* 定义SPIM相关接口 */
typedef struct{
	vd_bool_t (*init)(vd_bsp_spim_t *spim);/* 初始化 */
	vd_bool_t (*open)(vd_bsp_spim_t *spim);/* 开 */
	vd_bool_t (*close)(vd_bsp_spim_t *spim);/* 关 */
	vd_bool_t (*cs)(vd_bsp_spim_t *qspi, vd_uint8_t pin_level);/* 软件片选控制 */
	vd_int32_t (*read)(vd_bsp_spim_t *spim, vd_uint8_t* data, vd_uint32_t data_len);/* 读 */
	vd_int32_t (*write)(vd_bsp_spim_t *spim, vd_uint8_t* data, vd_uint32_t data_len);/* 写 */
	vd_int32_t (*transmit)(vd_bsp_spim_t *spim, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len);/* 读写数据 */
	vd_int32_t (*write_dma)(vd_bsp_spim_t *spim, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, void(*cb)(void *), void* cb_param);/* 通过DMA写 */
}spim_port_t;

/* 定义SPIS相关接口 */
typedef struct{
	vd_bool_t (*init)(vd_bsp_spis_t *spis);/* 初始化 */
	vd_bool_t (*open)(vd_bsp_spis_t *spis);/* 开 */
	vd_bool_t (*close)(vd_bsp_spis_t *spis);/* 关 */
	vd_int32_t (*read)(vd_bsp_spis_t *spis, vd_uint8_t* data, vd_uint32_t data_len);/* 读 */
	vd_int32_t (*write)(vd_bsp_spis_t *spis, vd_uint8_t* data, vd_uint32_t data_len);/* 写 */
	vd_int32_t (*transmit)(vd_bsp_spis_t *spis, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len);/* 读写数据 */
	vd_int32_t (*write_dma)(vd_bsp_spis_t *spis, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, void(*cb)(void *), void* cb_param);/* 通过DMA写 */
}spis_port_t;

/* 定义SPI平台接口 */
typedef struct{
	spim_port_t spim_port;/* spim接口 */
	spis_port_t spis_port;/* spis接口 */
}spi_ops_t;

/* 平台注册接口 */
typedef vd_bool_t (*spi_plat_register_t)(spi_ops_t *ops);


/* ======================================================================================
 * declaration
 */

/**
 * @brief 初始化SPIM
 *
 * @param spim_id  SPIM 初始化对应ID号
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_init(vd_bsp_port_e spim_id);

/**
 * @brief 打开SPIM功能
 *
 * @param spim_id SPIM索引
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_open(vd_bsp_port_e spim_id);

/**
 * @brief 关闭SPIM功能
 *
 * @param spim_id SPIM索引
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_close(vd_bsp_port_e spim_id);

/**
 * @brief SPIM软件片选控制
 *
 * @param spim_id 			SPI索引
 * @param pin_level 	引脚电平
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_cs_control(vd_bsp_port_e spim_id, vd_uint8_t pin_level);

/**
 * @brief  读取SPIM数据
 *
 * @param spim_id SPIM索引
 * @param data 读取数据缓存buff
 * @param data_len 需要读取的数据长度
 * @return	非负值：	实际读取的数据长度
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_read(vd_bsp_port_e spim_id, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief 写SPIM数据
 *
 * @param spim_id : SPIM索引
 * @param data : SPIM数据内容缓存buff
 * @param data_len : SPIM数据内容长度
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_write(vd_bsp_port_e spim_id, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief spim同步收发数据传输通道
 *
 * @param spim_id SPIM索引
 * @param tx_data SPIM发送数据的buff
 * @param tx_data_len SPIM发送数据的数据长度
 * @param rx_data SPIM接收数据的数据缓存buff
 * @param rx_data_len SPIM接收数据的长度
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_transmit(
    vd_bsp_port_e  	spim_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    vd_uint8_t* rx_data,
    vd_uint32_t rx_data_len);

/**
 * @brief SPIMaster通过DMA方式写数据
 *
 * @param spim_id SPIM索引号
 * @param tx_data SPIM发送数据的buff
 * @param tx_data_len SPIM发送数据的数据长度
 * @param cb 回调函数指针
 * @param cb_param 回调函数
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_write_dma(
    vd_bsp_port_e  	spim_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    void (*cb)(void*),
    void* cb_param);
/**
 * @brief 初始化SPIS
 *
 * @param spis_id : SPIS索引
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_init(vd_bsp_port_e spis_id);
/**
 * @brief 打开SPIS功能
 *
 * @param spis_id : SPIS索引
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_open(vd_bsp_port_e spis_id);
/**
 * @brief 关闭SPIS功能
 *
 * @param spis_id : SPIS索引
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_close(vd_bsp_port_e spis_id);
/**
 * @brief  读取SPIS数据
 *
 * @param spis_id : SPIS索引
 * @param data 读取数据缓存buff
 * @param data_len 需要读取的数据长度
 * @return	非负值：	实际读取的数据长度
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_read(vd_bsp_port_e spis_id, vd_uint8_t* data, vd_uint32_t data_len);
/**
 * @brief 写SPIS数据
 *
 * @param spis_id : SPIS索引
 * @param data : SPIS数据内容缓存buff
 * @param data_len : SPIS数据内容长度
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_write(vd_bsp_port_e spis_id, vd_uint8_t* data, vd_uint32_t data_len);
/**
 * @brief spis同步收发数据传输通道
 *
 * @param spis_id SPIS索引
 * @param tx_data SPIS发送数据的buff
 * @param tx_data_len SPIS发送数据的数据长度
 * @param rx_data SPIS接收数据的数据缓存buff
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_spis_transmit(
    vd_bsp_port_e  	spis_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    vd_uint8_t* rx_data,
    vd_uint32_t rx_data_len);

/**
 * @brief SPISlave通过DMA方式写数据
 *
 * @param spis_id SPIS索引号
 * @param tx_data SPIS发送数据的buff
 * @param tx_data_len SPIS发送数据的数据长度
 * @param cb 回调函数指针
 * @param cb_param 回调函数
 * @return	非负值：	成功
						负值：		错误码 		参考 vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_write_dma(
    vd_bsp_port_e  	spis_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    void (*cb)(void*),
    void* cb_param);
/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // _DRV_SPI_H
