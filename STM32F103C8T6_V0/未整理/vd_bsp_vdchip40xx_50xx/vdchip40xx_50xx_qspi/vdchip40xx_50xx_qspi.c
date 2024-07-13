/**
 * @file vdchip40xx_50xx_qspi.c
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
#include "vdchip40xx_50xx_qspi/vdchip40xx_50xx_qspi.h"
#include "driver_gpio.h"
#include "driver_system.h"
#include "driver_spi.h"
#include "vdchip40xx_50xx_dma.h"
#include "vd_log.h"

/* ======================================================================================
 * types
 */
/* ����qspi�������� */
typedef struct{
	SPI_HandleTypeDef		hqspi;						/* qspi��� */
	void (*tx_cplt_callback)(void*);			/* ������ɻص� */
	void* 							cb_param;					/* �ص����� */
}vdchip_40xx_50xx_qspi_t;

vdchip_40xx_50xx_qspi_t qspi0 = {0};
vdchip_40xx_50xx_qspi_t qspi1 = {0};

/* ======================================================================================
 * declaration
 */
/*  ƽ̨�ӿ�  */
static vd_bool_t vdchip_40xx_50xx_qspi_init(vd_bsp_qspi_t *qspi);/* qspi��ʼ�� */
static vd_bool_t vdchip_40xx_50xx_qspi_open(vd_bsp_qspi_t *qspi);/* qspi�� */
static vd_bool_t vdchip_40xx_50xx_qspi_close(vd_bsp_qspi_t *qspi);/* qspi�ر� */
static vd_bool_t vdchip_40xx_50xx_qspi_cs(vd_bsp_qspi_t *qspi, vd_uint8_t pin_level);/* qspi���Ƭѡ */
static vd_int32_t vdchip_40xx_50xx_qspi_read(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint32_t data_len);/* qspi�� */
static vd_int32_t vdchip_40xx_50xx_qspi_write(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint32_t data_len);/* qspiд */
static vd_int32_t vdchip_40xx_50xx_qspi_transmit(vd_bsp_qspi_t *qspi, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len);/* qspi��д���� */
static vd_int32_t vdchip_40xx_50xx_qspi_write_dma(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, const vd_uint8_t* tx_data, vd_uint32_t tx_data_len, void (*cb)(void*), void* cb_param);/* qspi dmaд */

/* ======================================================================================
 * helper
 */
/**
 * @brief ע��QSPIƽ̨�ӿ�
 *
 * @param ops ƽ̨�ӿ�
 * @return vd_bool_t
 */
vd_bool_t vdchip_40xx_50xx_qspi_register(qspi_ops_t *ops)
{
	/* �ӿ�ע�� */
	ops->init = vdchip_40xx_50xx_qspi_init;
	ops->open = vdchip_40xx_50xx_qspi_open;
	ops->close = vdchip_40xx_50xx_qspi_close;
	ops->cs = vdchip_40xx_50xx_qspi_cs;
	ops->read = vdchip_40xx_50xx_qspi_read;
	ops->write = vdchip_40xx_50xx_qspi_write;
	ops->transmit = vdchip_40xx_50xx_qspi_transmit;
	ops->write_dma = vdchip_40xx_50xx_qspi_write_dma;
	
	return vd_true;
}

/* ======================================================================================
 * implementation
 */

/**
 * @brief ��ʼ��QSPIM
 *
 * @param qspi QSPIM����
 * @return vd_bool_t
 */
