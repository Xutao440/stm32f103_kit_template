
#ifndef VD_GATT_PROFILE_H
#define VD_GATT_PROFILE_H

/*
 * INCLUDES (包含头文件)
 */
#include "base.h"
#include <stdio.h>
#include <string.h>
#include "gap_api.h"
#include "gatt_api.h"
#include "gatt_sig_uuid.h"


/*
 * MACROS (宏定义)
 */

/*
 * CONSTANTS (常量定义)
 */
// Simple Profile attributes index. 
enum
{
    VD_IDX_SERVICE,

    VD_IDX_CHAR1_DECLARATION,
    VD_IDX_CHAR1_VALUE,
    VD_IDX_CHAR1_USER_DESCRIPTION,

    VD_IDX_CHAR2_DECLARATION,
    VD_IDX_CHAR2_VALUE,
    VD_IDX_CHAR2_USER_DESCRIPTION,

    VD_IDX_CHAR3_DECLARATION,
    VD_IDX_CHAR3_VALUE,
    VD_IDX_CHAR3_USER_DESCRIPTION,

    VD_IDX_CHAR4_DECLARATION,
    VD_IDX_CHAR4_VALUE,
    VD_IDX_CHAR4_CFG,
    VD_IDX_CHAR4_USER_DESCRIPTION,
    
    VD_IDX_CHAR5_DECLARATION,
    VD_IDX_CHAR5_VALUE,
    VD_IDX_CHAR5_USER_DESCRIPTION,
    
    VD_IDX_NB,
};

// Simple GATT Profile Service UUID
#define VD_SVC_UUID              0xFFF0

#define VD_CHAR1_UUID            0xFFF1
#define VD_CHAR2_UUID            0xFFF2
#define VD_CHAR3_UUID            0xFFF3
#define VD_CHAR4_UUID            0xFFF4
#define VD_CHAR5_UUID            0xFFF5

/*
 * TYPEDEFS (类型定义)
 */

/*
 * GLOBAL VARIABLES (全局变量)
 */
extern const gatt_attribute_t vd_profile_att_table[];

/*
 * LOCAL VARIABLES (本地变量)
 */


/*
 * PUBLIC FUNCTIONS (全局函数)
 */
vd_void_t vd_gatt_read(vd_uint8_t *p_read, vd_uint16_t *len, vd_uint16_t att_idx);
vd_void_t vd_gatt_write(vd_uint8_t *write_buf, vd_uint16_t len, vd_uint16_t att_idx);
vd_void_t vd_gatt_ntf(vd_uint8_t att_idx,vd_uint8_t *data,vd_uint8_t data_len);
#endif







