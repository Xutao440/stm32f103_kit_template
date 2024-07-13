/*
 * Project: mcu-kit
 * Module: 协议解析处理层
 * File: protocol_server.c
 * Created Date: 2022-03-24 15:03:17
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)

/* ======================================================================================
 * includes
 */
#include "protocol_server.h"
#if (PROTOCOL_SERVER_EN == ON)
#include "exception_event.h"
//#include "kit_driver.h"
#include "kit_framework.h"
#include "queue/mq_queue.h"
#include "event_server.h"
#include "log.h"
#include "string.h"

#if (MACHINE_DEMO == ON)
#include "protocol_server_demo/machine_demo.h"
#endif
#if (MACHINE_PR005 == ON)
#include "machine_pr005/machine_pr005.h"
#endif
#if (MACHINE_PR008 == ON)
#include "machine_pr008/machine_pr008.h"
#endif
#if (MACHINE_HT001 == ON)
#include "machine_ht001/machine_ht001.h"
#endif
#if (MACHINE_PR015 == ON)
#include "machine_pr015/machine_pr015.h"
#endif
#if (MACHINE_PR015_ISDN == ON)
#include "machine_pr015_isdn/machine_pr015_isdn.h"
#endif
#if (MACHINE_PR015_BLE == ON)
#include "machine_pr015_ble/machine_pr015_ble.h"
#endif
#if (OTA_SERVER_EN == ON)
#include "ota_server/ota_server.h"
#endif
/* ======================================================================================
 * macros
 */
#define SEND_TIMEOUT   1000  // ms
#define SEND_COUNT_MAX 3
#define SEND_INTERVAL  10
/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */

static protocol_machine_handler_t g_machine_handlers[] = {
#if (MACHINE_IF006 == ON)
    {MACHINE_IF006_PORT, machine_if006_init, machine_if006_get_cmdcode, machine_if006_receive_handle},
#endif
#if (MACHINE_DEMO == ON)
		{MACHINE_DEMO_PORT, machine_demo_init, machine_demo_get_cmdcode, machine_demo_receive_handle},
#endif
#if (MACHINE_PR005 == ON)
    {MACHINE_PR005_PORT, machine_pr005_init, machine_pr005_get_cmdcode, machine_pr005_receive_handle},
#endif
#if (MACHINE_PR008 == ON)
    {MACHINE_PR008_PORT, machine_pr008_init, machine_pr008_get_cmdcode, machine_pr008_receive_handle},
#endif
#if (MACHINE_HT001 == ON)
    {MACHINE_HT001_PORT, machine_ht001_init, machine_ht001_get_cmdcode, machine_ht001_receive_handle},
#endif
#if (MACHINE_PR015 == ON)
    {MACHINE_PR015_PORT, machine_pr015_init, machine_pr015_get_cmdcode, machine_pr015_receive_handle},
#endif
#if (MACHINE_PR015_ISDN == ON)
    {MACHINE_PR015_ISDN_PORT,
     machine_pr015_isdn_init,
     machine_pr015_isdn_get_cmdcode,
     machine_pr015_isdn_receive_handle},
#endif
#if (MACHINE_PR015_BLE == ON)
    {MACHINE_PR015_BLE_PORT, machine_pr015_ble_init, machine_pr015_ble_get_cmdcode, machine_pr015_ble_receive_handle},
#endif
#if (OTA_SERVER_EN == ON)
    {OTA_PORT, ota_server_receive_handle},
#endif
    {0, 0},
};

// 消息转发函数
msg_dispatch_fun_t g_msg_dispatch = vd_null;

static mqd_t          g_send_queue;
static vd_bool_t      is_send_process = vd_false;
static protocol_cmd_t g_send_cmd;                       // 当前正在发送指令
static protocol_cmd_t g_heart_cmd;                      // 心跳指令
static protocol_cmd_t g_loop_cmd;                       // 循环指令
static protocol_cmd_t g_countdown_cmd;                  // 倒计时指令
static vd_uint64_t     g_loop_interval = SEND_INTERVAL;  // 默认周期1s
static vd_uint8_t     g_heart_count   = 0;
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/**
 * @brief : 接收回调
 *
 * @param port_id
 * @param data
 * @param data_len
 * @return vd_void_t
 */
