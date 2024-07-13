/**
 * @file vdchip40xx_50xx_spi.c
 * @author liyongshaung
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
#include "vdchip40xx_50xx_spi/vdchip40xx_50xx_spi.h"
#include "driver_spi.h"
#include "driver_gpio.h"
#include "driver_system.h"
#include "driver_dma.h"
#include "vd_log.h"

/* ======================================================================================
 * types
 */

/* ����spi�������� */
typedef struct{
	SPI_HandleTypeDef		hspi;							/* spi��� */
	DMA_HandleTypeDef  	hdma;							/* dma��� */
	void (*tx_cplt_callback)(void*);			/* ������ɻص� */
	void* 							cb_param;					/* �ص����� */
}vdchip_40xx_50xx_spi_t;

vdchip_40xx_50xx_spi_t spi0 = {0};
vdchip_40xx_50xx_spi_t spi1 = {0};

/* ======================================================================================
 * declaration
 */
/*  ƽ̨�ӿ�  */
static vd_bool_t vdchip_40xx_50xx_spim_init(vd_bsp_spim_t *spim);/* spim��ʼ�� */
static vd_bool_t vdchip_40xx_50xx_spim_open(vd_bsp_spim_t *spim);/* spim�� */
static vd_bool_t vdchip_40xx_50xx_spim_close(vd_bsp_spim_t *spim);/* spim�ر� */
static vd_bool_t vdchip_40xx_50xx_spim_cs(vd_bsp_spim_t *spim, vd_uint8_t pin_level);/* spim���Ƭѡ���� */
static vd_int32_t vdchip_40xx_50xx_spim_read(vd_bsp_spim_t *spim, vd_uint8_t* data, vd_uint32_t data_len);/* spim�� */
static vd_int32_t vdchip_40xx_50xx_spim_write(vd_bsp_spim_t *spim, vd_uint8_t* data, vd_uint32_t data_len);/* spimд */
static vd_int32_t vdchip_40xx_50xx_spim_transmit(vd_bsp_spim_t *spim, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len);/* spim��д���� */
static vd_int32_t vdchip_40xx_50xx_spim_write_dma(vd_bsp_spim_t *spim, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, void (*cb)(void*), void* cb_param);/* spi dmaд */

static vd_bool_t vdchip_40xx_50xx_spis_init(vd_bsp_spis_t *spis);/* spis��ʼ�� */

/* ======================================================================================
 * helper
 */
/**
 * @brief ע��SPIƽ̨�ӿ�
 *
 * @param ops ƽ̨�ӿ�
 * @return vd_bool_t
 */
vd_bool_t vdchip_40xx_50xx_spi_register(spi_ops_t *ops)
{
	spim_port_t *spim_port = &ops->spim_port;
	spis_port_t *spis_port = &ops->spis_port;
	
	/* �ӿ�ע�� */
	spim_port->init = vdchip_40xx_50xx_spim_init;
	spim_port->open = vdchip_40xx_50xx_spim_open;
	spim_port->close = vdchip_40xx_50xx_spim_close;
	spim_port->cs = vdchip_40xx_50xx_spim_cs;
	spim_port->read = vdchip_40xx_50xx_spim_read;
	spim_port->write = vdchip_40xx_50xx_spim_write;
	spim_port->transmit = vdchip_40xx_50xx_spim_transmit;
	spim_port->write_dma = vdchip_40xx_50xx_spim_write_dma;
	
	spis_port->init = vdchip_40xx_50xx_spis_init;
	
	return vd_true;
}

/* ======================================================================================
 * private implementation
 */
/**
 * @brief ��ʼ��SPIM
 *
 * @param spim SPIM����
 * @return vd_bool_t
 */