static vd_bool_t vdchip_40xx_50xx_qspi_init(vd_bsp_qspi_t *qspi)
{
		vd_bool_t   ret = vd_false;
		GPIO_InitTypeDef gpio_handle = {0};
		SPI_HandleTypeDef *hqspix = vd_null;
		
		log_debug(">>>>>>>>>>>>>>>>>>>>>>>>>>> %s >>>>>>>>>>>>>>\r\n", __func__);
	
		if(qspi->id == VD_PORT_QSPIM0) {
			log_debug("start init qspi[0]\r\n");
			hqspix = &qspi0.hqspi;
			hqspix->SPIx = SPIM0;
			__SYSTEM_SPI0_MASTER_CLK_SELECT_96M();																		//SPI0��ʱ��ѡ��96Mhz
			__SYSTEM_SPI0_MASTER_CLK_ENABLE();																				//ʹ��SPI0ʱ��
		}else if(qspi->id == VD_PORT_QSPIM1){
			log_debug("start init qspi[1]\r\n");
			hqspix = &qspi1.hqspi;
			hqspix->SPIx = SPIM1;
			__SYSTEM_SPI1_MASTER_CLK_SELECT_96M();																		//SPI1��ʱ��ѡ��96Mhz
			__SYSTEM_SPI1_MASTER_CLK_ENABLE();																				//ʹ��SPI1ʱ��
		}
		
		vd_check_return_val_error(hqspix == vd_null, vd_false, "qspi handle is null");
		vd_check_return_val_error((qspi->data_bit - 1 < SPI_FRAME_SIZE_4BIT || qspi->data_bit - 1 > SPI_FRAME_SIZE_16BIT), vd_false, "qspi data bit error");
			
/* =============================================  GPIO ���� ==================================================================== */	
		__SYSTEM_GPIO_CLK_ENABLE();/* ��ʱ�� */
		
		gpio_handle.Pull = GPIO_PULLUP;/* ���� */
		/*Ƭѡ��������*/	
		log_debug("cs_pin = %d, cs_mode = %d\r\n", qspi->cs_pin, qspi->cs_mode);
		
		if(qspi->cs_pin >= 0){
			gpio_handle.Pin = 1 << (qspi->cs_pin % 8);
			
			if(qspi->cs_mode == PALTFORM_QSPI_CS_MODE_HARDWARE){/* Ӳ��Ƭѡ */
				gpio_handle.Mode = GPIO_MODE_AF_PP;/* �������� */
				gpio_handle.Alternate = GPIO_FUNCTION_2;
//				system_set_port_mux(spim->cs_pin / 8, spim->cs_pin % 8, 0x02);//PORTA7_FUNC_SSIM0_CSN);
			}else{/* ���Ƭѡ */
				gpio_handle.Mode = GPIO_MODE_OUTPUT_PP;/* ���� */
			}
			gpio_init(qspi->cs_pin / 8, &gpio_handle);
			gpio_write_pin(qspi->cs_pin / 8, 1 << (qspi->cs_pin % 8), 1);/* ��ѡ�� */	
		}
		
		/* ʱ�ӡ������������� */
		if(qspi->sclk_pin < 0 || qspi->io0_pin < 0 || qspi->io1_pin < 0 || qspi->io2_pin < 0 || qspi->io3_pin < 0){
			log_error("qspi pin error!!!!!!!!!:\r\n qspi_sclk_pin %d\r\n qspi_io0_pin %d\r\n qspi_io1_pin %d\r\n qspi_io1_pin %d\r\n qspi_io2_pin %d\r\n qspi_io3_pin %d\r\n"\
				, qspi->sclk_pin, qspi->io0_pin, qspi->io1_pin, qspi->io2_pin, qspi->io3_pin);	
			return vd_false;
		}
		//CLK
//		gpio_handle.Pin = 1 << (qspi->sclk_pin % 8);
//		gpio_handle.Mode = GPIO_MODE_AF_PP;/* �������� */
//		gpio_handle.Alternate = GPIO_FUNCTION_2;
//		gpio_init(qspi->sclk_pin / 8, &gpio_handle);
		system_set_port_mux(qspi->sclk_pin / 8, qspi->sclk_pin % 8, 0x02);//PORTA6_FUNC_SSIM0_CLK);	
		log_debug("qspi_sclk_pin %d init ok\r\n", qspi->sclk_pin);	
		
		//IO0
//		gpio_handle.Pin = 1 << (qspi->io0_pin % 8);
//		gpio_init(qspi->io0_pin / 8, &gpio_handle);
		system_set_port_mux(qspi->io0_pin / 8, qspi->io0_pin % 8, 0x02);//PORTA2_FUNC_SSIM0_IO0);
		log_debug("qspi_io0_pin %d init ok\r\n", qspi->io0_pin);	
		
		//IO1
//		gpio_handle.Pin = 1 << (qspi->io1_pin % 8);
//		gpio_init(qspi->io1_pin / 8, &gpio_handle);
		system_set_port_mux(qspi->io1_pin / 8, qspi->io1_pin % 8, 0x02);//PORTA3_FUNC_SSIM0_IO1);
		log_debug("qspi_io1_pin %d init ok\r\n", qspi->io1_pin);
		
		//IO2
//		gpio_handle.Pin = 1 << (qspi->io2_pin % 8);
//		gpio_init(qspi->io2_pin / 8, &gpio_handle);
		system_set_port_mux(qspi->io2_pin / 8, qspi->io2_pin % 8, 0x02);//PORTA4_FUNC_SSIM0_IO2);
		log_debug("qspi_io2_pin %d init ok\r\n", qspi->io2_pin);	
		
		//IO3
//		gpio_handle.Pin = 1 << (qspi->io3_pin % 8);
//		gpio_init(qspi->io3_pin / 8, &gpio_handle);
		system_set_port_mux(qspi->io3_pin / 8, qspi->io3_pin % 8, 0x02);//PORTA5_FUNC_SSIM0_IO3);
		log_debug("qspi_io3_pin %d init ok\r\n", qspi->io3_pin);	
		
		/* ============================================= QSPI ���� ==================================================================== */
		log_debug("qspi_mode = %d, data_bit = %d, speed = %d\r\n", qspi->mode, qspi->data_bit, qspi->speed);
		
		hqspix->Init.Work_Mode             = qspi->mode;
		hqspix->Init.Frame_Size            = qspi->data_bit - 1;
		hqspix->Init.BaudRate_Prescaler    = 96000000 / qspi->speed;															
		hqspix->Init.TxFIFOEmpty_Threshold = 20;
		hqspix->Init.RxFIFOFull_Threshold  = 0;
		spi_master_init(hqspix);

		if(qspi->use_dma){
			ret = vdchip_40xx_50xx_dma_init(qspi->id == VD_PORT_QSPIM0 ? DMA_ID_SPIM0_TX : DMA_ID_SPIM1_TX);
			vd_check_return_val_error(ret == vd_false, vd_false, "func:%s, line:%d, qspi dma init error\r\n", __func__, __LINE__);
		}
	
		log_debug(">>>>>>>>>>>>>>>>>>>>>>>>>>> %s END >>>>>>>>>>>>>>\r\n", __func__);
	
		return vd_true;
}

