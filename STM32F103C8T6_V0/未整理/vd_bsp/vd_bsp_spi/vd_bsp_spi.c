/*
 * Project: MCU_kit
 * Module: vd_bsp_spi.c
 * File: vd_bsp_spi.c
 * Created Date: 2022-03-24 15:19:11
 * Author: GanWenXu
 * Description: SPI����
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
#include "vd_bsp_spi/vd_bsp_spi.h"
#include "platform_config.h"
#include "vd_log.h"

/* ======================================================================================
 * macros
 */
#ifndef PLATFORM_SPI_REGISTER
#define PLATFORM_SPI_REGISTER vd_null
#endif

/* ======================================================================================
 * types
 */

/* SPI�������� */
typedef struct{
	spi_plat_register_t 	plat_register;		/* spiƽ̨ע��ӿ� */
	spi_ops_t 			ops;				/* spiƽ̨�ӿ� */
}spi_drv_t;

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
 

/* ����SPI�������� */
static spi_drv_t spi_drv = {
	.plat_register = PLATFORM_SPI_REGISTER,
	.ops = {0},
};

/* spi�����б� */
static vd_bsp_spim_t g_vd_bsp_spim[] = {
	#if (PLATFORM_SPIM0_EN == ON)
		{
			.id = VD_PORT_SPIM0,
			.cs_pin = PLATFORM_SPIM0_CS_PIN,  
			.sclk_pin = PLATFORM_SPIM0_SCLK_PIN, 
			.mosi_pin = PLATFORM_SPIM0_MOSI_PIN,
			.miso_pin = PLATFORM_SPIM0_MISO_PIN, 
			.cs_mode = PLATFORM_SPIM0_CS_MODE,
			.data_bit = PLATFORM_SPIM0_DATA_BIT, 
			.speed = PLATFORM_SPIM0_SPEED,
			.mode = PLATFORM_SPIM0_MODE,
			.use_dma = PLATFORM_SPIM0_DMA_EN,
			.dma_channel = PLATFORM_SPIM0_DMA_CHANNEL,
		},
	#endif
	#if (PLATFORM_SPIM1_EN == ON)
		{
			.spim_id = VD_PORT_SPIM1, 
			.spim_cs_pin = PLATFORM_SPIM1_CS_PIN, 
			.spim_cmd_pin = PLATFORM_SPIM1_CMD_PIN, 
			.spim_sclk_pin = PLATFORM_SPIM1_SCLK_PIN, 
			.spim_mosi_pin = PLATFORM_SPIM1_IO0_PIN,
			.spim_miso_pin = PLATFORM_SPIM1_IO1_PIN, 
			.spim_io2_pin = PLATFORM_SPIM1_IO2_PIN, 
			.spim_io3_pin = PLATFORM_SPIM1_IO3_PIN, 
			.spim_data_bit = 8, 
			.spim_speed = PLATFORM_SPIM1_SPEED
		},
	#endif
};

