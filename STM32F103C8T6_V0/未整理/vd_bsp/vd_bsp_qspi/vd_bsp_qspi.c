/*
 * Project: MCU_kit
 * Module: vd_bsp_qspi.c
 * File: vd_bsp_qspi.c
 * Created Date: 2022-03-24 15:19:11
 * Author: GanWenXu
 * Description: QSPI驱动
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */

/* ======================================================================================
 * includes
 */
#include "vd_bsp_qspi/vd_bsp_qspi.h"
#include "platform_config.h"
#include "vd_log.h"
#include "string.h"

/* ======================================================================================
 * macros
 */
#ifndef PLATFORM_QSPI_REGISTER
#define PLATFORM_QSPI_REGISTER vd_null
#endif

/* ======================================================================================
 * types
 */

/* QSPI驱动对象 */
typedef struct{
	qspi_plat_register_t 	plat_register;		/* qspi平台注册接口 */
	qspi_ops_t 			ops;				/* qspi平台接口 */
}qspi_drv_t;

/* ======================================================================================
 * declaration
 */

static void qspim_dma_tx_cplt_handler(void *arg);//dma发送完成处理
/* ======================================================================================
 * globals
 */
 

/* 定义QSPI驱动对象 */
static qspi_drv_t qspi_drv = {
	.plat_register = PLATFORM_QSPI_REGISTER,
	.ops = {0},
};

/* qspi配置列表 */
static vd_bsp_qspi_t g_vd_bsp_qspi[] = {
	#if (PLATFORM_QSPIM0_EN == ON)
		{
			.id = VD_PORT_QSPIM0,
			.cs_pin = PLATFORM_QSPIM0_CS_PIN,  
			.sclk_pin = PLATFORM_QSPIM0_SCLK_PIN, 
			.io0_pin = PLATFORM_QSPIM0_IO0_PIN,
			.io1_pin = PLATFORM_QSPIM0_IO1_PIN, 
			.io2_pin = PLATFORM_QSPIM0_IO2_PIN,
			.io3_pin = PLATFORM_QSPIM0_IO3_PIN, 
			.cs_mode = PLATFORM_QSPIM0_CS_MODE,
			.data_bit = PLATFORM_QSPIM0_DATA_BIT, 
			.speed = PLATFORM_QSPIM0_SPEED,
			.mode = PLATFORM_QSPIM0_MODE,
			.use_dma = PLATFORM_QSPIM0_DMA_EN,
			.dma_channel = PLATFORM_QSPIM0_DMA_CHANNEL,
		},
	#endif

};
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */





/**
 * @brief 根据ID索引查找驱动
 *
 * @param qspi_id QSPIM对应的ID号
 * @return vd_bsp_qspi_t*
 */
static vd_bsp_qspi_t* vd_bsp_qspi_find(vd_bsp_port_e qspi_id) 
{
    for (int i = 0; i < vd_array_size(g_vd_bsp_qspi); i++) {
        if (g_vd_bsp_qspi[i].id == qspi_id) {
            return &g_vd_bsp_qspi[i];
        }
    }
    return vd_null;
}

/* ======================================================================================
 * implementation
 */

/**
 * @brief 初始化QSPIM
 *
 * @param qspim_id  QSPI 初始化对应ID号
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_init(vd_bsp_port_e qspim_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;

	/* 平台注册 */
	vd_check_return_val_error(qspi_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "platform qspi not bound\r\n");
	ret = qspi_drv.plat_register(&qspi_drv.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "platform qspi register fail\r\n");

	/* 参数校验 */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* 遍历qspi配置列表 */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "qspi[%d] not found\r\n", qspim_id - VD_PORT_QSPIM0);

	/* 参数校验 */
	vd_check_return_val_error(vd_bsp_qspi->mode >= PALTFORM_QSPI_WORK_MODE_MAX, -VD_BSP_ERR_PARAMETER, "qspi[%d] mode error\r\n", qspim_id - VD_PORT_QSPIM0);
	vd_check_return_val_error(vd_bsp_qspi->cs_mode >= PALTFORM_QSPI_CS_MODE_MAX, -VD_BSP_ERR_PARAMETER, "qspi[%d] cs mode error\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* qspi初始化 */
	vd_check_return_val_error(qspi_drv.ops.init == vd_null, -VD_BSP_ERR_SDK, "qspi init port is null\r\n");
	ret = qspi_drv.ops.init(vd_bsp_qspi);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform qspi[%d] init error\r\n", qspim_id - VD_PORT_QSPIM0);

  return VD_BSP_NOERROR;
}
/**
 * @brief 打开QSPIM功能
 *
 * @param qspim_id QSPIM索引
 * @return vd_bool_t 返回打开结果
 */