static vd_void_t protocol_server_receive_callback(vd_bsp_port_e port_id, vd_uint8_t* data, vd_uint16_t data_len) {
    vd_uint8_t i;
	
	log_debug(">>>>>in func: %s\r\n", __func__);
    for (i = 0; i < vd_array_size(g_machine_handlers); i++) {
        if (g_machine_handlers[i].port_id == port_id) {
            // 指令接收处理
            if (g_machine_handlers[i].recv_handler) {
                if (g_machine_handlers[i].recv_handler(port_id, data, data_len)) {
                    protocol_event_t protocol_event;
                    protocol_event.ev_type  = EV_SYS_PROTOCOL;
                    protocol_event.port_id  = port_id;
                    protocol_event.data_len = data_len;
                    memcpy(protocol_event.protocol_data, data, data_len);
                    event_push_l(&protocol_event);
                }
            }
            // 判断指令是否是回复指令
            if (g_machine_handlers[i].get_cmdcode) {
                if (is_send_process) {
                    if (port_id == g_send_cmd.port_id) {
                        if (g_machine_handlers[i].get_cmdcode(data) == g_send_cmd.nr_code) {
                            is_send_process = vd_false;
                            log_debug("nr_code=0x%02x get reply!!!\r\n", g_send_cmd.nr_code);
                        }
                    }
                }
            }
            if (port_id == g_heart_cmd.port_id) {
                g_heart_count = 0;
            }
        }
    }
}

//vd_void_t protocol_server_test(vd_void_t) {
//    protocol_buffer_t buffer;
//    buffer.port_id = PLATFORM_UART0;
//    buffer.buf[0]  = 0x01;
//    buffer.buf[1]  = 0x02;
//    buffer.buf[2]  = 0x7D;
//    buffer.buf[3]  = 0x04;
//    buffer.buf[4]  = 0x7E;
//    buffer.buf_len = 5;
//    protocol_format_write(&buffer);
//}

//vd_void_t protocol_server_ble_rec(vd_uint8_t * _buf, vd_uint8_t len) {
//    protocol_buffer_t buffer;
//    buffer.port_id = MACHINE_PR015_BLE_PORT;
//		memcpy(buffer.buf, _buf, len);
//    buffer.buf_len = len;
//    protocol_format_write(&buffer);
//}



/**
 * @brief : 指令发送到iservice
 *
 * @param protocol_cmd
 * @return vd_bool_t
 */
static vd_bool_t protocol_server_send(protocol_cmd_t* protocol_cmd) 
{
    if (protocol_cmd->data_len <= PROTOCOL_PAYLOAD_SIZE_MAX) 
	{
		return iservice_write(protocol_cmd->port_id, 
							  protocol_cmd->format_id, 
							  protocol_cmd->data, 
							  protocol_cmd->data_len
							 );
    }
	else 
	{
        log_error("protocol_cmd payload=%d > PROTOCOL_PAYLOAD_SIZE_MAX=%d\r\n",protocol_cmd->data_len,PROTOCOL_PAYLOAD_SIZE_MAX);
        return vd_false;
    }
}

/**
 * @brief : 指令超时处理
 *
 * @return vd_bool_t
 */
