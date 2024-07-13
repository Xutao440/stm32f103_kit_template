/*
 * Project: linux-kit
 * Module: 具体某个机器协议处理
 * File: machine_pr008.c
 * Created Date: 2022-05-17 17:04:21
 * Author: meizhen
 * Description: 此文件模板通过工具生成
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
#include "machine_pr008.h"

#if (MACHINE_PR008 == ON)

#include "../protocol_server.h"
#include "kit_driver.h"

/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types enum
 */

/* ======================================================================================
 * types struct
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */

extern msg_dispatch_fun_t g_msg_dispatch;  //消息转发函数

static version_reply_t     g_version_reply;
static querystatus_reply_t g_querystatus_reply;

/**
 * @brief 获取协议指令码
 *
 * @param data
 * @return vd_uint32_t
 */
vd_uint32_t machine_pr008_get_cmdcode(vd_uint8_t* data) {
    return data[0];
}

/**
 * @brief : 获取version指令信息
 *
 * @return version_reply_t
 */
version_reply_t* machine_pr008_get_version(vd_void_t) {
    return &g_version_reply;
}

/**
 * @brief : 获取querystatus指令信息
 *
 * @return querystatus_reply_t
 */
querystatus_reply_t* machine_pr008_get_querystatus(vd_void_t) {
    return &g_querystatus_reply;
}

////////命令发送处理////////
/**
 * @brief : 发送version命令
 *
 * @return vd_bool_t
 */
vd_bool_t machine_pr008_version_cmd_send(version_cmd_t* version_cmd) {
    protocol_cmd_t protocol_cmd;
    vd_bool_t      ret;

    vd_check_return_val_error(version_cmd == vd_false, vd_false, "version_cmd null");

    // 1, 组包成protocol_cmd
    protocol_cmd.port_id   = MACHINE_PR008_PORT;
    protocol_cmd.is_nr     = 1;              // 指令是否需要回复
    protocol_cmd.nr_code   = REPLY_VERSION;  // 指令回复码
    protocol_cmd.type      = 0;              // 指令类型
    protocol_cmd.interval  = 1000;           // 指令下发周期
    protocol_cmd.is_active = 1;
    protocol_cmd.data_len  = sizeof(version_cmd_t);
    memcpy(protocol_cmd.data, version_cmd, sizeof(version_cmd_t));
    log_info("version_cmd=0x%02X", version_cmd->cmd);

    // 2, 通过protocol_server发送命令
    ret = protocol_server_send_cmd(&protocol_cmd);
    if (!ret) {
        log_error("protocol_server_send_cmd version_cmd error");
    }
    return ret;
}

////////命令发送处理////////
/**
 * @brief : 发送querystatus命令
 *
 * @return vd_bool_t
 */
vd_bool_t machine_pr008_querystatus_cmd_send(querystatus_cmd_t* querystatus_cmd) {
    protocol_cmd_t protocol_cmd;
    vd_bool_t      ret;

    vd_check_return_val_error(querystatus_cmd == vd_false, vd_false, "querystatus_cmd null");

    // 1, 组包成protocol_cmd
    protocol_cmd.port_id   = MACHINE_PR008_PORT;
    protocol_cmd.is_nr     = 0;                  // 指令是否需要回复
    protocol_cmd.nr_code   = REPLY_QUERYSTATUS;  // 指令回复码
    protocol_cmd.type      = 1;                  // 指令类型
    protocol_cmd.interval  = 1000;               // 指令下发周期
    protocol_cmd.is_active = 1;
    protocol_cmd.data_len  = sizeof(querystatus_cmd_t);
    memcpy(protocol_cmd.data, querystatus_cmd, sizeof(querystatus_cmd_t));
//    log_info("querystatus_cmd=0x%02X", querystatus_cmd->cmd);

    // 2, 通过protocol_server发送命令
    ret = protocol_server_send_cmd(&protocol_cmd);
    if (!ret) {
        log_error("protocol_server_send_cmd querystatus_cmd error");
    }
    return ret;
}

