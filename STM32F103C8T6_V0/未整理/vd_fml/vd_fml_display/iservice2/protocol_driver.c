/*
 * Project: DT1866
 * Module: Protocol Driver Driver
 * File: protocol_driver.c
 * Created Date: 2021-07-22 20:49:40
 * Author: meizhen
 * Description: Must provide init, read, write funcitio.
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)

/* ======================================================================================
 * includes
 */
#include "project_config.h"
#if (ISERVICE_EN == ON)
#include "protocol_driver.h"
#include "common.h"
#include "kit_driver_bsp_config.h"
#include "log.h"


/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */
typedef struct {
    vd_uint8_t  cmd[80];  // 十六进制的指令
    vd_uint32_t cmd_len;  // 指令的长度
} simulate_cmd_t;

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
#if (PROTOCOL_SIMULATE_DRIVER_READ)
static simulate_cmd_t simulate_read_cmds[] = {
    {0x7e, 0x08, 0xA5, 0x06, 0x01, 0x00, 0xB4, 0x7E},
    {0x7e, 0x10, 0xa5, 0x06, 0x00, 0x01, 0x01, 0x05, 0x28, 0x00, 0x7d, 0x01, 0x00, 0x00, 0x00, 0x67, 0x7e},  //转义测试
    // "7e10a5060301010528007d020000006B7e",  // 反转义测试
};
#endif

#if (PROTOCOL_SIMULATE_DRIVER_WRITE)
static vd_uint8_t simulate_write_cmd_str[][80] = {
    // yitai-2021-09-08 by meizhen
    // "7E0601060D7E", // 查询版本号
    // "7E10A50601010102250100010200E97E", //设置命令
    // "7E06C006CC7E", // 查询状态
    // "7E07030600107E" // 休眠命令
    // "7e10a5060a0101061900960000007c7e",
    // "7e10a5060a0100061900960000007b7e"
    "7e10a50602010105280096000000827e",  //开机 开始出水 温水 40度 小杯  默认 默认 正常工作
    // "7e10a50602010005280096000000817e", //开机 停止出水 温水 40度 小杯  默认 默认 正常工作
    // "7e10a50602010205280096000000837e", //开机 暂停出水 温水 40度 小杯  默认 默认 正常工作
    // "7e10a50602010105280000000000ec7e", //开机 开始出水 温水 40度 连续  默认 默认 正常工作
    "7e14a506010101a200000000000001000000657e",
};
#endif


static driver_rx_it_callback_t g_driver_rx_it_callback = vd_null;
static vd_void_t *g_driver_cb_arg = vd_null;

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */
#if (PROTOCOL_SIMULATE_DRIVER_READ || PROTOCOL_SIMULATE_DRIVER_WRITE)
static vd_int32_t simulate_init(vd_uint8_t* port_id, vd_uint32_t private) {
    log_debug("simulate_init\r\n");
    for (vd_uint8_t i = 0; i < vd_array_size(simulate_read_cmds); i++) {
        simulate_read_cmds[i].cmd_len = simulate_read_cmds[i].cmd[1];
#if LOG_ENABLE_DEBUG
        log_debug("init read_cmd%d[%d]:", i, simulate_read_cmds[i].cmd_len);
        for (vd_uint8_t j = 0; j < simulate_read_cmds[i].cmd_len; j++) {
            log_debug("%02x", simulate_read_cmds[i].cmd[j]);
        }
        log_debug("\r\n");
#endif
        return vd_true;
    }
}

/**
 * 测试数据上报逻辑，屏蔽底层串口读写
 */
static vd_uint16_t simulate_read(vd_uint8_t fd, vd_uint8_t* data, vd_uint16_t data_len) {
    vd_uint16_t ret = 0;
#if (PROTOCOL_SIMULATE_DRIVER_READ)  // 使用虚拟数据
    static vd_uint32_t read_times = 0;
    static vd_uint32_t i          = 0;
    if (read_times++ > 10) {
        read_times = 0;
        memcpy(data, simulate_read_cmds[i].cmd, simulate_read_cmds[i].cmd_len);
        data_len = simulate_read_cmds[i].cmd_len;
#if LOG_ENABLE_DEBUG
        log_debug("read data[%d]", data_len);
        for (int j = 0; j < data_len; j++) {
            log_debug("%02x", data[j]);
        }
        log_debug("\r\n");
#endif
        if (++i >= vd_array_size(simulate_read_cmds)) {
            i = 0;
        }
        ret = data_len;
    }
#endif

    return ret;
}

/**
 * 测试底层串口读写，自发自收，屏蔽上层逻辑
 */
