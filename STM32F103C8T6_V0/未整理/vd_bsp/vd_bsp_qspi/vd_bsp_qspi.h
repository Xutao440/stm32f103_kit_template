/*
 * Project: MCU_kit
 * Module: DRV_QSPI
 * File: vd_bsp_spi.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_spiͷ�ļ�
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

/* qspi����ģʽ */
typedef enum  
{
    PALTFORM_QSPI_WORK_MODE_0 = 0,    /* Idle: Low ; sample: first  edge */
    PALTFORM_QSPI_WORK_MODE_1,    /* Idle: Low ; sample: second edge */
    PALTFORM_QSPI_WORK_MODE_2,    /* Idle: High; sample: first  edge */
    PALTFORM_QSPI_WORK_MODE_3,    /* Idle: High; sample: second edge */
		PALTFORM_QSPI_WORK_MODE_MAX,
}plat_qspi_mode_e;

/* qspiƬѡģʽ */
typedef enum  
{
    PALTFORM_QSPI_CS_MODE_SOFTWARE = 0,    /* ���Ƭѡ */
    PALTFORM_QSPI_CS_MODE_HARDWARE,    /* Ӳ��Ƭѡ */
		PALTFORM_QSPI_CS_MODE_MAX,
}plat_qspi_cs_mode_e;


typedef enum{
	QSPI_LINE_X1 = 0,
	QSPI_LINE_X2,
	QSPI_LINE_X4,
	QSPI_LINE_MAX,
}qspi_line_e;

/* qspim�����ṹ�� */
typedef struct {	
    vd_bsp_port_e  id;						//QSPI���� 

		vd_int8_t  sclk_pin;			//ʱ������
    vd_int8_t  cs_pin;   			// QSPIƬѡ��
    vd_int8_t  io0_pin;				// io0
		vd_int8_t  io1_pin;				// io1
		vd_int8_t  io2_pin;
		vd_int8_t  io3_pin;

		vd_uint8_t	mode;						//����ģʽ ֵ plat_qspi_mode_e
    vd_uint8_t  cs_mode;			//Ƭѡģʽ		ֵ plat_qspi_cs_mode_e
    vd_uint8_t  data_bit;			//����λ
    vd_uint32_t speed;			

		vd_bool_t 	use_dma;				//�Ƿ�ʹ��dma
		vd_uint8_t	dma_channel;		//dmaͨ��ѡ��
	
		vd_uint8_t	*dma_buf;				//dma���ͻ���
		vd_void_t 	(*tx_cplt_cb)(vd_void_t *arg);//DMA������ɻص�
		vd_void_t 	*cb_arg;//�ص�����
} vd_bsp_qspi_t;

/* ����qspi�շ��������� */
typedef struct{
	vd_uint32_t instruct;			/* ָ�� */
	vd_uint32_t address;			/* ��ַ */
	vd_uint8_t instruct_len;	/* ָ��� */
	vd_uint8_t address_len;		/* ��ַ���� */
	qspi_line_e line;					/* ������ */
}vd_qspi_tx_cfg_t;

/* ����QSPIƽ̨�ӿ� */
typedef struct{
	vd_bool_t (*init)(vd_bsp_qspi_t *qspi);/* ��ʼ�� */
	vd_bool_t (*open)(vd_bsp_qspi_t *qspi);/* �� */
	vd_bool_t (*close)(vd_bsp_qspi_t *qspi);/* �� */
	vd_bool_t (*cs)(vd_bsp_qspi_t *qspi, vd_uint8_t pin_level);/* ���Ƭѡ���� */
	vd_int32_t (*read)(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint32_t data_len);/* �� */
	vd_int32_t (*write)(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint32_t data_len);/* д */
	vd_int32_t (*transmit)(vd_bsp_qspi_t *qspi, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len);/* ��д���� */
	vd_int32_t (*write_dma)(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, const vd_uint8_t* tx_data, vd_uint32_t tx_data_len, void(*cb)(void *), void* cb_param);/* ͨ��DMAд */
}qspi_ops_t;

/* ƽ̨ע��ӿ� */
typedef vd_bool_t (*qspi_plat_register_t)(qspi_ops_t *ops);


/* ======================================================================================
 * declaration
 */

/**
 * @brief ��ʼ��QSPI
 *
 * @param qspi_id  QSPI ��ʼ����ӦID��
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_init(vd_bsp_port_e qspim_id);

/**
 * @brief ��QSPI����
 *
 * @param qspi_id QSPI����
 * @return vd_bool_t ���ش򿪽��
 */
vd_int32_t vd_bsp_qspim_open(vd_bsp_port_e qspim_id);

/**
 * @brief �ر�QSPI����
 *
 * @param qspi_id QSPI����
 * @return vd_bool_t ���عرս��
 */
vd_int32_t vd_bsp_qspim_close(vd_bsp_port_e qspim_id);

/**
 * @brief QSPI���Ƭѡ����
 *
 * @param qspi_id 		QSPI����
 * @param pin_level 	���ŵ�ƽ
 * @return vd_bool_t 	���ؿ��ƽ��
 */
vd_int32_t vd_bsp_qspim_cs_control(vd_bsp_port_e qspim_id, vd_uint8_t pin_level);

/**
 * @brief  ��ȡQSPI����
 *
 * @param qspi_id 			QSPI����
 * @param cfg 					��д����
 * @param data 					��ȡ���ݻ���buff
 * @param data_len 			��Ҫ��ȡ�����ݳ���
 * @return vd_uint16_t 	����ʵ�ʶ�ȡ�����ݳ���
 */
vd_int32_t vd_bsp_qspim_read(vd_bsp_port_e qspim_id, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief дQSPI����
 *
 * @param qspi_id : QSPI����
 * @param cfg 			��д����
 * @param data : QSPI�������ݻ���buff
 * @param data_len : QSPI�������ݳ���
 * @return vd_bool_t : д��ɹ���ʧ��
 */
vd_int32_t vd_bsp_qspim_write(vd_bsp_port_e qspim_id, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief qspiͬ���շ����ݴ���ͨ��
 *
 * @param qspi_id QSPI����
 * @param tx_data QSPI�������ݵ�buff
 * @param tx_data_len QSPI�������ݵ����ݳ���
 * @param rx_data QSPI�������ݵ����ݻ���buff
 * @param rx_data_len QSPI�������ݵĳ���
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_qspim_transmit(
    vd_bsp_port_e  	qspim_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    vd_uint8_t* rx_data,
    vd_uint32_t rx_data_len);

/**
 * @brief QSPIasterͨ��DMA��ʽд����
 *
 * @param qspi_id QSPI������
 * @param cfg 			��д����
 * @param tx_data QSPI�������ݵ�buff
 * @param tx_data_len QSPI�������ݵ����ݳ���
 * @param cb �ص�����ָ��
 * @param cb_param �ص�����
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