/**
 * @brief : 收到version_reply回复数据处理
 *
 * @param reply
 * @return vd_bool_t
 */
vd_bool_t machine_pr008_version_handle(vd_uint8_t port_id, vd_uint8_t* reply, vd_uint16_t data_len) {
    vd_check_return_val_error(reply == vd_null, vd_false, "reply null");

    if (data_len != sizeof(version_reply_t)) {
        log_error("version_reply data_len error");
        return vd_false;
    }
    version_reply_t* version_reply = (version_reply_t*)reply;

    // 1, 本地指令保存
    memcpy(&g_version_reply, version_reply, sizeof(version_reply_t));
    // version_reply.vendor_code  = big2little16(version_reply->vendor_code);// 大端转小端

    // 2,  本地指令处理业务逻辑代码(如果需要)

    // 3, 转发解析完的指令信息到其他模块(如果需要)
    if (g_msg_dispatch) {
        // g_msg_dispatch(0, (vd_uint8_t*)version_reply, sizeof(version_reply_t));
    }

    return vd_true;
}

/**
 * @brief : 收到querystatus_reply回复数据处理
 *
 * @param reply
 * @return vd_bool_t
 */
vd_bool_t machine_pr008_querystatus_handle(vd_uint8_t port_id, vd_uint8_t* reply, vd_uint16_t data_len) {
    vd_check_return_val_error(reply == vd_null, vd_false, "reply null");

    if (data_len != sizeof(querystatus_reply_t)) {
        log_error("querystatus_reply data_len error");
        return vd_false;
    }
    querystatus_reply_t* querystatus_reply = (querystatus_reply_t*)reply;

    // 1, 本地指令保存
    memcpy(&g_querystatus_reply, querystatus_reply, sizeof(querystatus_reply_t));
    // querystatus_reply.vendor_code  = big2little16(querystatus_reply->vendor_code);// 大端转小端

    // 2,  本地指令处理业务逻辑代码(如果需要)

    // 3, 转发解析完的指令信息到其他模块(如果需要)
    if (g_msg_dispatch) {
        // g_msg_dispatch(0, (vd_uint8_t*)querystatus_reply, sizeof(querystatus_reply_t));
    }

    return vd_true;
}

////////命令接收处理////////
/**
 * @brief : 通过cmd处理对应指令
 *
 * @param port_id: 端口ID
 * @param data:指令
 * @param data_len: 指令长度
 * @return vd_bool_t：true:此指令需要透传上报，false:此指令不需要透传上报
 */
vd_bool_t machine_pr008_receive_handle(vd_uint8_t port_id, vd_uint8_t* data, vd_uint16_t data_len) {
    vd_uint32_t cmdcode;
    vd_bool_t   ret = vd_false;

    vd_check_return_val_error(data == vd_null, vd_false, "data = 0");
    vd_check_return_val_error(data_len == 0, vd_false, "data_len = 0");

    cmdcode = machine_pr008_get_cmdcode(data);

    switch (cmdcode) {
        case REPLY_VERSION:
            ret = machine_pr008_version_handle(port_id, data, data_len);
            break;

        case REPLY_QUERYSTATUS:
            ret = machine_pr008_querystatus_handle(port_id, data, data_len);
            break;

        default:
            log_error("not support this cmdcode");
            break;
    }

    return ret;
}

/**
 * @brief 初始化
 *
 * @return vd_bool_t
 */
vd_bool_t machine_pr008_init(vd_void_t) {
    version_cmd_t version_cmd;
    memset(&version_cmd, 0, sizeof(version_cmd_t));
    version_cmd.cmd = CMD_VERSION;
    machine_pr008_version_cmd_send(&version_cmd);

    querystatus_cmd_t querystatus_cmd;
    memset(&querystatus_cmd, 0, sizeof(querystatus_cmd_t));
    querystatus_cmd.cmd = CMD_QUERYSTATUS;
		log_info("querystatus_cmd1=0x%02X", querystatus_cmd.cmd);
    machine_pr008_querystatus_cmd_send(&querystatus_cmd);

    return vd_true;
}

#endif