/**
 * @brief ��QSPIM����
 *
 * @param qspi QSPIM����
 * @return vd_bool_t ���ش򿪽��
 */
static vd_bool_t vdchip_40xx_50xx_qspi_open(vd_bsp_qspi_t *qspi) 
{
//	__SPI_ENABLE(__SPIx__);
	
	return vd_false;
}

/**
 * @brief �ر�QSPIM����
 *
 * @param qspi QSPIM����
 * @return vd_bool_t ���عرս��
 */
static vd_bool_t vdchip_40xx_50xx_qspi_close(vd_bsp_qspi_t *qspi) 
{
//	__SPI_DISABLE(__SPIx__);
	
	return vd_false;
}

/**
 * @brief QSPI���Ƭѡ����
 *
 * @param qspi 				QSPIM����
 * @param pin_level 	���ŵ�ƽ
 * @return vd_bool_t 	���ؿ��ƽ��
 */
static vd_bool_t vdchip_40xx_50xx_qspi_cs(vd_bsp_qspi_t *qspi, vd_uint8_t pin_level)
{
	gpio_write_pin(qspi->cs_pin / 8, 1 << (qspi->cs_pin % 8), pin_level ? GPIO_PIN_SET : GPIO_PIN_CLEAR);
	return vd_true;
}

/**
 * @brief QSPIM��ȡ
 *
 * @param cfg 			��д����
 * @param qspi QSPIM����
 * @param data ��ȡ���ݻ���buff
 * @param data_len ��Ҫ��ȡ�����ݳ���
 * @return vd_uint16_t ����ʵ�ʶ�ȡ�����ݳ���
 */
