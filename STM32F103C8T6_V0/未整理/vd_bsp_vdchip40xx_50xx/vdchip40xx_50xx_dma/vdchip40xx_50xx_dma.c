#include "vdchip40xx_50xx_dma.h"
#include "platform_config.h"
#include "driver_system.h"
#include "driver_dma.h"
#include "vd_log.h"

typedef struct {
    vd_fr8000_dma_init_t    init;
    DMA_HandleTypeDef  	    handle;							/* dma句柄 */   
    vd_void_t (*cb)(vd_void_t *arg);
    vd_void_t               *arg;
    vd_uint8_t              is_init : 1;    //ê?·?3?ê??ˉ
    vd_uint8_t              reserve : 7;
}vd_fr8000_dma_t;

vd_fr8000_dma_t fr8000_dma[] = PLATFORM_DMA_INIT_TABLE;




vd_fr8000_dma_t *vdchip_40xx_50xx_find_dma(vd_fr8000_dma_id_e dma_id)
{
    vd_uint8_t i;

    for (i = 0; i < vd_array_size(fr8000_dma); i++){
        if (dma_id == fr8000_dma[i].init.id){
            return &fr8000_dma[i];
        }
    }
    
    return vd_null;
}

/**
 * @brief dma初始化
 * 
 * @return ** vd_bool_t 
 */
vd_bool_t vdchip_40xx_50xx_dma_init(vd_fr8000_dma_id_e dma_id)
{
    vd_fr8000_dma_t *dma_obj = vd_null;

    vd_check_return_val_error(dma_id >= DMA_ID_MAX, vd_false, "func:%s, line:%d, dma_id error\r\n", __func__, __LINE__);
			
    dma_obj = vdchip_40xx_50xx_find_dma(dma_id);
    vd_check_return_val_error(dma_obj == vd_null, vd_false, "func:%s, line:%d, dma_id invalid\r\n", __func__, __LINE__);
    vd_check_return_val_error(dma_obj->init.channel > 2, vd_false, "func:%s, line:%d, dma channel error\r\n", __func__, __LINE__);
    vd_check_return_val_error(dma_obj->init.request_id == 0 || dma_obj->init.request_id > 15, vd_false, "func:%s, line:%d, dma request_id error\r\n", __func__, __LINE__);

    __SYSTEM_DMA_CLK_ENABLE();

    switch (dma_obj->init.id){
        case DMA_ID_UART0_RX: __DMA_REQ_ID_UART0_RX(dma_obj->init.request_id); break;
        case DMA_ID_UART0_TX: __DMA_REQ_ID_UART0_TX(dma_obj->init.request_id); break;
        case DMA_ID_UART1_RX: __DMA_REQ_ID_UART1_RX(dma_obj->init.request_id); break;
        case DMA_ID_UART1_TX: __DMA_REQ_ID_UART1_TX(dma_obj->init.request_id); break;
        case DMA_ID_SPIM0_RX: __DMA_REQ_ID_SPI0_MASTER_RX(dma_obj->init.request_id); break;
        case DMA_ID_SPIM0_TX: __DMA_REQ_ID_SPI0_MASTER_TX(dma_obj->init.request_id); break;
        case DMA_ID_SPIM1_RX: __DMA_REQ_ID_SPI1_MASTER_RX(dma_obj->init.request_id); break;
        case DMA_ID_SPIM1_TX: __DMA_REQ_ID_SPI1_MASTER_TX(dma_obj->init.request_id); break;
        case DMA_ID_SPIS_RX: __DMA_REQ_ID_SPI_SLAVE_RX(dma_obj->init.request_id); break;
        case DMA_ID_SPIS_TX: __DMA_REQ_ID_SPI_SLAVE_TX(dma_obj->init.request_id); break;
        case DMA_ID_I2C0_RX: __DMA_REQ_ID_I2C0_RX(dma_obj->init.request_id); break;
        case DMA_ID_I2C0_TX: __DMA_REQ_ID_I2C0_TX(dma_obj->init.request_id); break;
        case DMA_ID_I2C1_RX: __DMA_REQ_ID_I2C1_RX(dma_obj->init.request_id); break;
        case DMA_ID_I2C1_TX: __DMA_REQ_ID_I2C1_TX(dma_obj->init.request_id); break;
        case DMA_ID_I2S_RX: __DMA_REQ_ID_I2S_RX(dma_obj->init.request_id); break;
        case DMA_ID_I2S_TX: __DMA_REQ_ID_I2S_TX(dma_obj->init.request_id); break;
        case DMA_ID_ADC: __DMA_REQ_ID_ADC(dma_obj->init.request_id); break;
        case DMA_ID_PDM: __DMA_REQ_ID_PDM(dma_obj->init.request_id); break;
        default: break;
    }
    
    dma_obj->handle.Channel               = dma_obj->init.channel;
    dma_obj->handle.Init.Request_ID       = dma_obj->init.request_id;
    switch(dma_obj->init.src_width){
        case 8: dma_obj->handle.Init.Source_Width = DMA_TRANSFER_WIDTH_8; break;
        case 16: dma_obj->handle.Init.Source_Width = DMA_TRANSFER_WIDTH_16; break;
        case 32: dma_obj->handle.Init.Source_Width = DMA_TRANSFER_WIDTH_32; break;
        default: {
            log_error("func:%s, line:%d, src data width error\r\n", __func__, __LINE__);
            return vd_false;
        }
    }
    switch(dma_obj->init.dst_width){
        case 8: dma_obj->handle.Init.Desination_Width = DMA_TRANSFER_WIDTH_8; break;
        case 16: dma_obj->handle.Init.Desination_Width = DMA_TRANSFER_WIDTH_16; break;
        case 32: dma_obj->handle.Init.Desination_Width = DMA_TRANSFER_WIDTH_32; break;
        default: {
            log_error("func:%s, line:%d, dst data width error\r\n", __func__, __LINE__);
            return vd_false;
        }
    }
    if(dma_obj->init.id % 2){
        dma_obj->handle.Init.Data_Flow = DMA_M2P_DMAC;
        dma_obj->handle.Init.Source_Inc       = DMA_ADDR_INC_INC;
        dma_obj->handle.Init.Desination_Inc   = DMA_ADDR_INC_NO_CHANGE;
    }else{
        dma_obj->handle.Init.Data_Flow = DMA_P2M_DMAC;
        dma_obj->handle.Init.Source_Inc       = DMA_ADDR_INC_NO_CHANGE;
        dma_obj->handle.Init.Desination_Inc   = DMA_ADDR_INC_INC;  
    }
    dma_init(&dma_obj->handle);
    
    NVIC_SetPriority(DMA_IRQn, 0);
    NVIC_EnableIRQ(DMA_IRQn);

    dma_obj->is_init = vd_true;	

    return vd_true;
}

