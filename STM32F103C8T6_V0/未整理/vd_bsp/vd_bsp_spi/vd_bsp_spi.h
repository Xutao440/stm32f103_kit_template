/*
 * Project: MCU_kit
 * Module: DRV_SPI
 * File: vd_bsp_spi.h
 * Created Date: 2022-03-24 18:09:33
 * Author: GanWenXu
 * Description: vd_bsp_spiͷ�ļ�
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

/* spi����ģʽ */
typedef enum  
{
    PALTFORM_SPI_WORK_MODE_0,    /* Idle: Low ; sample: first  edge */
    PALTFORM_SPI_WORK_MODE_1,    /* Idle: Low ; sample: second edge */
    PALTFORM_SPI_WORK_MODE_2,    /* Idle: High; sample: first  edge */
    PALTFORM_SPI_WORK_MODE_3,    /* Idle: High; sample: second edge */
		PALTFORM_SPI_WORK_MODE_MAX,
}plat_spi_mode_e;

/* spiƬѡģʽ */
typedef enum  
{
    PALTFORM_SPI_CS_MODE_SOFTWARE,    /* ���Ƭѡ */
    PALTFORM_SPI_CS_MODE_HARDWARE,    /* Ӳ��Ƭѡ */
		PALTFORM_SPI_CS_MODE_MAX,
}plat_spi_cs_mode_e;

/* spim�����ṹ�� */
typedef struct {
    vd_bsp_port_e  	id;					//SPI���� 

		vd_int8_t  	sclk_pin;		//ʱ������
    vd_int8_t  	cs_pin;   	// SPIƬѡ��
    vd_int8_t  	mosi_pin;		//mosi����
		vd_int8_t  	miso_pin;		//miso����

		vd_uint8_t	mode;				//����ģʽ ֵ plat_spi_mode_e
    vd_uint8_t  cs_mode;		//Ƭѡģʽ		ֵ plat_spi_cs_mode_e
    vd_uint8_t  data_bit;		//����λ
    vd_uint32_t speed;			

		vd_bool_t 	use_dma;				//�Ƿ�ʹ��dma
		vd_uint8_t	dma_channel;		//dmaͨ��ѡ��

} vd_bsp_spim_t;

/* spis�����ṹ�� */
typedef struct {
    vd_bsp_port_e 	id;
	
		vd_int8_t  	sclk_pin;		//ʱ������
    vd_int8_t 	cs_pin;			//Ƭѡ����
    vd_int8_t  	mosi_pin;		//mosi���ţ�qspi io0
		vd_int8_t  	miso_pin;		//miso���ţ�qspi io1
	
    vd_uint8_t 	data_bit;
    vd_uint32_t speed;			
		vd_uint8_t	mode;						//����ģʽ
	
		vd_bool_t 	use_dma;
		vd_uint8_t	dma_channel;		//dmaͨ��ѡ��
} vd_bsp_spis_t;


/* ����SPIM��ؽӿ� */
typedef struct{
	vd_bool_t (*init)(vd_bsp_spim_t *spim);/* ��ʼ�� */
	vd_bool_t (*open)(vd_bsp_spim_t *spim);/* �� */
	vd_bool_t (*close)(vd_bsp_spim_t *spim);/* �� */
	vd_bool_t (*cs)(vd_bsp_spim_t *qspi, vd_uint8_t pin_level);/* ���Ƭѡ���� */
	vd_int32_t (*read)(vd_bsp_spim_t *spim, vd_uint8_t* data, vd_uint32_t data_len);/* �� */
	vd_int32_t (*write)(vd_bsp_spim_t *spim, vd_uint8_t* data, vd_uint32_t data_len);/* д */
	vd_int32_t (*transmit)(vd_bsp_spim_t *spim, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len);/* ��д���� */
	vd_int32_t (*write_dma)(vd_bsp_spim_t *spim, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, void(*cb)(void *), void* cb_param);/* ͨ��DMAд */
}spim_port_t;

/* ����SPIS��ؽӿ� */
typedef struct{
	vd_bool_t (*init)(vd_bsp_spis_t *spis);/* ��ʼ�� */
	vd_bool_t (*open)(vd_bsp_spis_t *spis);/* �� */
	vd_bool_t (*close)(vd_bsp_spis_t *spis);/* �� */
	vd_int32_t (*read)(vd_bsp_spis_t *spis, vd_uint8_t* data, vd_uint32_t data_len);/* �� */
	vd_int32_t (*write)(vd_bsp_spis_t *spis, vd_uint8_t* data, vd_uint32_t data_len);/* д */
	vd_int32_t (*transmit)(vd_bsp_spis_t *spis, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len);/* ��д���� */
	vd_int32_t (*write_dma)(vd_bsp_spis_t *spis, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, void(*cb)(void *), void* cb_param);/* ͨ��DMAд */
}spis_port_t;

