#include "vd_bsp_uart.h"
#include "platform_config.h"
#include "vd_log.h"

#ifndef PLATFORM_UART_REGISTER
#define PLATFORM_UART_REGISTER vd_null
#endif

#define UART_RX_BUF_SIZE	200


//���ھ��
typedef struct{
	vd_bsp_uart_t								param;
	
	vd_uint8_t 									rx_buf[UART_RX_BUF_SIZE];
	vd_bsp_uart_rx_callback_t		rx_cb;
	vd_void_t										*rx_arg;		//�ص�����
	
	vd_uint8_t									*tx_buf;		//���ͻ���
	vd_bsp_uart_tx_callback_t 	tx_cb;			//������ɻص�
	vd_void_t										*tx_arg;		//�ص�����
}uart_handle_t;

//��������
typedef struct{
	vd_bsp_uart_register_t	plat_register;
	vd_bsp_uart_ops_t 			ops;
	
}uart_drv_t;

uart_handle_t g_uart_handle[] = {
	#if (PLATFORM_UART0_EN || PLATFORM_UART1_EN || PLATFORM_UART2_EN)
	#if (PLATFORM_UART0_EN == ON)
		{VD_PORT_UART0, PLATFORM_UART0_TX, PLATFORM_UART0_RX, PLATFORM_UART0_BAUDEATE, PLATFORM_UART0_MODE, PLATFORM_UART0_DATA_BITS, PLATFORM_UART0_STOP_BITS, PLATFORM_UART0_PARITY},
	#endif
	#if (PLATFORM_UART1_EN == ON)
		{VD_PORT_UART1, PLATFORM_UART1_TX, PLATFORM_UART1_RX, PLATFORM_UART1_BAUDEATE, PLATFORM_UART1_MODE, PLATFORM_UART1_DATA_BITS, PLATFORM_UART1_STOP_BITS, PLATFORM_UART1_PARITY},
	#endif
	#if (PLATFORM_UART2_EN == ON)
		{VD_PORT_UART2, PLATFORM_UART2_TX, PLATFORM_UART2_RX, PLATFORM_UART2_BAUDEATE, PLATFORM_UART2_MODE, PLATFORM_UART2_DATA_BITS, PLATFORM_UART2_STOP_BITS, PLATFORM_UART2_PARITY},
	#endif
	#else
		{0},
	#endif
};



static uart_drv_t uart_drv = {
	.plat_register = PLATFORM_UART_REGISTER,
	.ops = {0},
};

/**
 * @brief ����ID������������
 *
 * @param uart_id 		UART��Ӧ��ID��
 * @return uart_handle_t*
 */
static uart_handle_t* vd_bsp_uart_find(vd_bsp_port_e uart_id) {
    for (int i = 0; i < vd_array_size(g_uart_handle); i++) {
        if (g_uart_handle[i].param.id == uart_id) {
            return &g_uart_handle[i];
        }
    }
    return vd_null;
}

/**
 * @brief ���ڳ�ʼ��
 * 
 * @param uart_id  		����id
 * @return 	�Ǹ�ֵ��	�ɹ�
						��ֵ��		������ 		@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_init(vd_bsp_port_e  uart_id)
{
	vd_bool_t ret = vd_false;
	uart_handle_t *huartx = vd_null;
	
	vd_check_return_val(uart_id < VD_PORT_UART0 || uart_id >= VD_PORT_UART_MAX, -VD_BSP_ERR_PORT_INVALID);
	
	vd_check_return_val(uart_drv.plat_register == vd_null, -VD_BSP_ERR_REGISTER);
	ret = uart_drv.plat_register(&uart_drv.ops);
	vd_check_return_val(ret == vd_false, -VD_BSP_ERR_REGISTER);
	
	/* ���������б� */
	huartx = vd_bsp_uart_find(uart_id);
	vd_check_return_val_error(huartx == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "uart[%d] not found\n", uart_id - VD_PORT_UART0);
	
	vd_check_return_val(uart_drv.ops.init == vd_null, -VD_BSP_ERR_SDK);
	log_debug("uart[%d] init start\n", uart_id - VD_PORT_UART0);
	ret = uart_drv.ops.init(&huartx->param);
	vd_check_return_val(ret == vd_false, -VD_BSP_ERR_SDK);
	
	log_debug("uart[%d] init ok\n", uart_id - VD_PORT_UART0);
	return VD_BSP_NOERROR;
}

/**
 * @brief ������ʽ��
 * 
 * @param uart_id  			����id
 * @param data 					���ݻ���
 * @param data_len 			���ݳ���
 * @param outtime 			��ʱʱ��
 * @return 	�Ǹ�ֵ��		ʵ�ʶ�ȡ����
						��ֵ��			������ 		@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_read(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime)
{
	vd_int32_t ret = 0;
	uart_handle_t *huartx = vd_null;
	
	vd_check_return_val( uart_id < VD_PORT_UART0 || uart_id >= VD_PORT_UART_MAX , -VD_BSP_ERR_PORT_INVALID);
	
	/* ���������б� */
	huartx = vd_bsp_uart_find(uart_id);
	vd_check_return_val_error(huartx == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "uart[%d] not found\n", uart_id - VD_PORT_UART0);
	
	vd_check_return_val(uart_drv.ops.read == vd_null, -VD_BSP_ERR_SDK);