static vd_bool_t vdchip_40xx_50xx_spim_init(vd_bsp_spim_t *spim)
{
		vd_bool_t   ret = vd_false;
		GPIO_InitTypeDef gpio_handle = {0};
		SPI_HandleTypeDef *hspix = vd_null;
		DMA_HandleTypeDef	*hdmax = vd_null;
	
		if(spim->id == VD_PORT_SPIM0) {
				hspix = &spi0.hspi;
				hdmax = &spi0.hdma;
				hspix->SPIx = SPIM0;
			__SYSTEM_SPI0_MASTER_CLK_SELECT_96M();																		//SPI0��ʱ��ѡ��96Mhz
			__SYSTEM_SPI0_MASTER_CLK_ENABLE();																				//ʹ��SPI0ʱ��

		}else if(spim->id == VD_PORT_SPIM1){
				hspix = &spi1.hspi;
        hspix->SPIx = SPIM1;
				__SYSTEM_SPI1_MASTER_CLK_SELECT_96M();																		//SPI1��ʱ��ѡ��96Mhz
				__SYSTEM_SPI1_MASTER_CLK_ENABLE();																				//ʹ��SPI1ʱ��
		}
		
		vd_check_return_val_error(hspix == vd_null, vd_false, "spim handle is null");
		vd_check_return_val_error((spim->data_bit - 1 < SPI_FRAME_SIZE_4BIT || spim->data_bit - 1 > SPI_FRAME_SIZE_16BIT), vd_false, "spim data bit error");
		
		/* =============================================  GPIO ���� ==================================================================== */
		__SYSTEM_GPIO_CLK_ENABLE();/* ��ʱ�� */
		
		/*Ƭѡ��������*/
			gpio_handle.Pin = 1 << (spim->cs_pin % 8);
			
			gpio_handle.Pull = GPIO_PULLUP;/* ���� */
		if(spim->cs_pin > 0){
			if(spim->cs_mode == PALTFORM_SPI_CS_MODE_HARDWARE){/* Ӳ��Ƭѡ */
				gpio_handle.Mode = GPIO_MODE_AF_PP;/* �������� */
				gpio_handle.Alternate = GPIO_FUNCTION_2;
//				system_set_port_mux(spim->cs_pin / 8, spim->cs_pin % 8, 0x02);//PORTA7_FUNC_SSIM0_CSN);
			}else{/* ���Ƭѡ */
				gpio_handle.Mode = GPIO_MODE_OUTPUT_PP;/* ���� */
			}
			gpio_init(spim->cs_pin / 8, &gpio_handle);
			gpio_write_pin(spim->cs_pin / 8, 1 << (spim->cs_pin % 8), 1);/* ��ѡ�� */	
		}
		
		/* ʱ�ӡ������������� */
		if(spim->sclk_pin < 0 || spim->mosi_pin < 0 || spim->miso_pin < 0){
			log_debug("spim_sclk_pin %d ,spim_mosi_pin %d ,spim_miso_pin %d error!!!!!!!!!\r\n", spim->sclk_pin, spim->mosi_pin, spim->miso_pin);	
			return vd_false;
		}
		
		gpio_handle.Pin = 1 << (spim->sclk_pin % 8);
		gpio_handle.Mode = GPIO_MODE_AF_PP;/* �������� */
		gpio_handle.Alternate = GPIO_FUNCTION_2;
		gpio_init(spim->sclk_pin / 8, &gpio_handle);
//		system_set_port_mux(spim->sclk_pin / 8, spim->sclk_pin % 8, 0x02);//PORTA6_FUNC_SSIM0_CLK);	
		log_debug("spim_sclk_pin %d init ok\r\n", spim->sclk_pin);	
		
		gpio_handle.Pin = 1 << (spim->mosi_pin % 8);
		gpio_init(spim->mosi_pin / 8, &gpio_handle);
//		system_set_port_mux(spim->mosi_pin / 8, spim->mosi_pin % 8, 0x02);//PORTA2_FUNC_SSIM0_IO0);
		log_debug("spim_mosi_pin %d init ok\r\n", spim->mosi_pin);	
		
		gpio_handle.Pin = 1 << (spim->miso_pin % 8);
		gpio_init(spim->miso_pin / 8, &gpio_handle);
//		system_set_port_mux(spim->miso_pin / 8, spim->miso_pin % 8, 0x02);//PORTA3_FUNC_SSIM0_IO1);
		log_debug("spim_miso_pin %d init ok\r\n", spim->miso_pin);	
		
		/* ============================================= SPI ���� ==================================================================== */
		hspix->Init.Work_Mode             = spim->mode;
		hspix->Init.Frame_Size            = spim->data_bit - 1;
		hspix->Init.BaudRate_Prescaler    = 96000000 / spim->speed;	
		hspix->Init.TxFIFOEmpty_Threshold = 20;
		hspix->Init.RxFIFOFull_Threshold  = 0;
		spi_master_init(hspix);
		
		if(spim->use_dma && hdmax != vd_null){/* ʹ��DMA */
//			system_regs->dma_req[0] |= 1 << 20;
			if(hdmax == &spi0.hdma){
				__DMA_REQ_ID_SPI0_MASTER_TX(1);//DMA�������
			}else{
				__DMA_REQ_ID_SPI1_MASTER_TX(1);//DMA�������
			}
				
			hdmax->Channel               = spim->dma_channel;
			hdmax->Init.Data_Flow        = DMA_M2P_DMAC;
			hdmax->Init.Request_ID       = 1;
			hdmax->Init.Source_Inc       = DMA_ADDR_INC_INC;
			hdmax->Init.Desination_Inc   = DMA_ADDR_INC_NO_CHANGE;
			hdmax->Init.Source_Width     = DMA_TRANSFER_WIDTH_8;
			hdmax->Init.Desination_Width = DMA_TRANSFER_WIDTH_8;
			dma_init(hdmax);
			NVIC_EnableIRQ(DMA_IRQn);
		}

		return vd_true;
}