static vd_bool_t simulate_write(vd_uint8_t fd, vd_uint8_t* data, vd_uint32_t data_len) {
#if (PROTOCOL_SIMULATE_DRIVER_WRITE)
    static vd_uint32_t i           = 0;
    static vd_uint32_t write_times = 0;

    if (write_times++ > 100) {
        write_times = 0;
        if (i >= g_simulate_write_cmd_size) {
            i = 0;
            // return vd_true;
        }
        tty_write(fd, g_simulate_write_cmds[i].cmd, g_simulate_write_cmds[i].cmd_len);
        i++;
    }
#endif

    return vd_true;
}
#endif

/**
 * @brief 中断接收回调注册
 *
 * @param 	cb  	回调函数
 * @param 	arg  	回调参数
 * @return 	vd_bool_t
 */
vd_bool_t protocol_driver_rx_it_callback_register(driver_rx_it_callback_t cb, vd_void_t *arg)
{
	g_driver_rx_it_callback = cb;
	g_driver_cb_arg = arg;
}

/*=========================================串口接收中断处理============================================================*/
//串口接收中断处理
vd_void_t uart_rx_cplt_handler(vd_bsp_port_e uart_id, vd_uint8_t *data, vd_uint16_t data_len, vd_void_t *arg)
{
	if(g_driver_rx_it_callback){
		g_driver_rx_it_callback(uart_id, data, data_len, g_driver_cb_arg);
	}
}


/**
 * @brief :  串口协议驱动初始化
 *
 * @param port_id : 串口号
 * @param method ： 串口数据读取方式
 * @return vd_int8_t
 */
static vd_bool_t protocol_driver_uart_init(vd_bsp_port_e port_id, read_method_e method)
{
	vd_int32_t ret;
	
	vd_check_return_val_error(method >= READ_METHOD_MAX, vd_false, "read method error");
	
	ret = vd_bsp_uart_init(port_id);
	vd_check_return_val_error(ret < 0, vd_false, "in func: %s   line: %d", __func__, __LINE__);
	
	if(method == READ_METHOD_INT){
		ret = vd_bsp_uart_read_it(port_id, uart_rx_cplt_handler, vd_null);
		vd_check_return_val_error(ret < 0, vd_false, "in func: %s   line: %d", __func__, __LINE__);
	}

	return vd_true;
}

static vd_uint16_t protocol_driver_uart_read(vd_bsp_port_e port_id, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t ret;
	
  ret = vd_bsp_uart_read(port_id, data, data_len, 1000);
	vd_check_return_val_error(ret < 0, 0, "in func: %s   line: %d", __func__, __LINE__);
	
	return ret;
}

static vd_bool_t protocol_driver_uart_write(vd_bsp_port_e port_id, vd_uint8_t* data, vd_uint16_t data_len) 
{
	vd_int32_t ret;
	
//    log_array("write_data", data, data_len);
	
  ret = vd_bsp_uart_write(port_id, data, data_len, 1000);
	vd_check_return_val_error(ret < 0, vd_false, "in func: %s   line: %d", __func__, __LINE__);
	vd_check_return_val_error(ret != data_len, vd_false, "in func: %s   line: %d", __func__, __LINE__);
	
	return vd_true;
}




static vd_bool_t protocol_driver_ble_write(vd_bsp_port_e port_id, vd_uint8_t* data, vd_uint16_t data_len) {
//    log_array("write_data", data, data_len);
//    return drv_ble_write(port_id, data, data_len);
}
/* ======================================================================================
 * implementation
 */

/**
 * @brief 获取协议驱动
 *
 * @param 	port_id  	驱动端口号
 * @return 	vd_bool_t
 */
vd_bool_t protocol_driver_get(vd_bsp_port_e port_id, protocol_driver_t* p_driver) {
    vd_bool_t ret = vd_false;

#if (PROTOCOL_SIMULATE_DRIVER_READ || PROTOCOL_SIMULATE_DRIVER_WRITE)
    p_driver->d_init  = simulate_init;
    p_driver->d_read  = simulate_read;
    p_driver->d_write = simulate_write;
    return vd_true;
#endif

    switch (port_id) {
			case VD_PORT_UART0:
			case VD_PORT_UART1:
			case VD_PORT_UART2:
			case VD_PORT_UART3:
			case VD_PORT_UART4:
			case VD_PORT_UART5:{
					p_driver->d_init  = protocol_driver_uart_init;
					p_driver->d_read  = protocol_driver_uart_read;
					p_driver->d_write = protocol_driver_uart_write;
					ret               = vd_true;
			}break;
			case VD_PORT_ISDN:{

			}break;
			case VD_PORT_BLE:{
					p_driver->d_write = protocol_driver_ble_write;
					ret               = vd_true;
			}break;
			default:break;
    }

    return ret;
}
#endif