vd_bool_t vdchip_40xx_50xx_dma_start(vd_fr8000_dma_id_e dma_id, const vd_void_t *src_addr, const vd_void_t *dst_addr, vd_uint32_t data_len, vd_void_t (*cb)(vd_void_t *arg), vd_void_t *arg)
{
    vd_fr8000_dma_t *dma_obj = vd_null;
    dma_burst_len_t burst_len;

    dma_obj = vdchip_40xx_50xx_find_dma(dma_id);
    vd_check_return_val_error(dma_obj == vd_null, vd_false, "func:%s, line:%d, dma_id invalid\r\n", __func__, __LINE__);
    vd_check_return_val_error(dma_obj->is_init == vd_false, vd_false, "func:%s, line:%d, dma is not init\r\n", __func__, __LINE__);

    if(dma_obj->handle.Channel == DMA_Channel0){
        switch(dma_obj->init.src_width){
            case 8: burst_len = DMA_BURST_LEN_64; break;
            case 16: burst_len = DMA_BURST_LEN_32; break;
            case 32: burst_len = DMA_BURST_LEN_16; break;
            default: {
                log_error("func:%s, line:%d, src data width error\r\n", __func__, __LINE__);
                return vd_false;
            }
        }
    }else{
        switch(dma_obj->init.src_width){
            case 8: burst_len = DMA_BURST_LEN_32; break;
            case 16: burst_len = DMA_BURST_LEN_16; break;
            case 32: burst_len = DMA_BURST_LEN_8; break;
            default: {
                log_error("func:%s, line:%d, src data width error\r\n", __func__, __LINE__);
                return vd_false;
            }
        } 
    }

    dma_obj->cb = cb;
    dma_obj->arg = arg;

    dma_start_IT(&dma_obj->handle, (vd_uint32_t)src_addr, (vd_uint32_t)dst_addr, data_len, burst_len);

    return vd_true;
}

/*============================================================= 中断服务函数 ==================================================================================*/
/**
 * @brief DMA中断服务函数
 *
 * @param vd_void_t 			
 * @return vd_void_t
 */

__attribute__((section("ram_code"))) void dma_isr(void)
{
    vd_uint8_t i;

    for (i = 0; i < vd_array_size(fr8000_dma); i++){
        if (dma_get_tfr_Status(fr8000_dma[i].handle.Channel)){
            /* Clear Transfer complete status */
            dma_clear_tfr_Status(fr8000_dma[i].handle.Channel);
            /* channel Transfer complete interrupt disable */
            dma_tfr_interrupt_disable(fr8000_dma[i].handle.Channel);

            if(fr8000_dma[i].cb){
                fr8000_dma[i].cb(fr8000_dma[i].arg);
            }
        }
    }
}