static vd_int32_t vdchip_40xx_50xx_qspi_read(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint32_t data_len) 
{
	vdchip_40xx_50xx_qspi_t *qspix = vd_null;
	
	if(qspi->id == VD_PORT_QSPIM0){
		qspix = &qspi0;		
	}else if(qspi->id == VD_PORT_QSPIM1){
		qspix = &qspi1;		
	}
	
	vd_check_return_val_error(qspix == vd_null, -1, "qspi read error");
	vd_check_return_val_error(cfg == vd_null, -1, "qspi read config error");
	vd_check_return_val_error(cfg->instruct_len > 2, -1, "qspi read config error");
	vd_check_return_val_error(cfg->address_len > 4, -1, "qspi read config error");
	
	qspix->hqspi.MultWireParam.Wire_X2X4      = Wire_X4;
	qspix->hqspi.MultWireParam.InstructLength = cfg->instruct_len ? (cfg->instruct_len + 1) : INST_0BIT;
	qspix->hqspi.MultWireParam.Instruct       = (vd_uint16_t)cfg->instruct;
	qspix->hqspi.MultWireParam.AddressLength  = cfg->address_len << 1;
	qspix->hqspi.MultWireParam.Address        = cfg->address;		

	spi_master_receive_X2X4(&qspix->hqspi, (uint16_t *)data, data_len);		
	
	return data_len;
}

/**
 * @brief QSPIMд����
 *
 * @param qspi 					QSPIM����
 * @param cfg 					��д����
 * @param data 					д�����ݻ���buff
 * @param data_len 			��Ҫд������ݳ���
 * @return vd_bool_t 		д��ɹ���ʧ��
 */
static vd_int32_t vdchip_40xx_50xx_qspi_write(vd_bsp_qspi_t *qspi, vd_qspi_tx_cfg_t *cfg, vd_uint8_t* data, vd_uint32_t data_len)
{
	vdchip_40xx_50xx_qspi_t *qspix = vd_null;
	
	if(qspi->id == VD_PORT_QSPIM0){
//		log_debug("qspi[0] tx, len = %d\r\n", data_len);
		qspix = &qspi0;		
	}else if(qspi->id == VD_PORT_QSPIM1){
//		log_debug("qspi[1] tx, len = %d\r\n", data_len);
		qspix = &qspi1;		
	}
	
//	log_debug("line = %d\r\n", cfg->line);
//	log_debug("instruct_len = %d\r\n", cfg->instruct_len);
//	log_debug("instruct = 0x%x\r\n", cfg->instruct);
//	log_debug("address_len = %d\r\n", cfg->address_len);
//	log_debug("address = 0x%x\r\n", cfg->address);
	
	if(cfg->line == QSPI_LINE_X1){
		spi_master_transmit_X1(&qspix->hqspi, (uint16_t *)data, data_len);		
	}else{
		vd_check_return_val_error(qspix == vd_null, -1, "qspi write error\r\n");
		vd_check_return_val_error(cfg == vd_null, -1, "qspi write config error\r\n");
		vd_check_return_val_error(cfg->instruct_len > 2, -1, "qspi write config error, cfg->instruct_len = %d\r\n", cfg->instruct_len);
		vd_check_return_val_error(cfg->address_len > 4, -1, "qspi write config error, cfg->address_len = %d\r\n", cfg->address_len);
		
		qspix->hqspi.MultWireParam.Wire_X2X4      = cfg->line - 1;
		qspix->hqspi.MultWireParam.InstructLength = cfg->instruct_len ? (cfg->instruct_len + 1) : INST_0BIT;
		qspix->hqspi.MultWireParam.Instruct       = (vd_uint16_t)cfg->instruct;
		qspix->hqspi.MultWireParam.AddressLength  = cfg->address_len << 1;
		qspix->hqspi.MultWireParam.Address        = cfg->address;			

		spi_master_transmit_X2X4(&qspix->hqspi, (uint16_t *)data, data_len);		
	}
	
	return data_len;
}

/**
 * @brief qspiͬ���շ����ݴ���ͨ��
 *
 * @param qspi 				QSPIM����
 * @param tx_data 		QSPIM�������ݵ�buff
 * @param tx_data_len QSPIM�������ݵ����ݳ���
 * @param rx_data 		QSPIM�������ݵ����ݻ���buff
 * @param rx_data_len QSPIM�������ݵĳ���
 * @return vd_bool_t	�ɹ���ʧ��
 */
static vd_int32_t vdchip_40xx_50xx_qspi_transmit(vd_bsp_qspi_t *qspi, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len)
{
	return 0;
}

/**
 * @brief qspi dma ������ɻص�
 * 
 * @param arg 
 * @return ** vd_void_t 
 */