//	log_debug("uart[%d] read start\n", uart_id - VD_PORT_UART0);
	ret = uart_drv.ops.read(&huartx->param, data, data_len, outtime);
	vd_check_return_val(ret < 0, -VD_BSP_ERR_SDK);
	
//	log_debug("uart[%d] read ok\n", uart_id - VD_PORT_UART0);
	return ret;
}

/**
 * @brief  ����ɻص�
 * 
 * @param uart_id 			���ں�
 * @param arg 					�ص�����
 * @return vd_void_t
 */
static vd_void_t vd_bsp_uart_read_cplt_callback(vd_bsp_port_e uart_id, vd_uint8_t *data, vd_uint16_t data_len, vd_void_t *arg)
{
	uart_handle_t *huartx = arg;
	
	if(huartx->rx_cb){//ִ�лص�
		huartx->rx_cb(uart_id, data, data_len, huartx->rx_arg);
	}
}

/**
 * @brief  �жϷ�ʽ��
 * 
 * @param uart_id 			���ں�
 * @param cb 						�ص�����
 * @param arg 					�ص�����
 * @return vd_int32_t		������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_read_it(vd_bsp_port_e uart_id, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg)
{
	vd_bool_t ret;
	uart_handle_t *huartx = vd_null;
	
	vd_check_return_val( uart_id < VD_PORT_UART0 || uart_id >= VD_PORT_UART_MAX , -VD_BSP_ERR_PORT_INVALID);
	
	/* ���������б� */
	huartx = vd_bsp_uart_find(uart_id);
	vd_check_return_val_error(huartx == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "uart[%d] not found\n", uart_id - VD_PORT_UART0);
	
	vd_check_return_val(uart_drv.ops.read_it == vd_null, -VD_BSP_ERR_SDK);
	ret = uart_drv.ops.read_it(&huartx->param, huartx->rx_buf, 1, vd_bsp_uart_read_cplt_callback, huartx);
	vd_check_return_val(ret == vd_false, -VD_BSP_ERR_SDK);
	
	huartx->rx_cb = cb;
	huartx->rx_arg = arg;
	
	return VD_BSP_NOERROR;
}

/**
 * @brief  DMA��ʽ��
 * 
 * @param uart_id 			���ں�
 * @param cb 						�ص�����
 * @param arg 					�ص�����
 * @return vd_int32_t		������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_read_dma(vd_bsp_port_e uart_id, vd_bsp_uart_rx_callback_t cb, vd_void_t *arg)
{
	vd_bool_t ret;
	uart_handle_t *huartx = vd_null;
	
	vd_check_return_val( uart_id < VD_PORT_UART0 || uart_id >= VD_PORT_UART_MAX , -VD_BSP_ERR_PORT_INVALID);
	
	/* ���������б� */
	huartx = vd_bsp_uart_find(uart_id);
	vd_check_return_val_error(huartx == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "uart[%d] not found\n", uart_id - VD_PORT_UART0);
	
	vd_check_return_val(uart_drv.ops.read_dma == vd_null, -VD_BSP_ERR_SDK);
	ret = uart_drv.ops.read_dma(&huartx->param, huartx->rx_buf, sizeof(huartx->rx_buf), vd_bsp_uart_read_cplt_callback, huartx);
	vd_check_return_val(ret == vd_false, -VD_BSP_ERR_SDK);
	
	huartx->rx_cb = cb;
	huartx->rx_arg = arg;
	
	return VD_BSP_NOERROR;
}


/**
 * @brief  ������ʽд
 * 
 * @param uart_id 			���ں�
 * @param data 					���ݻ���
 * @param data_len 			���ݳ���
 * @param outtime 			��ʱʱ��
 * @return vd_int32_t		�Ǹ�ֵ -- ʵ��д�볤��
												��ֵ	 -- ������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_write(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_uint32_t outtime)
{
	vd_int32_t ret = 0;
	uart_handle_t *huartx = vd_null;
	
	vd_check_return_val( uart_id < VD_PORT_UART0 || uart_id >= VD_PORT_UART_MAX , -VD_BSP_ERR_PORT_INVALID);
	
	/* ���������б� */
	huartx = vd_bsp_uart_find(uart_id);
	vd_check_return_val_error(huartx == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "uart[%d] not found\n", uart_id - VD_PORT_UART0);
	
	vd_check_return_val(uart_drv.ops.write == vd_null, -VD_BSP_ERR_SDK);
	ret = uart_drv.ops.write(&huartx->param, data, data_len, outtime);
	vd_check_return_val(ret < 0, -VD_BSP_ERR_SDK);
	
	return ret;
}

/**
 * @brief  д��ɻص�
 * 
 * @param uart_id 			���ں�
 * @param arg 					�ص�����
 * @return vd_void_t
 */