/**
 * @brief ��SPIM����
 *
 * @param spim SPIM����
 * @return vd_bool_t ���ش򿪽��
 */
static vd_bool_t vdchip_40xx_50xx_spim_open(vd_bsp_spim_t *spim) 
{
//	__SPI_ENABLE(__SPIx__);
	
	return vd_false;
}

/**
 * @brief �ر�SPIM����
 *
 * @param spim SPIM����
 * @return vd_bool_t ���عرս��
 */
static vd_bool_t vdchip_40xx_50xx_spim_close(vd_bsp_spim_t *spim) 
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
static vd_bool_t vdchip_40xx_50xx_spim_cs(vd_bsp_spim_t *spim, vd_uint8_t pin_level)
{
	gpio_write_pin(spim->cs_pin / 8, 1 << (spim->cs_pin % 8), pin_level ? GPIO_PIN_SET : GPIO_PIN_CLEAR);
	return vd_true;
}

/**
 * @brief SPIM��ȡ
 *
 * @param spim SPIM����
 * @param data ��ȡ���ݻ���buff
 * @param data_len ��Ҫ��ȡ�����ݳ���
 * @return vd_uint16_t ����ʵ�ʶ�ȡ�����ݳ���
 */
static vd_int32_t vdchip_40xx_50xx_spim_read(vd_bsp_spim_t *spim, vd_uint8_t* data, vd_uint32_t data_len) 
{
	vdchip_40xx_50xx_spi_t *spix = vd_null;
	
	if(spim->id == VD_PORT_SPIM0){
		spix = &spi0;		
	}else if(spim->id == VD_PORT_SPIM1){
		spix = &spi1;		
	}
	
	vd_check_return_val_error(spix == vd_null, -1, "spim read data error");

	spi_master_receive_X1(&spix->hspi, (uint16_t *)data, data_len);
		
	return data_len;
}

/**
 * @brief SPIMд����
 *
 * @param spim SPIM����
 * @param data д�����ݻ���buff
 * @param data_len ��Ҫд������ݳ���
 * @return vd_bool_t д��ɹ���ʧ��
 */
