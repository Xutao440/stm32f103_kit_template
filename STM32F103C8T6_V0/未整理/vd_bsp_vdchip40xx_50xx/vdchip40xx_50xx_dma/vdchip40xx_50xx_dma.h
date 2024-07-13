#ifndef __VDCHIP40XX_50XX_DMA_H
#define __VDCHIP40XX_50XX_DMA_H

#include "base.h"

//DMA ID
typedef enum{
    DMA_ID_UART0_RX,
    DMA_ID_UART0_TX,
    DMA_ID_UART1_RX,
    DMA_ID_UART1_TX,
    DMA_ID_SPIM0_RX,
    DMA_ID_SPIM0_TX,
    DMA_ID_SPIM1_RX,
    DMA_ID_SPIM1_TX,
    DMA_ID_SPIS_RX,
    DMA_ID_SPIS_TX,
    DMA_ID_I2C0_RX,
    DMA_ID_I2C0_TX,
    DMA_ID_I2C1_RX,
    DMA_ID_I2C1_TX,
    DMA_ID_I2S_RX,
    DMA_ID_I2S_TX,
    DMA_ID_ADC,
    DMA_ID_PDM,
    DMA_ID_MAX,
}vd_fr8000_dma_id_e;

//DMA��ʼ������
typedef struct{
    vd_fr8000_dma_id_e  id;             //DMA ID
    vd_uint8_t          channel;        //ͨ��
    vd_uint8_t          request_id;     //����ID
    vd_uint8_t          src_width;      //Դ��ַ���ݿ��
    vd_uint8_t          dst_width;      //Ŀ���ַ���ݿ��
}vd_fr8000_dma_init_t;


vd_bool_t vdchip_40xx_50xx_dma_init(vd_fr8000_dma_id_e dma_id);
vd_bool_t vdchip_40xx_50xx_dma_start(vd_fr8000_dma_id_e dma_id, const vd_void_t *src_addr, const vd_void_t *dst_addr, vd_uint32_t data_len, vd_void_t (*cb)(vd_void_t *arg), vd_void_t *arg);


#endif