static vd_bool_t protocol_server_send_timeout(vd_void_t) {
    static vd_uint32_t send_count    = 0;
    static vd_uint64_t  pre_timestamp = 0;

    if (!is_send_process) {
        send_count    = 0;
        pre_timestamp = 0;
        return vd_false;
    }

    // 没回复，重复发送三次
    if (pre_timestamp == 0) {
        pre_timestamp = common_time_get_current_time_ms();
    }

    if (common_time_get_current_time_ms() - pre_timestamp > SEND_TIMEOUT) {
        if (send_count++ < SEND_COUNT_MAX) {
            log_debug("send cmd %d count, nr_code=0x%02x\r\n", send_count, g_send_cmd.nr_code);
            if(!protocol_server_send(&g_send_cmd)){
                exception_event_report(error_code_write_cmd_failed);
            }
            pre_timestamp = common_time_get_current_time_ms();
        } else {
            send_count      = 0;
            pre_timestamp   = 0;
            is_send_process = vd_false;
            log_error(
                "resend cmd(nr_code=0x%02x) %d times without reply!!! next cmd\r\n", g_send_cmd.nr_code, SEND_COUNT_MAX);
            exception_event_report(error_code_write_cmd_timeout);
        }
    }

    return vd_true;
}

/**
 * @brief
 *
 * @return vd_void_t
 */
static vd_void_t protocol_server_send_task(vd_void_t) {
    // 1,处理发送超时(需要回复的指令)
    static vd_uint64_t pre_timestamp = 0;

    if (protocol_server_send_timeout()) {
        return;
    }

    if (pre_timestamp == 0) {
        pre_timestamp = common_time_get_current_time_ms();
    }

    // g_loop_interval保护
    if (g_loop_interval <= 0) {
        g_loop_interval = SEND_INTERVAL;
    }

    // 2,获取指令发送(从发送队列)
    if (common_time_get_current_time_ms() - pre_timestamp >= g_loop_interval-10) {
			machine_fc001_send_data_handle();
			pre_timestamp = common_time_get_current_time_ms();
        if (!is_send_process) {
            if (vd_mq_receive(g_send_queue, (vd_char_t*)&g_send_cmd, sizeof(protocol_cmd_t), vd_null) > 0) {
//                log_debug("normal cmd");
                if (g_send_cmd.is_nr) {
                    is_send_process = vd_true;  // 此指令需要回复
                }
                g_loop_interval = g_send_cmd.interval;
                if (!protocol_server_send(&g_send_cmd)) {
                    exception_event_report(error_code_write_cmd_failed);
                }
            } else {  // 空闲时发送循环指令,倒计时指令,或心跳指令
                if (g_loop_cmd.is_active) {
//                    log_debug("loop cmd");
                    g_loop_interval = g_loop_cmd.interval;
                    if (!protocol_server_send(&g_loop_cmd)) {
                        exception_event_report(error_code_write_cmd_failed);
                    }
                } else if (g_countdown_cmd.is_active) {
//                    log_debug("countdown cmd");
                    g_loop_interval = g_countdown_cmd.interval;
                    if (!protocol_server_send(&g_countdown_cmd)) {
                        exception_event_report(error_code_write_cmd_failed);
                    }
                } else if (g_heart_cmd.is_active) {
//                    log_debug("heart cmd");
                    g_loop_interval = g_heart_cmd.interval;
                    if (!protocol_server_send(&g_heart_cmd)) {
                        exception_event_report(error_code_write_cmd_failed);
                    } else {
                        if (g_heart_count++ > 5) {
                            g_heart_count = 0;
                            exception_event_report(error_code_heart_timeout);
                        }
                    }
                } else {
                    log_error("no cmd, wait\r\n");
                    g_loop_interval = 0xFFFFFFFF;
                }
            }
        }
        
    }
}

/* ======================================================================================
 * implementation
 */

/**
 * @brief : 获取协议指令码
 *
 * @param data
 * @return vd_uint32_t
 */
//vd_uint32_t protocol_server_get_cmdcode(vd_bsp_port_e port_id, vd_uint8_t* data) {
//    vd_uint8_t i;
//    for (i = 0; i < vd_array_size(g_machine_handlers); i++) {
//        if (g_machine_handlers[i].port_id == port_id) {
//            if (g_machine_handlers[i].get_cmdcode) {
//                return g_machine_handlers[i].get_cmdcode(data);
//            }
//        }
//    }
//    return 0;
//}

