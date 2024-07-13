/*
 * Project: linux-kit
 * Module: ����ĳ������Э�鴦��
 * File: machine_pr008.c
 * Created Date: 2022-05-17 17:04:21
 * Author: meizhen
 * Description: ���ļ�ģ��ͨ����������
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

extern msg_dispatch_fun_t g_msg_dispatch;  //��Ϣת������

static version_reply_t     g_version_reply;
static querystatus_reply_t g_querystatus_reply;

/**
 * @brief ��ȡЭ��ָ����
 *
 * @param data
 * @return vd_uint32_t
 */
vd_uint32_t machine_pr008_get_cmdcode(vd_uint8_t* data) {
    return data[0];
}

/**
 * @brief : ��ȡversionָ����Ϣ
 *
 * @return version_reply_t
 */
version_reply_t* machine_pr008_get_version(vd_void_t) {
    return &g_version_reply;
}

/**
 * @brief : ��ȡquerystatusָ����Ϣ
 *
 * @return querystatus_reply_t
 */
querystatus_reply_t* machine_pr008_get_querystatus(vd_void_t) {
    return &g_querystatus_reply;
}

////////����ʹ���////////
/**
 * @brief : ����version����
 *
 * @return vd_bool_t
 */
vd_bool_t machine_pr008_version_cmd_send(version_cmd_t* version_cmd) {
    protocol_cmd_t protocol_cmd;
    vd_bool_t      ret;

    vd_check_return_val_error(version_cmd == vd_false, vd_false, "version_cmd null");

    // 1, �����protocol_cmd
    protocol_cmd.port_id   = MACHINE_PR008_PORT;
    protocol_cmd.is_nr     = 1;              // ָ���Ƿ���Ҫ�ظ�
    protocol_cmd.nr_code   = REPLY_VERSION;  // ָ��ظ���
    protocol_cmd.type      = 0;              // ָ������
    protocol_cmd.interval  = 1000;           // ָ���·�����
    protocol_cmd.is_active = 1;
    protocol_cmd.data_len  = sizeof(version_cmd_t);
    memcpy(protocol_cmd.data, version_cmd, sizeof(version_cmd_t));
    log_info("version_cmd=0x%02X", version_cmd->cmd);

    // 2, ͨ��protocol_server��������
    ret = protocol_server_send_cmd(&protocol_cmd);
    if (!ret) {
        log_error("protocol_server_send_cmd version_cmd error");
    }
    return ret;
}

////////����ʹ���////////
/**
 * @brief : ����querystatus����
 *
 * @return vd_bool_t
 */
vd_bool_t machine_pr008_querystatus_cmd_send(querystatus_cmd_t* querystatus_cmd) {
    protocol_cmd_t protocol_cmd;
    vd_bool_t      ret;

    vd_check_return_val_error(querystatus_cmd == vd_false, vd_false, "querystatus_cmd null");

    // 1, �����protocol_cmd
    protocol_cmd.port_id   = MACHINE_PR008_PORT;
    protocol_cmd.is_nr     = 0;                  // ָ���Ƿ���Ҫ�ظ�
    protocol_cmd.nr_code   = REPLY_QUERYSTATUS;  // ָ��ظ���
    protocol_cmd.type      = 1;                  // ָ������
    protocol_cmd.interval  = 1000;               // ָ���·�����
    protocol_cmd.is_active = 1;
    protocol_cmd.data_len  = sizeof(querystatus_cmd_t);
    memcpy(protocol_cmd.data, querystatus_cmd, sizeof(querystatus_cmd_t));
//    log_info("querystatus_cmd=0x%02X", querystatus_cmd->cmd);

    // 2, ͨ��protocol_server��������
    ret = protocol_server_send_cmd(&protocol_cmd);
    if (!ret) {
        log_error("protocol_server_send_cmd querystatus_cmd error");
    }
    return ret;
}

/**
 * @brief : �յ�version_reply�ظ����ݴ���
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

    // 1, ����ָ���
    memcpy(&g_version_reply, version_reply, sizeof(version_reply_t));
    // version_reply.vendor_code  = big2little16(version_reply->vendor_code);// ���תС��

    // 2,  ����ָ���ҵ���߼�����(�����Ҫ)

    // 3, ת���������ָ����Ϣ������ģ��(�����Ҫ)
    if (g_msg_dispatch) {
        // g_msg_dispatch(0, (vd_uint8_t*)version_reply, sizeof(version_reply_t));
    }

    return vd_true;
}

/**
 * @brief : �յ�querystatus_reply�ظ����ݴ���
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

    // 1, ����ָ���
    memcpy(&g_querystatus_reply, querystatus_reply, sizeof(querystatus_reply_t));
    // querystatus_reply.vendor_code  = big2little16(querystatus_reply->vendor_code);// ���תС��

    // 2,  ����ָ���ҵ���߼�����(�����Ҫ)

    // 3, ת���������ָ����Ϣ������ģ��(�����Ҫ)
    if (g_msg_dispatch) {
        // g_msg_dispatch(0, (vd_uint8_t*)querystatus_reply, sizeof(querystatus_reply_t));
    }

    return vd_true;
}

////////������մ���////////
/**
 * @brief : ͨ��cmd�����Ӧָ��
 *
 * @param port_id: �˿�ID
 * @param data:ָ��
 * @param data_len: ָ���
 * @return vd_bool_t��true:��ָ����Ҫ͸���ϱ���false:��ָ���Ҫ͸���ϱ�
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
 * @brief ��ʼ��
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
