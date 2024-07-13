/*
 * Project: MCU_kit
 * Module: vd_bsp_qspi.c
 * File: vd_bsp_qspi.c
 * Created Date: 2022-03-24 15:19:11
 * Author: GanWenXu
 * Description: QSPI����
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

/* QSPI�������� */
typedef struct{
	qspi_plat_register_t 	plat_register;		/* qspiƽ̨ע��ӿ� */
	qspi_ops_t 			ops;				/* qspiƽ̨�ӿ� */
}qspi_drv_t;

/* ======================================================================================
 * declaration
 */

static void qspim_dma_tx_cplt_handler(void *arg);//dma������ɴ���
/* ======================================================================================
 * globals
 */
 

/* ����QSPI�������� */
static qspi_drv_t qspi_drv = {
	.plat_register = PLATFORM_QSPI_REGISTER,
	.ops = {0},
};

/* qspi�����б� */
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
 * @brief ����ID������������
 *
 * @param qspi_id QSPIM��Ӧ��ID��
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
 * @brief ��ʼ��QSPIM
 *
 * @param qspim_id  QSPI ��ʼ����ӦID��
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_init(vd_bsp_port_e qspim_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;

	/* ƽ̨ע�� */
	vd_check_return_val_error(qspi_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "platform qspi not bound\r\n");
	ret = qspi_drv.plat_register(&qspi_drv.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "platform qspi register fail\r\n");

	/* ����У�� */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* ����qspi�����б� */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "qspi[%d] not found\r\n", qspim_id - VD_PORT_QSPIM0);

	/* ����У�� */
	vd_check_return_val_error(vd_bsp_qspi->mode >= PALTFORM_QSPI_WORK_MODE_MAX, -VD_BSP_ERR_PARAMETER, "qspi[%d] mode error\r\n", qspim_id - VD_PORT_QSPIM0);
	vd_check_return_val_error(vd_bsp_qspi->cs_mode >= PALTFORM_QSPI_CS_MODE_MAX, -VD_BSP_ERR_PARAMETER, "qspi[%d] cs mode error\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* qspi��ʼ�� */
	vd_check_return_val_error(qspi_drv.ops.init == vd_null, -VD_BSP_ERR_SDK, "qspi init port is null\r\n");
	ret = qspi_drv.ops.init(vd_bsp_qspi);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform qspi[%d] init error\r\n", qspim_id - VD_PORT_QSPIM0);

  return VD_BSP_NOERROR;
}
/**
 * @brief ��QSPIM����
 *
 * @param qspim_id QSPIM����
 * @return vd_bool_t ���ش򿪽��
 */
vd_int32_t vd_bsp_qspim_open(vd_bsp_port_e qspim_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* ����qspi�����б� */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* qspi�� */
	vd_check_return_val_error(qspi_drv.ops.open == vd_null, -VD_BSP_ERR_SDK, "qspi open port is null\r\n");
	ret = qspi_drv.ops.open(vd_bsp_qspi);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform qspi[%d] open error\r\n", qspim_id - VD_PORT_QSPIM0);

	return VD_BSP_NOERROR;
}
/**
 * @brief �ر�QSPIM����
 *
 * @param qspim_id QSPIM����
 * @return vd_bool_t ���عرս��
 */
vd_int32_t vd_bsp_qspim_close(vd_bsp_port_e qspim_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* ����qspi�����б� */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);

	/* qspi�ر� */
	vd_check_return_val_error(qspi_drv.ops.close == vd_null, -VD_BSP_ERR_SDK, "qspi close port is null\r\n");
	ret = qspi_drv.ops.close(vd_bsp_qspi);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform qspi[%d] close error\r\n", qspim_id - VD_PORT_QSPIM0);

	return VD_BSP_NOERROR;
}

/**
 * @brief QSPI���Ƭѡ����
 *
 * @param qspim_id 		QSPI����
 * @param pin_level 	���ŵ�ƽ
 * @return vd_bool_t 	���ؿ��ƽ��
 */
vd_int32_t vd_bsp_qspim_cs_control(vd_bsp_port_e qspim_id, vd_uint8_t pin_level)
{
	vd_bool_t   ret = vd_false;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* ����qspi�����б� */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* ����Ƿ�Ϊ���Ƭѡ */
	vd_check_return_val_error(vd_bsp_qspi->cs_mode == PALTFORM_QSPI_CS_MODE_HARDWARE, -VD_BSP_ERR_PARAMETER, "qspi[%d] is hardware chip select\r\n", qspim_id);

	/* qspi���Ƭѡ */
	vd_check_return_val_error(qspi_drv.ops.cs == vd_null, -VD_BSP_ERR_SDK, "qspi cs port is null\r\n");
	ret = qspi_drv.ops.cs(vd_bsp_qspi, pin_level);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform qspi[%d] cs error\r\n", qspim_id - VD_PORT_QSPIM0);

	return VD_BSP_NOERROR;
}

/**
 * @brief  ��ȡQSPIM����
 *
 * @param qspim_id QSPIM����
 * @param cfg 			��д����
 * @param data ��ȡ���ݻ���buff
 * @param data_len ��Ҫ��ȡ�����ݳ���
 * @return vd_uint16_t ����ʵ�ʶ�ȡ�����ݳ���
 */