static vd_void_t vdchip_40xx_50xx_qspi_tx_cplt_callback(vd_void_t *arg)
{
	vd_bsp_qspi_t *qspi = (vd_bsp_qspi_t *)arg;

	if(qspi->id == VD_PORT_QSPIM0 && qspi0.tx_cplt_callback){
		while(__SPI_IS_BUSY(SPIM0));
		__SPI_DISABLE(qspi0.hqspi.SPIx);
		qspi0.tx_cplt_callback(qspi0.cb_param);
	}else if(qspi->id == VD_PORT_QSPIM1 && qspi1.tx_cplt_callback){
		while(__SPI_IS_BUSY(SPIM1));
		__SPI_DISABLE(qspi1.hqspi.SPIx);
		qspi1.tx_cplt_callback(qspi1.cb_param);	
	}
}

/**
 * @brief qspiͨ��DMA��ʽд����
 *
 * @param qspi 					QSPIM����
 * @param cfg 					��д����
 * @param tx_data 			QSPIM�������ݵ�buff
 * @param tx_data_len 	QSPIM�������ݵ����ݳ���
 * @param cb 						�ص�����ָ��
 * @param cb_param 			�ص�����
 * @return vd_bool_t
 */
static vd_int32_t vdchip_40xx_50xx_qspi_write_dma(	vd_bsp_qspi_t *qspi, 
													vd_qspi_tx_cfg_t *cfg,
													const vd_uint8_t* tx_data, 
													vd_uint32_t tx_data_len, 
													void (*cb)(void*), 
													void* cb_param) 
{
	vd_bool_t ret;
	vdchip_40xx_50xx_qspi_t *qspix = vd_null;
	
	if(qspi->id == VD_PORT_QSPIM0){
		qspix = &qspi0;		
//		log_debug("qspi[0] dma tx, len = %d\r\n", tx_data_len);
	}else if(qspi->id == VD_PORT_QSPIM1){
		qspix = &qspi1;	
//		log_debug("qspi[1] dma tx, len = %d\r\n", tx_data_len);	
	}
	
	vd_check_return_val_error(qspix == vd_null, -1, "qspi write_dma error");
	
//	log_debug("line = %d\r\n", cfg->line);
//	log_debug("instruct_len = %d\r\n", cfg->instruct_len);
//	log_debug("instruct = 0x%x\r\n", cfg->instruct);
//	log_debug("address_len = %d\r\n", cfg->address_len);
//	log_debug("address = 0x%x\r\n", cfg->address);
	
	if(cfg->line == QSPI_LINE_X1){
		spi_master_transmit_X1_DMA(&qspix->hqspi);	
	}else{
		vd_check_return_val_error(cfg == vd_null, -1, "qspi write config error");
		vd_check_return_val_error(cfg->instruct_len > 2, -1, "qspi write config error");
		vd_check_return_val_error(cfg->address_len > 4, -1, "qspi write config error");
		
		qspix->hqspi.MultWireParam.Wire_X2X4      = cfg->line - 1;
		qspix->hqspi.MultWireParam.InstructLength = cfg->instruct_len ? (cfg->instruct_len + 1) : INST_0BIT;
		qspix->hqspi.MultWireParam.Instruct       = (vd_uint16_t)cfg->instruct;
		qspix->hqspi.MultWireParam.AddressLength  = cfg->address_len << 1;
		qspix->hqspi.MultWireParam.Address        = cfg->address;				
		
		spi_master_transmit_X2X4_DMA(&qspix->hqspi);
	}
	
	qspix->tx_cplt_callback = cb;
	qspix->cb_param = cb_param;
	
	// dma_start_IT(&qspix->hdma, (uint32_t)tx_data, (uint32_t)&qspix->hqspi.SPIx->DR, tx_data_len, DMA_BURST_LEN_256);
	ret = vdchip_40xx_50xx_dma_start(qspi->id == VD_PORT_QSPIM0 ? DMA_ID_SPIM0_TX : DMA_ID_SPIM1_TX, tx_data, (vd_void_t *)&qspix->hqspi.SPIx->DR, tx_data_len, vdchip_40xx_50xx_qspi_tx_cplt_callback, qspi);
	vd_check_return_val_error(ret == vd_false, -1, "func:%s, line:%d, dma start tx error\r\n", __func__, __LINE__);
	
	return tx_data_len;
}