vd_int32_t vd_bsp_qspim_open(vd_bsp_port_e qspim_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* 参数校验 */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* 遍历qspi配置列表 */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* qspi打开 */
	vd_check_return_val_error(qspi_drv.ops.open == vd_null, -VD_BSP_ERR_SDK, "qspi open port is null\r\n");
	ret = qspi_drv.ops.open(vd_bsp_qspi);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform qspi[%d] open error\r\n", qspim_id - VD_PORT_QSPIM0);

	return VD_BSP_NOERROR;
}
/**
 * @brief 关闭QSPIM功能
 *
 * @param qspim_id QSPIM索引
 * @return vd_bool_t 返回关闭结果
 */
vd_int32_t vd_bsp_qspim_close(vd_bsp_port_e qspim_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* 参数校验 */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* 遍历qspi配置列表 */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);

	/* qspi关闭 */
	vd_check_return_val_error(qspi_drv.ops.close == vd_null, -VD_BSP_ERR_SDK, "qspi close port is null\r\n");
	ret = qspi_drv.ops.close(vd_bsp_qspi);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform qspi[%d] close error\r\n", qspim_id - VD_PORT_QSPIM0);

	return VD_BSP_NOERROR;
}

/**
 * @brief QSPI软件片选控制
 *
 * @param qspim_id 		QSPI索引
 * @param pin_level 	引脚电平
 * @return vd_bool_t 	返回控制结果
 */
vd_int32_t vd_bsp_qspim_cs_control(vd_bsp_port_e qspim_id, vd_uint8_t pin_level)
{
	vd_bool_t   ret = vd_false;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* 参数校验 */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* 遍历qspi配置列表 */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* 检查是否为软件片选 */
	vd_check_return_val_error(vd_bsp_qspi->cs_mode == PALTFORM_QSPI_CS_MODE_HARDWARE, -VD_BSP_ERR_PARAMETER, "qspi[%d] is hardware chip select\r\n", qspim_id);

	/* qspi软件片选 */
	vd_check_return_val_error(qspi_drv.ops.cs == vd_null, -VD_BSP_ERR_SDK, "qspi cs port is null\r\n");
	ret = qspi_drv.ops.cs(vd_bsp_qspi, pin_level);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform qspi[%d] cs error\r\n", qspim_id - VD_PORT_QSPIM0);

	return VD_BSP_NOERROR;
}

/**
 * @brief  读取QSPIM数据
 *
 * @param qspim_id QSPIM索引
 * @param cfg 			读写配置
 * @param data 读取数据缓存buff
 * @param data_len 需要读取的数据长度
 * @return vd_uint16_t 返回实际读取的数据长度
 */