static vd_int32_t vdchip_40xx_50xx_spim_write(vd_bsp_spim_t *spim, vd_uint8_t* data, vd_uint32_t data_len)
{
	vdchip_40xx_50xx_spi_t *spix = vd_null;
	
	if(spim->id == VD_PORT_SPIM0){
		spix = &spi0;		
	}else if(spim->id == VD_PORT_SPIM1){
		spix = &spi1;		
	}
	
	vd_check_return_val_error(spix == vd_null, -1, "spim write data error");

	spi_master_transmit_X1(&spix->hspi, (uint16_t *)data, data_len);
		
	return data_len;
}

/**
 * @brief spimͬ���շ����ݴ���ͨ��
 *
 * @param spim 				SPIM����
 * @param tx_data 		SPIM�������ݵ�buff
 * @param tx_data_len SPIM�������ݵ����ݳ���
 * @param rx_data 		SPIM�������ݵ����ݻ���buff
 * @param rx_data_len SPIM�������ݵĳ���
 * @return vd_bool_t	�ɹ���ʧ��
 */
static vd_int32_t vdchip_40xx_50xx_spim_transmit(vd_bsp_spim_t *spim, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, vd_uint8_t* rx_data, vd_uint32_t rx_data_len)
{
	return 0;
}

/**
 * @brief spimͨ��DMA��ʽд����
 *
 * @param spim 				SPIM����
 * @param tx_data SPIM�������ݵ�buff
 * @param tx_data_len SPIM�������ݵ����ݳ���
 * @param cb �ص�����ָ��
 * @param cb_param �ص�����
 * @return vd_bool_t
 */
static vd_int32_t vdchip_40xx_50xx_spim_write_dma(vd_bsp_spim_t *spim, vd_uint8_t* tx_data, vd_uint32_t tx_data_len, void (*cb)(void*), void* cb_param) 
{
	vdchip_40xx_50xx_spi_t *spix = vd_null;
	
	if(spim->id == VD_PORT_SPIM0){
		spix = &spi0;
	}
	
	vd_check_return_val_error(spix == vd_null, -1, "spim_write_dma error");
	
	spi_master_transmit_X1_DMA(&spix->hspi);
		
	dma_start_IT(&spix->hdma, (uint32_t)tx_data, (uint32_t)&spix->hspi.SPIx->DR, tx_data_len, DMA_BURST_LEN_256);
	
	spix->tx_cplt_callback = cb;
	spix->cb_param = cb_param;
	
	return tx_data_len;
}

/*============= SPI Slave =================*/
/**
 * @brief ��ʼ��SPIS
 *
 * @param spis SPIS����
 * @return vd_bool_t
 */