static vd_bsp_spis_t g_vd_bsp_spis[] = {
0
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
 * @param spim_id SPIM��Ӧ��ID��
 * @return vd_bsp_spim_t*
 */
static vd_bsp_spim_t* vd_bsp_spim_find(vd_bsp_port_e spim_id) 
{
    for (int i = 0; i < vd_array_size(g_vd_bsp_spim); i++) {
        if (g_vd_bsp_spim[i].id == spim_id) {
            return &g_vd_bsp_spim[i];
        }
    }
    return vd_null;
}
/**
 * @brief ����ID������������
 *
 * @param spis_id SPIS��Ӧ��ID��
 * @return vd_bsp_spis_t*
 */

static vd_bsp_spis_t* vd_bsp_spis_find(vd_bsp_port_e spis_id) 
{
    for (int i = 0; i < vd_array_size(g_vd_bsp_spis); i++) {
        if (g_vd_bsp_spis[i].id == spis_id) {
            return &g_vd_bsp_spis[i];
        }
    }
    return vd_null;
}
/* ======================================================================================
 * implementation
 */

/**
 * @brief ��ʼ��SPIM
 *
 * @param spim_id  SPI ��ʼ����ӦID��
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_spim_init(vd_bsp_port_e spim_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_spim_t* vd_bsp_spim = vd_null;

	/* ƽ̨ע�� */
	vd_check_return_val_error(spi_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "platform spi not bound");
	ret = spi_drv.plat_register(&spi_drv.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "platform register fail");

	/* ����У�� */
	vd_check_return_val_error(spim_id < VD_PORT_SPIM0 || spim_id >= VD_PORT_SPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "spim_id = %d, error", spim_id);
	
	/* ����spim�����б� */
	vd_bsp_spim = vd_bsp_spim_find(spim_id);
	vd_check_return_val_error(vd_bsp_spim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "spim[%d] not found", spim_id - VD_PORT_SPIM0);
	
	/* ����У�� */
	vd_check_return_val_error(vd_bsp_spim->mode >= PALTFORM_SPI_WORK_MODE_MAX, -VD_BSP_ERR_PARAMETER, "spim[%d] mode error", spim_id - VD_PORT_SPIM0);
	vd_check_return_val_error(vd_bsp_spim->cs_mode >= PALTFORM_SPI_CS_MODE_MAX, -VD_BSP_ERR_PARAMETER, "spim[%d] cs mode error", spim_id - VD_PORT_SPIM0);

	/* spim��ʼ�� */
	vd_check_return_val_error(spi_drv.ops.spim_port.init == vd_null, -VD_BSP_ERR_SDK, "spim init port is null");
	ret = spi_drv.ops.spim_port.init(vd_bsp_spim);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform spim[%d] init error", spim_id - VD_PORT_SPIM0);

  return VD_BSP_NOERROR;
}
/**
 * @brief ��SPIM����
 *
 * @param spim_id SPIM����
 * @return vd_bool_t ���ش򿪽��
 */