vd_int32_t vd_bsp_qspim_read(vd_bsp_port_e qspim_id, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* ����qspi�����б� */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* ������� */
	vd_check_return_val_error(cfg == vd_null, -VD_BSP_ERR_PARAMETER);
	vd_check_return_val_error(data_len > 0 && data == vd_null, -VD_BSP_ERR_PARAMETER);
	
	/* qspi��ȡ */
	vd_check_return_val_error(qspi_drv.ops.read == vd_null, -VD_BSP_ERR_SDK, "qspi read port is null\r\n");
	ret = qspi_drv.ops.read(vd_bsp_qspi, cfg, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform qspi[%d] read error\r\n", qspim_id - VD_PORT_QSPIM0);

	return ret;
}

/**
 * @brief : SIPMд����
 *
 * @param qspim_id
 * @param cfg 			��д����
 * @param data
 * @param data_len
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_write(vd_bsp_port_e qspim_id, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_qspi_t* vd_bsp_qspi = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* ����qspi�����б� */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
	
	/* ������� */
	vd_check_return_val_error(cfg == vd_null, -VD_BSP_ERR_PARAMETER);
	vd_check_return_val_error(cfg->line >= QSPI_LINE_MAX, -VD_BSP_ERR_PARAMETER);
	vd_check_return_val_error(data_len > 0 && data == vd_null, -VD_BSP_ERR_PARAMETER);

	/* qspiд���� */
	vd_check_return_val_error(qspi_drv.ops.write == vd_null, -VD_BSP_ERR_SDK, "qspi write port is null\r\n");
	ret = qspi_drv.ops.write(vd_bsp_qspi, cfg, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform qspi[%d] write error\r\n", qspim_id - VD_PORT_QSPIM0);

	return ret;
}

/**
 * @brief qspiͬ���շ����ݴ���ͨ��
 *
 * @param qspim_id QSPIM����
 * @param tx_data QSPIM�������ݵ�buff
 * @param tx_data_len QSPIM�������ݵ����ݳ���
 * @param rx_data QSPIM�������ݵ����ݻ���buff
 * @param rx_data_len QSPIM�������ݵĳ���
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
			
	/* ����У�� */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* ����qspi�����б� */
	vd_bsp_qspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(vd_bsp_qspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);
			
	/* qspi��д���� */
	vd_check_return_val_error(qspi_drv.ops.transmit == vd_null, -VD_BSP_ERR_SDK, "qspi transmit port is null\r\n");
	ret = qspi_drv.ops.transmit(vd_bsp_qspi, tx_data, tx_data_len, rx_data, rx_data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform qspi[%d] transmit error\r\n", qspim_id - VD_PORT_QSPIM0);
		
	return ret;
}

/**
 * @brief QSPIMasterͨ��DMA��ʽд����
 *
 * @param qspim_id QSPIM������
 * @param cfg 			��д����
 * @param tx_data QSPIM�������ݵ�buff
 * @param tx_data_len QSPIM�������ݵ����ݳ���
 * @param cb �ص�����ָ��
 * @param cb_param �ص�����
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
	
	/* ����У�� */
	vd_check_return_val_error(qspim_id < VD_PORT_QSPIM0 || qspim_id >= VD_PORT_QSPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "func:%s, line:%d, qspim_id = %d, error\r\n",__func__, __LINE__, qspim_id);
	
	/* ����qspi�����б� */
	hqspi = vd_bsp_qspi_find(qspim_id);
	vd_check_return_val_error(hqspi == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No qspi[%d]\r\n", qspim_id - VD_PORT_QSPIM0);

	/* ������� */
	vd_check_return_val_error(cfg == vd_null, -VD_BSP_ERR_PARAMETER);
	vd_check_return_val_error(tx_data_len > 0 && tx_data == vd_null, -VD_BSP_ERR_PARAMETER);
	
	//����DMA���ͻ��棬�����ݿ��������ͻ���
//	while(hqspi->dma_buf);//�ȴ���һ�η������
//	hqspi->dma_buf = PLATFORM_MALLOC(tx_data_len);
//	vd_check_return_val_error(hqspi->dma_buf == vd_null, -VD_BSP_ERR_MALLOC, "qspi write_dma malloc error\r\n");
//	memcpy(hqspi->dma_buf, tx_data, tx_data_len);
		
	/* qspi DMAд���� */
	if(qspi_drv.ops.write_dma == vd_null){
		log_error("qspi write_dma port is null\r\n");
//		PLATFORM_FREE(hqspi->dma_buf);
		return -VD_BSP_ERR_SDK;
	}
	
	//�󶨻ص�
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
 * @brief dma������ɴ���
 * @param arg �ص�����
 * @return void
 */
static void qspim_dma_tx_cplt_handler(void *arg)
{
	vd_bsp_qspi_t *hqspi = arg;
//	log_debug("in cb\r\n");
	
//	PLATFORM_FREE(hqspi->dma_buf);//�ͷŷ��ͻ���
	hqspi->dma_buf = vd_null;
	
	if(hqspi->tx_cplt_cb){//ִ�лص�
		hqspi->tx_cplt_cb(hqspi->cb_arg);
	}
}
	

