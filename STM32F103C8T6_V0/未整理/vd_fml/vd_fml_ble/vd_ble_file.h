#ifndef _VD_BLE_FILE_H
#define _VD_BLE_FILE_H

#include "base.h"

typedef enum
{
    OTA_CMD_NVDS_TYPE,
    OTA_CMD_GET_STR_BASE,
    OTA_CMD_READ_FW_VER,    //read firmware version
    OTA_CMD_PAGE_ERASE,
    OTA_CMD_CHIP_ERASE,
    OTA_CMD_WRITE_DATA,
    OTA_CMD_READ_DATA,
    OTA_CMD_WRITE_MEM,
    OTA_CMD_READ_MEM,
    OTA_CMD_REBOOT,
    OTA_CMD_NULL,
} ota_cmd_t;

struct app_otas_status_t
{
    uint8_t read_opcode;
    uint16_t length;
    uint32_t base_addr;
} app_otas_status;

typedef struct {
    uint8_t cmd; // 0x91
    uint8_t mcu_tran_type;
//    vd_uint8_t mcu_version;
    uint32_t image_addr;
    uint32_t iamge_size;
} ble_iamge_updata_cmd_t;

typedef struct {
    uint8_t cmd; // 0x91
    uint8_t mcu_tran_type;
//    vd_uint8_t mcu_version;
    uint8_t answer;
} ble_iamge_updata_cmd_replay_t;

typedef struct {
    uint8_t cmd; // 0x91
    uint8_t mcu_tran_type;
//    vd_uint8_t mcu_version;
    uint16_t image_bag_cnt;
		uint8_t image_rec_cnt;
    uint8_t iamge_data[85];
} ble_iamge_updata_data_t;

typedef struct {
    uint8_t cmd; // 0x91
    uint8_t mcu_tran_type;
//    vd_uint8_t mcu_version;
    uint8_t answer;
} ble_iamge_updata_data_replay_t;

typedef struct {
    uint32_t read_pos;
    uint32_t write_pos;
    uint32_t count;  // current element count in buf.
    uint8_t  buf[400];
} image_queue_t;

void app_otas_recv_data(uint8_t conidx,uint8_t *p_data,uint16_t len);

uint16_t app_otas_read_data(uint8_t conidx,uint8_t *p_data);


#endif