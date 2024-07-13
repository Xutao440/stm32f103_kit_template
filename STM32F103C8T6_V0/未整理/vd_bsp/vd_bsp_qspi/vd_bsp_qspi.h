/*
 * Project: MCU_kit
 * Module: DRV_QSPI
 * File: vd_bsp_spi.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_spi头文件
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef _DRV_QSPI_H
#define _DRV_QSPI_H

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

/* qspi工作模式 */
typedef enum  
{
    PALTFORM_QSPI_WORK_MODE_0 = 0,    /* Idle: Low ; sample: first  edge */
    PALTFORM_QSPI_WORK_MODE_1,    /* Idle: Low ; sample: second edge */
    PALTFORM_QSPI_WORK_MODE_2,    /* Idle: High; sample: first  edge */
    PALTFORM_QSPI_WORK_MODE_3,    /* Idle: High; sample: second edge */
		PALTFORM_QSPI_WORK_MODE_MAX,
}plat_qspi_mode_e;

/* qspi片选模式 */
typedef enum  
{
    PALTFORM_QSPI_CS_MODE_SOFTWARE = 0,    /* 软件片选 */
    PALTFORM_QSPI_CS_MODE_HARDWARE,    /* 硬件片选 */
		PALTFORM_QSPI_CS_MODE_MAX,
}plat_qspi_cs_mode_e;


typedef enum{
	QSPI_LINE_X1 = 0,
	QSPI_LINE_X2,
	QSPI_LINE_X4,
	QSPI_LINE_MAX,
}qspi_line_e;

/* qspim参数结构体 */
typedef struct {	
    vd_bsp_port_e  id;						//QSPI索引 

		vd_int8_t  sclk_pin;			//时钟引脚
    vd_int8_t  cs_pin;   			// QSPI片选脚
    vd_int8_t  io0_pin;				// io0
		vd_int8_t  io1_pin;				// io1
		vd_int8_t  io2_pin;
		vd_int8_t  io3_pin;

		vd_uint8_t	mode;						//工作模式 值 plat_qspi_mode_e
    vd_uint8_t  cs_mode;			//片选模式		值 plat_qspi_cs_mode_e
    vd_uint8_t  data_bit;			//数据位
    vd_uint32_t speed;			

		vd_bool_t 	use_dma;				//是否使用dma
		vd_uint8_t	dma_channel;		//dma通道选择
	
		vd_uint8_t	*dma_buf;				//dma发送缓存
		vd_void_t 	(*tx_cplt_cb)(vd_void_t *arg);//DMA发送完成回调
		vd_void_t 	*cb_arg;//回调参数
} vd_bsp_qspi_t;

/* 定义qspi收发配置类型 */
typedef struct{
	vd_uint32_t instruct;			/* 指令 */
	vd_uint32_t address;			/* 地址 */
	vd_uint8_t instruct_len;	/* 指令长度 */
	vd_uint8_t address_len;		/* 地址长度 */
	qspi_line_e line;					/* 线类型 */
}vd_qspi_tx_cfg_t;

/* 定义QSPI平台接口 */
typedef struct{
	vd_bool_t (*init)(vd_bsp_qspi_t *qspi);/* 初始化 */
	vd_bool_t (*open)(vd_bsp_qspi_t *qspi);/* 开 */
	vd_bool_t (*close)(vd_bsp_qspi_t *qspi);/* 关 */
	vd_bool_t (*cs)(vd_bsp_qspi_t *qspi, vd_uint8_t pin_level);/* 软件片选控制 */
	vd_int32_t (*read)(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint32_t data_len);/* 读 */
	vd_int32_t (*write)(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint32_t data_len);/* 写 */
	vd_int32_t (*transmit)(vd_bsp_qspi_t *qspi, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len);/* 读写数据 */
	vd_int32_t (*write_dma)(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, const vd_uint8_t* tx_data, vd_uint32_t tx_data_len, void(*cb)(void *), void* cb_param);/* 通过DMA写 */
}qspi_ops_t;

/* 平台注册接口 */
typedef vd_bool_t (*qspi_plat_register_t)(qspi_ops_t *ops);


/* ======================================================================================
 * declaration
 */

/**
 * @brief 初始化QSPI
 *
 * @param qspi_id  QSPI 初始化对应ID号
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_init(vd_bsp_port_e qspim_id);

/**
 * @brief 打开QSPI功能
 *
 * @param qspi_id QSPI索引
 * @return vd_bool_t 返回打开结果
 */
vd_int32_t vd_bsp_qspim_open(vd_bsp_port_e qspim_id);

/**
 * @brief 关闭QSPI功能
 *
 * @param qspi_id QSPI索引
 * @return vd_bool_t 返回关闭结果
 */
vd_int32_t vd_bsp_qspim_close(vd_bsp_port_e qspim_id);

/**
 * @brief QSPI软件片选控制
 *
 * @param qspi_id 		QSPI索引
 * @param pin_level 	引脚电平
 * @return vd_bool_t 	返回控制结果
 */
vd_int32_t vd_bsp_qspim_cs_control(vd_bsp_port_e qspim_id, vd_uint8_t pin_level);

/**
 * @brief  读取QSPI数据
 *
 * @param qspi_id 			QSPI索引
 * @param cfg 					读写配置
 * @param data 					读取数据缓存buff
 * @param data_len 			需要读取的数据长度
 * @return vd_uint16_t 	返回实际读取的数据长度
 */
vd_int32_t vd_bsp_qspim_read(vd_bsp_port_e qspim_id, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief 写QSPI数据
 *
 * @param qspi_id : QSPI索引
 * @param cfg 			读写配置
 * @param data : QSPI数据内容缓存buff
 * @param data_len : QSPI数据内容长度
 * @return vd_bool_t : 写入成功或失败
 */
vd_int32_t vd_bsp_qspim_write(vd_bsp_port_e qspim_id, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief qspi同步收发数据传输通道
 *
 * @param qspi_id QSPI索引
 * @param tx_data QSPI发送数据的buff
 * @param tx_data_len QSPI发送数据的数据长度
 * @param rx_data QSPI接收数据的数据缓存buff
 * @param rx_data_len QSPI接收数据的长度
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_transmit(
    vd_bsp_port_e  	qspim_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    vd_uint8_t* rx_data,
    vd_uint32_t rx_data_len);

/**
 * @brief QSPIaster通过DMA方式写数据
 *
 * @param qspi_id QSPI索引号
 * @param cfg 			读写配置
 * @param tx_data QSPI发送数据的buff
 * @param tx_data_len QSPI发送数据的数据长度
 * @param cb 回调函数指针
 * @param cb_param 回调函数
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_write_dma(
    vd_bsp_port_e  						qspim_id,
		vd_qspi_tx_cfg_t 			*cfg,
    const vd_uint8_t 						*tx_data,
    vd_uint32_t 					tx_data_len,
    void (*cb)(void*),
    void* cb_param);
/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // _DRV_QSPI_H