/**
 * @brief : 指令发送函数
 *
 * @param protocol_cmd_t	
 * @return vd_bool_t
 */
vd_bool_t protocol_server_send_cmd(protocol_cmd_t* protocol_cmd) {
    vd_check_return_val(protocol_cmd == vd_null, vd_false;);

    // protocol_buffer_t buffer;
    // buffer.port_id = protocol_cmd->port_id;
    // buffer.buf_len = protocol_cmd->data_len;

    // if (protocol_cmd->data_len > PROTOCOL_PAYLOAD_SIZE_MAX) {
    //     log_error(
    //         "protocol_cmd payload=%d > PROTOCOL_PAYLOAD_SIZE_MAX=%d",
    //         protocol_cmd->data_len,
    //         PROTOCOL_PAYLOAD_SIZE_MAX);
    //     return vd_false;
    // }
    // memcpy(buffer.buf, protocol_cmd->data, protocol_cmd->data_len);
    // protocol_format_write(&buffer);
    if (!is_send_process) {
        g_loop_interval = 0;
    }
    switch (protocol_cmd->type) {
        case CMD_TYPE_NORMAL:
            break;
        case CMD_TYPE_HEART:
            memcpy(&g_heart_cmd, protocol_cmd, sizeof(protocol_cmd_t));
            break;
        case CMD_TYPE_LOOP:
            memcpy(&g_loop_cmd, protocol_cmd, sizeof(protocol_cmd_t));
            break;
        case CMD_TYPE_COUNTDOWN:
            memcpy(&g_countdown_cmd, protocol_cmd, sizeof(protocol_cmd_t));
            break;
        case CMD_TYPE_CLEAR:
            is_send_process = vd_false;
            g_loop_interval = SEND_INTERVAL;
            vd_mq_clean(g_send_queue);
            return vd_true;
        default:
            break;
    }

    if (vd_mq_send(g_send_queue, (vd_char_t*)protocol_cmd, sizeof(protocol_cmd_t), 0) < 0) {
        exception_event_report(error_code_send_queue_full);
    }
    return vd_true;
}

/**
 * @brief : 协议服务循环
 *
 * @return vd_void_t
 */
vd_void_t protocol_server_loop(vd_void_t) {
// protocol_server_test();
// 1, 心跳
// 2, 指令超时重发
// 3, Protocol_ota
// 4, 循环发送
// protocol_server_receive_task();

// 1, 优先处理OTA升级
#if (OTA_SERVER_EN == ON)
    if (ota_server_is_run()) {
        ota_server_loop();
    } else
#endif
    {
//			log_debug("protocal_server inter\n\r");
//        protocol_server_send_task();
			
    }
}

/**
 * @brief : 协议服务层初始化,注册服务接收处理函数
 *
 * @return vd_bool_t
 */
vd_bool_t protocol_server_init(vd_void_t) {
    memset(&g_heart_cmd, 0, sizeof(protocol_cmd_t));
    memset(&g_loop_cmd, 0, sizeof(protocol_cmd_t));
    memset(&g_countdown_cmd, 0, sizeof(protocol_cmd_t));

    iservice_receive_callback_register(protocol_server_receive_callback);

    struct mq_attr qattr;
    qattr.mq_flags   = 0;
    qattr.mq_maxmsg  = PROTOCOL_SEND_QUEUQ_MAX;
    qattr.mq_msgsize = sizeof(protocol_cmd_t);
    g_send_queue     = vd_mq_open("g_send_queue", O_CREAT, 0644, &qattr);
    log_debug("g_send_queue=%d\r\n", g_send_queue);

    if (g_send_queue < 0) {
        log_error("protocol_server_init failed\r\n");
        return vd_false;
    }
    log_debug("protocol_server_init success\r\n");

    for (vd_uint8_t i = 0; i < vd_array_size(g_machine_handlers); i++) {
        if (g_machine_handlers[i].init) {
            g_machine_handlers[i].init();
        }
    }
    return vd_true;
}
#endif