/* ����SPIƽ̨�ӿ� */
typedef struct{
	spim_port_t spim_port;/* spim�ӿ� */
	spis_port_t spis_port;/* spis�ӿ� */
}spi_ops_t;

/* ƽ̨ע��ӿ� */
typedef vd_bool_t (*spi_plat_register_t)(spi_ops_t *ops);


/* ======================================================================================
 * declaration
 */

/**
 * @brief ��ʼ��SPIM
 *
 * @param spim_id  SPIM ��ʼ����ӦID��
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_init(vd_bsp_port_e spim_id);

/**
 * @brief ��SPIM����
 *
 * @param spim_id SPIM����
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_open(vd_bsp_port_e spim_id);

/**
 * @brief �ر�SPIM����
 *
 * @param spim_id SPIM����
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_close(vd_bsp_port_e spim_id);

/**
 * @brief SPIM���Ƭѡ����
 *
 * @param spim_id 			SPI����
 * @param pin_level 	���ŵ�ƽ
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_cs_control(vd_bsp_port_e spim_id, vd_uint8_t pin_level);

/**
 * @brief  ��ȡSPIM����
 *
 * @param spim_id SPIM����
 * @param data ��ȡ���ݻ���buff
 * @param data_len ��Ҫ��ȡ�����ݳ���
 * @return	�Ǹ�ֵ��	ʵ�ʶ�ȡ�����ݳ���
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_read(vd_bsp_port_e spim_id, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief дSPIM����
 *
 * @param spim_id : SPIM����
 * @param data : SPIM�������ݻ���buff
 * @param data_len : SPIM�������ݳ���
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_write(vd_bsp_port_e spim_id, vd_uint8_t* data, vd_uint16_t data_len);

/**
 * @brief spimͬ���շ����ݴ���ͨ��
 *
 * @param spim_id SPIM����
 * @param tx_data SPIM�������ݵ�buff
 * @param tx_data_len SPIM�������ݵ����ݳ���
 * @param rx_data SPIM�������ݵ����ݻ���buff
 * @param rx_data_len SPIM�������ݵĳ���
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_transmit(
    vd_bsp_port_e  	spim_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    vd_uint8_t* rx_data,
    vd_uint32_t rx_data_len);

/**
 * @brief SPIMasterͨ��DMA��ʽд����
 *
 * @param spim_id SPIM������
 * @param tx_data SPIM�������ݵ�buff
 * @param tx_data_len SPIM�������ݵ����ݳ���
 * @param cb �ص�����ָ��
 * @param cb_param �ص�����
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spim_write_dma(
    vd_bsp_port_e  	spim_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    void (*cb)(void*),
    void* cb_param);
/**
 * @brief ��ʼ��SPIS
 *
 * @param spis_id : SPIS����
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_init(vd_bsp_port_e spis_id);
/**
 * @brief ��SPIS����
 *
 * @param spis_id : SPIS����
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_open(vd_bsp_port_e spis_id);
/**
 * @brief �ر�SPIS����
 *
 * @param spis_id : SPIS����
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_close(vd_bsp_port_e spis_id);
/**
 * @brief  ��ȡSPIS����
 *
 * @param spis_id : SPIS����
 * @param data ��ȡ���ݻ���buff
 * @param data_len ��Ҫ��ȡ�����ݳ���
 * @return	�Ǹ�ֵ��	ʵ�ʶ�ȡ�����ݳ���
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_read(vd_bsp_port_e spis_id, vd_uint8_t* data, vd_uint32_t data_len);
/**
 * @brief дSPIS����
 *
 * @param spis_id : SPIS����
 * @param data : SPIS�������ݻ���buff
 * @param data_len : SPIS�������ݳ���
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 */
vd_int32_t vd_bsp_spis_write(vd_bsp_port_e spis_id, vd_uint8_t* data, vd_uint32_t data_len);
/**
 * @brief spisͬ���շ����ݴ���ͨ��
 *
 * @param spis_id SPIS����
 * @param tx_data SPIS�������ݵ�buff
 * @param tx_data_len SPIS�������ݵ����ݳ���
 * @param rx_data SPIS�������ݵ����ݻ���buff
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_spis_transmit(
    vd_bsp_port_e  	spis_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    vd_uint8_t* rx_data,
    vd_uint32_t rx_data_len);

/**
 * @brief SPISlaveͨ��DMA��ʽд����
 *
 * @param spis_id SPIS������
 * @param tx_data SPIS�������ݵ�buff
 * @param tx_data_len SPIS�������ݵ����ݳ���
 * @param cb �ص�����ָ��
 * @param cb_param �ص�����
 * @return	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		�ο� vd_bsp_error_e
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