static vd_bool_t vdchip_40xx_50xx_spis_init(vd_bsp_spis_t *spis)
{
	SPI_HandleTypeDef *hspix = vd_null;
	DMA_HandleTypeDef *hdmax = vd_null;
	GPIO_InitTypeDef gpio_handle ={0};
	
	if(spis->id == VD_PORT_SPIS0) {
		hspix = &spi0.hspi;
		hdmax = &spi0.hdma;
		hspix->SPIx = SPIS0;
		__SYSTEM_SPI_SLAVE_CLK_SELECT_96M();
		__SYSTEM_SPI_SLAVE_CLK_ENABLE();
	}else{
		return vd_false;
	}
	
	/* ��ʼ�� */
	vd_check_return_val_error(hspix == vd_null, vd_false, "spis handle is null");
	vd_check_return_val_error((spis->data_bit - 1 < SPI_FRAME_SIZE_4BIT || spis->data_bit - 1 > SPI_FRAME_SIZE_16BIT), vd_false, "spis data bit error");
	
	hspix->Init.Work_Mode             = spis->mode;
	hspix->Init.Frame_Size            = spis->data_bit - 1;
	hspix->Init.BaudRate_Prescaler    = 96000000 / spis->speed;															
	hspix->Init.TxFIFOEmpty_Threshold = 20;
	hspix->Init.RxFIFOFull_Threshold  = 0;
	spi_slave_init(hspix);

	if(spis->use_dma && hdmax != vd_null){/* ʹ��DMA */
//		system_regs->dma_req[0] |= 1 << 20;
		__DMA_REQ_ID_SPI_SLAVE_TX(1);//DMA�������

		
		hdmax->Channel               = spis->dma_channel;
		hdmax->Init.Data_Flow        = DMA_M2P_DMAC;
		hdmax->Init.Request_ID       = 1;
		hdmax->Init.Source_Inc       = DMA_ADDR_INC_INC;
		hdmax->Init.Desination_Inc   = DMA_ADDR_INC_NO_CHANGE;
		hdmax->Init.Source_Width     = DMA_TRANSFER_WIDTH_8;
		hdmax->Init.Desination_Width = DMA_TRANSFER_WIDTH_8;
		dma_init(hdmax);
		NVIC_EnableIRQ(DMA_IRQn);
	}

	/* ================  GPIO ���� =============== */
	__SYSTEM_GPIO_CLK_ENABLE();/* ��ʱ�� */
	
	/*Ƭѡ��������*/
	if(spis->cs_pin > 0){
		system_set_port_mux(spis->cs_pin / 8, spis->cs_pin % 8, 0x03);//PORTA5_FUNC_SSIS_CSN);
	}
	
	/* ������������ */
	if(spis->sclk_pin < 0 || spis->mosi_pin < 0 || spis->miso_pin < 0){
		log_debug("spis_sclk_pin %d ,spis_mosi_pin %d ,spis_miso_pin %d error!!!!!!!!!\r\n", spis->sclk_pin, spis->mosi_pin, spis->miso_pin);	
		return vd_false;
	}
	system_set_port_mux(spis->sclk_pin / 8, spis->sclk_pin % 8, 0x03);//PORTA6_FUNC_SSIM0_CLK);	
	log_debug("spis_sclk_pin %d init ok\r\n", spis->sclk_pin);	
	system_set_port_mux(spis->mosi_pin / 8, spis->mosi_pin % 8, 0x03);//PORTA2_FUNC_SSIM0_IO0);
	log_debug("spis_mosi_pin %d init ok\r\n", spis->mosi_pin);	
	system_set_port_mux(spis->miso_pin / 8, spis->miso_pin % 8, 0x03);//PORTA3_FUNC_SSIM0_IO1);
	log_debug("spis_miso_pin %d init ok\r\n", spis->miso_pin);	
	
	return vd_true;
}





/*============================================================= �жϷ����� ==================================================================================*/
/**
 * @brief DMA�жϷ�����
 *
 * @param vd_void_t 			
 * @return vd_void_t
 */
//__attribute__((section("ram_code"))) void dma_isr(void){
//	if(dma_get_tfr_Status(spi0.hdma.Channel)){
//    while(__SPI_IS_BUSY(SPIM0));

//    // CS Release
//	
//    /* Clear Transfer complete status */
//    dma_clear_tfr_Status(spi0.hdma.Channel);
//    /* channel Transfer complete interrupt disable */
//    dma_tfr_interrupt_disable(spi0.hdma.Channel);
//		
//    __SPI_DISABLE(spi0.hspi.SPIx);
//    __SPI_DATA_FRAME_SIZE(spi0.hspi.SPIx, SPI_FRAME_SIZE_8BIT);
//    
////    dma_transfer_done = true;
//		vd_bsp_gpio_write(PLATFORM_SPIM0_CS_PIN, 1);
//		
//		if(spi0.tx_cplt_callback){/* ��Ҫ�ص� */
//			spi0.tx_cplt_callback(spi0.cb_param);
//			spi0.tx_cplt_callback = vd_null;
//			spi0.cb_param = vd_null;
//		}
//	}
//}