vd_int32_t vd_bsp_spim_open(vd_bsp_port_e spim_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_spim_t* vd_bsp_spim = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(spim_id < VD_PORT_SPIM0 || spim_id >= VD_PORT_SPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "spim_id = %d, error", spim_id);
	
	/* ����spim�����б� */
	vd_bsp_spim = vd_bsp_spim_find(spim_id);
	vd_check_return_val_error(vd_bsp_spim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spim[%d]", spim_id - VD_PORT_SPIM0);
	
	/* spim�� */
	vd_check_return_val_error(spi_drv.ops.spim_port.open == vd_null, -VD_BSP_ERR_SDK, "spi open port is null");
	ret = spi_drv.ops.spim_port.open(vd_bsp_spim);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform spim[%d] open error", spim_id - VD_PORT_SPIM0);

	return VD_BSP_NOERROR;
}
/**
 * @brief �ر�SPIM����
 *
 * @param spim_id SPIM����
 * @return vd_bool_t ���عرս��
 */
vd_int32_t vd_bsp_spim_close(vd_bsp_port_e spim_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_spim_t* vd_bsp_spim = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(spim_id < VD_PORT_SPIM0 || spim_id >= VD_PORT_SPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "spim_id = %d, error", spim_id);
	
	/* ����spim�����б� */
	vd_bsp_spim = vd_bsp_spim_find(spim_id);
	vd_check_return_val_error(vd_bsp_spim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spim[%d]", spim_id - VD_PORT_SPIM0);

	/* spim�ر� */
	vd_check_return_val_error(spi_drv.ops.spim_port.close == vd_null, -VD_BSP_ERR_SDK, "spim close port is null");
	ret = spi_drv.ops.spim_port.close(vd_bsp_spim);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform spim[%d] close error", spim_id - VD_PORT_SPIM0);

	return VD_BSP_NOERROR;
}

/**
 * @brief SPI���Ƭѡ����
 *
 * @param spi_id 			SPI����
 * @param pin_level 	���ŵ�ƽ
 * @return vd_bool_t 	���ؿ��ƽ��
 */
vd_int32_t vd_bsp_spim_cs_control(vd_bsp_port_e spim_id, vd_uint8_t pin_level)
{
	vd_bool_t   ret = vd_false;
	vd_bsp_spim_t* vd_bsp_spim = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(spim_id < VD_PORT_SPIM0 || spim_id >= VD_PORT_SPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "spim_id = %d, error", spim_id);
	
	/* ����spim�����б� */
	vd_bsp_spim = vd_bsp_spim_find(spim_id);
	vd_check_return_val_error(vd_bsp_spim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spim[%d]", spim_id - VD_PORT_SPIM0);
	
	/* ����Ƿ�Ϊ���Ƭѡ */
	vd_check_return_val_error(vd_bsp_spim->cs_mode == PALTFORM_SPI_CS_MODE_HARDWARE, -VD_BSP_ERR_PARAMETER, "spim[%d] is hardware chip select", spim_id);

	/* qspi���Ƭѡ */
	vd_check_return_val_error(spi_drv.ops.spim_port.cs == vd_null, -VD_BSP_ERR_SDK, "spim cs port is null");
	ret = spi_drv.ops.spim_port.cs(vd_bsp_spim, pin_level);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform spim[%d] cs error", spim_id - VD_PORT_SPIM0);

	return VD_BSP_NOERROR;
}

/**
 * @brief  ��ȡSPIM����
 *
 * @param spim_id SPIM����
 * @param data ��ȡ���ݻ���buff
 * @param data_len ��Ҫ��ȡ�����ݳ���
 * @return vd_uint16_t ����ʵ�ʶ�ȡ�����ݳ���
 */
vd_int32_t vd_bsp_spim_read(vd_bsp_port_e spim_id, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_spim_t* vd_bsp_spim = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(spim_id < VD_PORT_SPIM0 || spim_id >= VD_PORT_SPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "spim_id = %d, error", spim_id);
	
	/* ����spim�����б� */
	vd_bsp_spim = vd_bsp_spim_find(spim_id);
	vd_check_return_val_error(vd_bsp_spim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spim[%d]", spim_id - VD_PORT_SPIM0);
	
	/* spim��ȡ */
	vd_check_return_val_error(spi_drv.ops.spim_port.read == vd_null, -VD_BSP_ERR_SDK, "spim read port is null");
	ret = spi_drv.ops.spim_port.read(vd_bsp_spim, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform spim[%d] read error", spim_id - VD_PORT_SPIM0);

	return ret;
}





/**
 * @brief : SIPMд����
 *
 * @param spim_id
 * @param data
 * @param data_len
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_spim_write(vd_bsp_port_e spim_id, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_spim_t* vd_bsp_spim = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(spim_id < VD_PORT_SPIM0 || spim_id >= VD_PORT_SPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "spim_id = %d, error", spim_id);
	
	/* ����spim�����б� */
	vd_bsp_spim = vd_bsp_spim_find(spim_id);
	vd_check_return_val_error(vd_bsp_spim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spim[%d]", spim_id - VD_PORT_SPIM0);

	/* spimд���� */
	vd_check_return_val_error(spi_drv.ops.spim_port.write == vd_null, -VD_BSP_ERR_SDK, "spim write port is null");
	ret = spi_drv.ops.spim_port.write(vd_bsp_spim, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform spim[%d] write error", spim_id - VD_PORT_SPIM0);

	return vd_true;
}

/**
 * @brief spimͬ���շ����ݴ���ͨ��
 *
 * @param spim_id SPIM����
 * @param tx_data SPIM�������ݵ�buff
 * @param tx_data_len SPIM�������ݵ����ݳ���
 * @param rx_data SPIM�������ݵ����ݻ���buff
 * @param rx_data_len SPIM�������ݵĳ���
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_spim_transmit(
    vd_bsp_port_e  	spim_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    vd_uint8_t* rx_data,
    vd_uint32_t rx_data_len) 
{
			
	vd_int32_t   ret = 0;
	vd_bsp_spim_t* vd_bsp_spim = vd_null;
			
	/* ����У�� */
	vd_check_return_val_error(spim_id < VD_PORT_SPIM0 || spim_id >= VD_PORT_SPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "spim_id = %d, error", spim_id);
	
	/* ����spim�����б� */
	vd_bsp_spim = vd_bsp_spim_find(spim_id);
	vd_check_return_val_error(vd_bsp_spim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spim[%d]", spim_id - VD_PORT_SPIM0);
			
	/* spim��д���� */
	vd_check_return_val_error(spi_drv.ops.spim_port.transmit == vd_null, -VD_BSP_ERR_SDK, "spim transmit port is null");
	ret = spi_drv.ops.spim_port.transmit(vd_bsp_spim, tx_data, tx_data_len, rx_data, rx_data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform spim[%d] transmit error", spim_id - VD_PORT_SPIM0);
		
	return ret;
}

/**
 * @brief SPIMasterͨ��DMA��ʽд����
 *
 * @param spim_id SPIM������
 * @param tx_data SPIM�������ݵ�buff
 * @param tx_data_len SPIM�������ݵ����ݳ���
 * @param cb �ص�����ָ��
 * @param cb_param �ص�����
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_spim_write_dma(
    vd_bsp_port_e  	spim_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    void (*cb)(void*),
    void* cb_param) 
{
			
	vd_int32_t   ret = 0;
	vd_bsp_spim_t* vd_bsp_spim = vd_null;
		
	/* ����У�� */
	vd_check_return_val_error(spim_id < VD_PORT_SPIM0 || spim_id >= VD_PORT_SPIM_MAX, -VD_BSP_ERR_PORT_INVALID, "spim_id = %d, error", spim_id);
	
	/* ����spim�����б� */
	vd_bsp_spim = vd_bsp_spim_find(spim_id);
	vd_check_return_val_error(vd_bsp_spim == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spim[%d]", spim_id - VD_PORT_SPIM0);

	/* spim DMAд���� */
	vd_check_return_val_error(spi_drv.ops.spim_port.write_dma == vd_null, -VD_BSP_ERR_SDK, "spim write_dma port is null");
	ret = spi_drv.ops.spim_port.write_dma(vd_bsp_spim, tx_data, tx_data_len, cb, cb_param);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform spim[%d] write_dma error", spim_id - VD_PORT_SPIM0);
		
	return ret;
}
		
/**
 * @brief ��ʼ��SPIS
 *
 * @param spis_id : SPIS����
 * @return vd_bool_t ���س�ʼ�����
 */
vd_int32_t vd_bsp_spis_init(vd_bsp_port_e spis_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_spis_t* vd_bsp_spis = vd_null;
	
	/* ƽ̨ע�� */
	vd_check_return_val_error(spi_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER, "platform spi not bound");
	ret = spi_drv.plat_register(&spi_drv.ops);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_REGISTER, "platform register fail");
	
	/* ����У�� */
	vd_check_return_val_error(spis_id < VD_PORT_SPIS0 || spis_id >= VD_PORT_SPIS_MAX, -VD_BSP_ERR_PORT_INVALID, "spis_id = %d, error", spis_id);
	
	/* ����spis�����б� */
	vd_bsp_spis = vd_bsp_spis_find(spis_id);
	vd_check_return_val_error(vd_bsp_spis == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spis[%d]", spis_id - VD_PORT_SPIS0);
	
	/* ����У�� */
	vd_check_return_val_error(vd_bsp_spis->mode >= PALTFORM_SPI_WORK_MODE_MAX, -VD_BSP_ERR_PARAMETER, "spis[%d] mode error", spis_id - VD_PORT_SPIS0);
	vd_check_return_val_error(vd_bsp_spis->mode >= PALTFORM_SPI_CS_MODE_MAX, -VD_BSP_ERR_PARAMETER, "spis[%d] cs mode error", spis_id - VD_PORT_SPIS0);
	
	/* spis��ʼ�� */
	vd_check_return_val_error(spi_drv.ops.spis_port.init == vd_null, -VD_BSP_ERR_SDK, "spis init port is null");
	ret = spi_drv.ops.spis_port.init(vd_bsp_spis);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform spis[%d] init error", spis_id - VD_PORT_SPIS0);

	return VD_BSP_NOERROR;	
}
/**
 * @brief ��SPIS����
 *
 * @param spis_id : SPIS����
 * @return vd_bool_t ���عرս��
 */
vd_int32_t vd_bsp_spis_open(vd_bsp_port_e spis_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_spis_t* vd_bsp_spis = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(spis_id < VD_PORT_SPIS0 || spis_id >= VD_PORT_SPIS_MAX, -VD_BSP_ERR_PORT_INVALID, "spis_id = %d, error", spis_id);
	
	/* ����spis�����б� */
	vd_bsp_spis = vd_bsp_spis_find(spis_id);
	vd_check_return_val_error(vd_bsp_spis == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spis[%d]", spis_id - VD_PORT_SPIS0);
	
	/* spis�� */
	vd_check_return_val_error(spi_drv.ops.spis_port.open == vd_null, -VD_BSP_ERR_SDK, "spis open port is null");
	ret = spi_drv.ops.spis_port.open(vd_bsp_spis);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform spis[%d] open error", spis_id - VD_PORT_SPIS0);

	return VD_BSP_NOERROR;	
}
/**
 * @brief �ر�SPIS����
 *
 * @param spis_id : SPIS����
 * @return vd_bool_t ���عرս��
 */
vd_int32_t vd_bsp_spis_close(vd_bsp_port_e spis_id) 
{
	vd_bool_t   ret = vd_false;
	vd_bsp_spis_t* vd_bsp_spis = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(spis_id < VD_PORT_SPIS0 || spis_id >= VD_PORT_SPIS_MAX, -VD_BSP_ERR_PORT_INVALID, "spis_id = %d, error", spis_id);
	
	/* ����spis�����б� */
	vd_bsp_spis = vd_bsp_spis_find(spis_id);
	vd_check_return_val_error(vd_bsp_spis == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spis[%d]", spis_id - VD_PORT_SPIS0);

	/* spis�ر� */
	vd_check_return_val_error(spi_drv.ops.spis_port.close == vd_null, -VD_BSP_ERR_SDK, "spis close port is null");
	ret = spi_drv.ops.spis_port.close(vd_bsp_spis);
	vd_check_return_val_error(ret == vd_false, -VD_BSP_ERR_SDK, "platform spis[%d] close error", spis_id - VD_PORT_SPIS0);

	return VD_BSP_NOERROR;	
}
/**
 * @brief  ��ȡSPIS����
 *
 * @param spis_id : SPIS����
 * @param data ��ȡ���ݻ���buff
 * @param data_len ��Ҫ��ȡ�����ݳ���
 * @return vd_uint32_t ����ʵ�ʶ�ȡ�����ݳ���
 */
vd_int32_t vd_bsp_spis_read(vd_bsp_port_e spis_id, vd_uint8_t* data, vd_uint32_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_spis_t* vd_bsp_spis = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(spis_id < VD_PORT_SPIS0 || spis_id >= VD_PORT_SPIS_MAX, -VD_BSP_ERR_PORT_INVALID, "spis_id = %d, error", spis_id);
	
	/* ����spis�����б� */
	vd_bsp_spis = vd_bsp_spis_find(spis_id);
	vd_check_return_val_error(vd_bsp_spis == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spis[%d]", spis_id - VD_PORT_SPIS0);
	
	/* spis�� */
	vd_check_return_val_error(spi_drv.ops.spis_port.read == vd_null, -VD_BSP_ERR_SDK, "spis read port is null");
	ret = spi_drv.ops.spis_port.read(vd_bsp_spis, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform spis[%d] read error", spis_id - VD_PORT_SPIS0);	

	return ret;	
}
/**
 * @brief дSPIS����
 *
 * @param spis_id : SPIS����
 * @param data : SPIS�������ݻ���buff
 * @param data_len : SPIS�������ݳ���
 * @return vd_bool_t : д��ɹ���ʧ��
 */
vd_int32_t vd_bsp_spis_write(vd_bsp_port_e spis_id, vd_uint8_t* data, vd_uint32_t data_len) 
{
	vd_int32_t   ret = 0;
	vd_bsp_spis_t* vd_bsp_spis = vd_null;
	
	/* ����У�� */
	vd_check_return_val_error(spis_id < VD_PORT_SPIS0 || spis_id >= VD_PORT_SPIS_MAX, -VD_BSP_ERR_PORT_INVALID, "spis_id = %d, error", spis_id);
	
	/* ����spis�����б� */
	vd_bsp_spis = vd_bsp_spis_find(spis_id);
	vd_check_return_val_error(vd_bsp_spis == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spis[%d]", spis_id - VD_PORT_SPIS0);
	
	/* spisд */
	vd_check_return_val_error(spi_drv.ops.spis_port.write == vd_null, -VD_BSP_ERR_SDK, "spis write port is null");
	ret = spi_drv.ops.spis_port.write(vd_bsp_spis, data, data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform spis[%d] write error", spis_id - VD_PORT_SPIS0);	
	
	return ret;
}
/**
 * @brief spisͬ���շ����ݴ���ͨ��
 *
 * @param spis_id SPIS����
 * @param tx_data SPIS�������ݵ�buff
 * @param tx_data_len SPIS�������ݵ����ݳ���
 * @param rx_data SPIS�������ݵ����ݻ���buff
 * @param rx_data_len SPIS�������ݵĳ���
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_spis_transmit(
    vd_bsp_port_e  	spis_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    vd_uint8_t* rx_data,
    vd_uint32_t rx_data_len) 
{
			
	vd_int32_t   ret = 0;
	vd_bsp_spis_t* vd_bsp_spis = vd_null;
		
	/* ����У�� */
	vd_check_return_val_error(spis_id < VD_PORT_SPIS0 || spis_id >= VD_PORT_SPIS_MAX, -VD_BSP_ERR_PORT_INVALID, "spis_id = %d, error", spis_id);
	
	/* ����spis�����б� */
	vd_bsp_spis = vd_bsp_spis_find(spis_id);
	vd_check_return_val_error(vd_bsp_spis == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spis[%d]", spis_id - VD_PORT_SPIS0);
			
	/* spis��д */
	vd_check_return_val_error(spi_drv.ops.spis_port.transmit == vd_null, -VD_BSP_ERR_SDK, "spis transmit port is null");
	ret = spi_drv.ops.spis_port.transmit(vd_bsp_spis, tx_data, tx_data_len, rx_data, rx_data_len);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform spis[%d] transmit error", spis_id - VD_PORT_SPIS0);
	
	return ret;	
}
/**
 * @brief SPISlaveͨ��DMA��ʽд����
 *
 * @param spis_id SPIS������
 * @param tx_data SPIS�������ݵ�buff
 * @param tx_data_len SPIS�������ݵ����ݳ���
 * @param cb �ص�����ָ��
 * @param cb_param �ص�����
 * @return vd_bool_t
 */
vd_int32_t vd_bsp_spis_write_dma(
    vd_bsp_port_e  	spis_id,
    vd_uint8_t* tx_data,
    vd_uint32_t tx_data_len,
    void (*cb)(void*),
    void* cb_param) 
{
			
	vd_int32_t   ret = 0;
	vd_bsp_spis_t* vd_bsp_spis = vd_null;
		
	/* ����У�� */
	vd_check_return_val_error(spis_id < VD_PORT_SPIS0 || spis_id >= VD_PORT_SPIS_MAX, -VD_BSP_ERR_PORT_INVALID, "spis_id = %d, error", spis_id);
	
	/* ����spis�����б� */
	vd_bsp_spis = vd_bsp_spis_find(spis_id);
	vd_check_return_val_error(vd_bsp_spis == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "No spis[%d]", spis_id - VD_PORT_SPIS0);
			
	/* spis DMAд���� */
	vd_check_return_val_error(spi_drv.ops.spis_port.write_dma == vd_null, -VD_BSP_ERR_SDK, "spis write_dma port is null");
	ret = spi_drv.ops.spis_port.write_dma(vd_bsp_spis, tx_data, tx_data_len, cb, cb_param);
	vd_check_return_val_error(ret < 0, -VD_BSP_ERR_SDK, "platform spis[%d] write_dma error", spis_id - VD_PORT_SPIS0);
		
	return ret;		
}