vd_int32_t vd_bsp_qspim_read(vd_bsp_port_e qspim_id, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* 参数校验 */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* 遍历qspi配置列表 */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* 参数检查 */
	vd_check_return_val_error(cfg == vd_null, -VD_BSP_ERR_PARAMETER);
	vd_check_return_val_error(data_len > 0 && data == vd_null, -VD_BSP_ERR_PARAMETER);
	
	/* qspi读取 */
	vd_check_return_val_error(qspi_drv.ops.read == vd_null, -VD_BSP_ERR_SDK, "qspi read port is null\r\n");
	ret = qspi_drv.ops.read(vd_bsp_qspi, cfg, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform qspi[%d] read error\r\n", qspim_id - VD_PORT_QSPIM0);

	return ret;
}

/**
 * @brief : SIPM写数据
 *
 * @param qspim_id
 * @param cfg 			读写配置
 * @param data
 * @param data_len
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_write(vd_bsp_port_e qspim_id, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* 参数校验 */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* 遍历qspi配置列表 */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* 参数检查 */
	vd_check_return_val_error(cfg == vd_null, -VD_BSP_ERR_PARAMETER);
	vd_check_return_val_error(cfg->line >= QSPI_LINE_MAX, -VD_BSP_ERR_PARAMETER);
	vd_check_return_val_error(data_len > 0 && data == vd_null, -VD_BSP_ERR_PARAMETER);

	/* qspi写数据 */
	vd_check_return_val_error(qspi_drv.ops.write == vd_null, -VD_BSP_ERR_SDK, "qspi write port is null\r\n");
	ret = qspi_drv.ops.write(vd_bsp_qspi, cfg, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform qspi[%d] write error\r\n", qspim_id - VD_PORT_QSPIM0);

	return ret;
}

/**
 * @brief qspi同步收发数据传输通道
 *
 * @param qspim_id QSPIM索引
 * @param tx_data QSPIM发送数据的buff
 * @param tx_data_len QSPIM发送数据的数据长度
 * @param rx_data QSPIM接收数据的数据缓存buff
 * @param rx_data_len QSPIM接收数据的长度
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_transmit(
	vd_bsp_port_e  	qspim_id,
	vd_uint8_t* tx_data,
	vd_uint32_t tx_data_len,
	vd_uint8_t* rx_data,
	vd_uint32_t rx_data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
			
	/* 参数校验 */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* 遍历qspi配置列表 */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
			
	/* qspi读写数据 */
	vd_check_return_val_error(qspi_drv.ops.transmit == vd_null, -VD_BSP_ERR_SDK, "qspi transmit port is null\r\n");
	ret = qspi_drv.ops.transmit(vd_bsp_qspi, tx_data, tx_data_len, rx_data, rx_data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform qspi[%d] transmit error\r\n", qspim_id - VD_PORT_QSPIM0);
		
	return ret;
}

/**
 * @brief QSPIMaster通过DMA方式写数据
 *
 * @param qspim_id QSPIM索引号
 * @param cfg 			读写配置
 * @param tx_data QSPIM发送数据的buff
 * @param tx_data_len QSPIM发送数据的数据长度
 * @param cb 回调函数指针
 * @param cb_param 回调函数
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_write_dma(
	vd_bsp_port_e  					qspim_id,
	vd_qspi_tx_cfg_t 		*cfg,
	const vd_uint8_t					*tx_data,
	vd_uint32_t 				tx_data_len,
	void (*cb)(void*),
	void* cb_param) 
{
	vd_int32_t   ret = 0;
	vd_bsp_qspi_t* hqspi = vd_null;
	
	/* 参数校验 */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* 遍历qspi配置列表 */
	hqspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(hqspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);

	/* 参数检查 */
	vd_check_return_val_error(cfg == vd_null, -VD_BSP_ERR_PARAMETER);
	vd_check_return_val_error(tx_data_len > 0 && tx_data == vd_null, -VD_BSP_ERR_PARAMETER);
	
	//申请DMA发送缓存，将数据拷贝到发送缓存
//	while(hqspi->dma_buf);//等待上一次发送完成
//	hqspi->dma_buf = PLATFORM_MALLOC(tx_data_len);
//	vd_check_return_val_error(hqspi->dma_buf == vd_null, -VD_BSP_ERR_MALLOC, "qspi write_dma malloc error\r\n");
//	memcpy(hqspi->dma_buf, tx_data, tx_data_len);
		
	/* qspi DMA写数据 */
	if(qspi_drv.ops.write_dma == vd_null){
		log_error("qspi write_dma port is null\r\n");
//		PLATFORM_FREE(hqspi->dma_buf);
		return -VD_BSP_ERR_SDK;
	}
	
	//绑定回调
	hqspi->tx_cplt_cb = cb;
	hqspi->cb_arg = cb_param;
	
	ret = qspi_drv.ops.write_dma(hqspi, cfg, tx_data, tx_data_len, qspim_dma_tx_cplt_handler, hqspi);
	if(ret < 0){
		log_error("platform qspi[%d] write_dma error\r\n", qspim_id - VD_PORT_QSPIM0);
//		PLATFORM_FREE(hqspi->dma_buf);
		return -VD_BSP_ERR_SDK;
	}
	
	return ret;
}

/**
 * @brief dma发送完成处理
 * @param arg 回调参数
 * @return void
 */
static void qspim_dma_tx_cplt_handler(void *arg)
{
	vd_bsp_qspi_t *hqspi = arg;
//	log_debug("in cb\r\n");
	
//	PLATFORM_FREE(hqspi->dma_buf);//释放发送缓存
	hqspi->dma_buf = vd_null;
	
	if(hqspi->tx_cplt_cb){//执行回调
		hqspi->tx_cplt_cb(hqspi->cb_arg);
	}
}
	