static vd_void_t vd_bsp_uart_write_cplt_callback(vd_bsp_port_e uart_id, vd_void_t *arg)
{
	uart_handle_t *huartx = arg;
	
	PLATFORM_FREE(huartx->tx_buf);//�ͷŷ��ͻ���
	huartx->tx_buf = vd_null;
	
	if(huartx->tx_cb){//ִ�лص�
		huartx->tx_cb(uart_id, huartx->tx_arg);
		huartx->tx_cb = vd_null;
		huartx->tx_arg = vd_null;
	}
}

/**
 * @brief  �жϷ�ʽд
 * 
 * @param uart_id 			���ں�
 * @param data 					���ݻ���
 * @param data_len 			���ݳ���
 * @param cb 						�ص�����
 * @param arg 					�ص�����
 * @return vd_int32_t		�Ǹ�ֵ -- ʵ��д�볤��
												��ֵ	 -- ������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_write_it(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg)
{
	vd_bool_t ret;
	uart_handle_t *huartx = vd_null;
	
	vd_check_return_val( uart_id < VD_PORT_UART0 || uart_id >= VD_PORT_UART_MAX , -VD_BSP_ERR_PORT_INVALID);
	
	/* ���������б� */
	huartx = vd_bsp_uart_find(uart_id);
	vd_check_return_val_error(huartx == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "uart[%d] not found\n", uart_id - VD_PORT_UART0);
	
	//���뷢�ͻ��棬�����ݿ��������ͻ���
	while(huartx->tx_buf);//�ȴ���һ�η������
	huartx->tx_buf = PLATFORM_MALLOC(data_len);
	vd_check_return_val_error(huartx->tx_buf == vd_null, -VD_BSP_ERR_MALLOC, "uart write_it malloc error\r\n");
	memcpy(huartx->tx_buf, data, data_len);
	
	if(uart_drv.ops.write_it == vd_null){
		log_error("uart[%d] write_it port is null\r\n", uart_id - VD_PORT_UART0);
		PLATFORM_FREE(huartx->tx_buf);
		return -VD_BSP_ERR_SDK;
	}
	ret = uart_drv.ops.write_it(&huartx->param, huartx->tx_buf, data_len, vd_bsp_uart_write_cplt_callback, huartx);
	if(ret == vd_false){
		log_error("uart[%d] write_it error\r\n", uart_id - VD_PORT_UART0);
		PLATFORM_FREE(huartx->tx_buf);
		return -VD_BSP_ERR_SDK;
	}
	
	//�󶨻ص�
	huartx->tx_cb = cb;
	huartx->tx_arg = arg;
	
	return VD_BSP_NOERROR;
}

/**
 * @brief  DMA��ʽд
 * 
 * @param uart_id 			���ں�
 * @param data 					���ݻ���
 * @param data_len 			���ݳ���
 * @param cb 						�ص�����
 * @param arg 					�ص�����
 * @return vd_int32_t		�Ǹ�ֵ -- ʵ��д�볤��
												��ֵ	 -- ������	@ref vd_bsp_error_e
 */
vd_int32_t vd_bsp_uart_write_dma(vd_bsp_port_e uart_id, vd_uint8_t* data, vd_uint16_t data_len, vd_bsp_uart_tx_callback_t cb, vd_void_t *arg)
{
	vd_bool_t ret;
	uart_handle_t *huartx = vd_null;
	
	vd_check_return_val( uart_id < VD_PORT_UART0 || uart_id >= VD_PORT_UART_MAX , -VD_BSP_ERR_PORT_INVALID);
	
	/* ���������б� */
	huartx = vd_bsp_uart_find(uart_id);
	vd_check_return_val_error(huartx == vd_null, -VD_BSP_ERR_PORT_INACTIVE, "uart[%d] not found\n", uart_id - VD_PORT_UART0);
	
	//���뷢�ͻ��棬�����ݿ��������ͻ���
	while(huartx->tx_buf);//�ȴ���һ�η������
	huartx->tx_buf = PLATFORM_MALLOC(data_len);
	vd_check_return_val_error(huartx->tx_buf == vd_null, -VD_BSP_ERR_MALLOC, "uart write_dma malloc error\r\n");
	memcpy(huartx->tx_buf, data, data_len);
	
	if(uart_drv.ops.write_dma == vd_null){
		log_error("uart[%d] write_dma port is null\r\n", uart_id - VD_PORT_UART0);
		PLATFORM_FREE(huartx->tx_buf);
		return -VD_BSP_ERR_SDK;
	}
	ret = uart_drv.ops.write_dma(&huartx->param, huartx->tx_buf, data_len, vd_bsp_uart_write_cplt_callback, huartx);
	if(ret == vd_false){
		log_error("uart[%d] write_dma error\r\n", uart_id - VD_PORT_UART0);
		PLATFORM_FREE(huartx->tx_buf);
		return -VD_BSP_ERR_SDK;
	}
	
	//�󶨻ص�
	huartx->tx_cb = cb;
	huartx->tx_arg = arg;
	
	return VD_BSP_NOERROR;
}




