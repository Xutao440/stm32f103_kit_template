#include "vd_ble.h"
#include <stdio.h>
#include <string.h>
#include "co_printf.h"
#include "gap_api.h"
#include "gatt_api.h"
#include "gatt_sig_uuid.h"
#include "batt_service.h"


/*
 * MACROS (宏定义)
 */

/*
 * CONSTANTS (常量定义)
 */


// Simple GATT Profile Service UUID: 0xFFF0
const uint8_t sp_svc_uuid[] = UUID16_ARR(VD_SVC_UUID);

/******************************* Characteristic 1 defination *******************************/
// Characteristic 1 UUID: 0xFFF1
// Characteristic 1 data 
#define SP_CHAR1_VALUE_LEN  10
uint8_t sp_char1_value[SP_CHAR1_VALUE_LEN] = {0};
// Characteristic 1 User Description
#define SP_CHAR1_DESC_LEN   3
const uint8_t sp_char1_desc[SP_CHAR1_DESC_LEN] = {0x01,0x01,0x01};

/******************************* Characteristic 2 defination *******************************/
// Characteristic 2 UUID: 0xFFF2
// Characteristic 2 data 
#define SP_CHAR2_VALUE_LEN  20
uint8_t sp_char2_value[SP_CHAR2_VALUE_LEN] = {0};
// Characteristic 2 User Description
#define SP_CHAR2_DESC_LEN   17
const uint8_t sp_char2_desc[SP_CHAR2_DESC_LEN] = "Characteristic 2";

/******************************* Characteristic 3 defination *******************************/
// Characteristic 3 UUID: 0xFFF3
// Characteristic 3 data 
#define SP_CHAR3_VALUE_LEN  30
uint8_t sp_char3_value[SP_CHAR3_VALUE_LEN] = {0};
// Characteristic 3 User Description
#define SP_CHAR3_DESC_LEN   17
const uint8_t sp_char3_desc[SP_CHAR3_DESC_LEN] = "Characteristic 3";

/******************************* Characteristic 4 defination *******************************/
// Characteristic 4 UUID: 0xFFF4
// Characteristic 4 data 
#define SP_CHAR4_VALUE_LEN  40
uint8_t sp_char4_value[SP_CHAR4_VALUE_LEN] = {0};
// Characteristic 4 client characteristic configuration
#define SP_CHAR4_CCC_LEN   2
uint8_t sp_char4_ccc[SP_CHAR4_CCC_LEN] = {0};
// Characteristic 4 User Description
#define SP_CHAR4_DESC_LEN   17
const uint8_t sp_char4_desc[SP_CHAR4_DESC_LEN] = "Characteristic 4";

/******************************* Characteristic 5 defination *******************************/
// Characteristic 5 UUID: 0xFFF5
uint8_t sp_char5_uuid[UUID_SIZE_2] =
{ 
  LO_UINT16(VD_CHAR5_UUID), HI_UINT16(VD_CHAR5_UUID)
};
// Characteristic 5 data 
#define SP_CHAR5_VALUE_LEN  50
uint8_t sp_char5_value[SP_CHAR5_VALUE_LEN] = {0};
// Characteristic 5 User Description
#define SP_CHAR5_DESC_LEN   17
const uint8_t sp_char5_desc[SP_CHAR5_DESC_LEN] = "Characteristic 5";

/*
 * TYPEDEFS (类型定义)
 */

/*
 * GLOBAL VARIABLES (全局变量)
 */


/*
 * LOCAL VARIABLES (本地变量)
 */
/*********************************************************************
 * Profile Attributes - Table
 * 每一项都是一个attribute的定义。
 * 第一个attribute为Service 的的定义。
 * 每一个特征值(characteristic)的定义，都至少包含三个attribute的定义；
 * 1. 特征值声明(Characteristic Declaration)
 * 2. 特征值的值(Characteristic value)
 * 3. 特征值描述符(Characteristic description)
 * 如果有notification 或者indication 的功能，则会包含四个attribute的定义，除了前面定义的三个，还会有一个特征值客户端配置(client characteristic configuration)。
 *
 */

const gatt_attribute_t vd_profile_att_table[VD_IDX_NB] =
{
    // Simple gatt Service Declaration
    [VD_IDX_SERVICE]                        =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_PRIMARY_SERVICE_UUID) },     /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    UUID_SIZE_2,                                                /* Max size of the value */     /* Service UUID size in service declaration */
                                                    (uint8_t*)sp_svc_uuid,                                      /* Value of the attribute */    /* Service UUID value in service declaration */
                                                },

        // Characteristic 1 Declaration           
    [VD_IDX_CHAR1_DECLARATION]          =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CHARACTER_UUID) },           /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    0,                                                          /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */
                                                },
        // Characteristic 1 Value                  
    [VD_IDX_CHAR1_VALUE]                =   {
                                                    { UUID_SIZE_2, UUID16_ARR(VD_CHAR1_UUID) },                 /* UUID */
                                                    GATT_PROP_READ | GATT_PROP_WRITE,                           /* Permissions */
                                                    SP_CHAR1_VALUE_LEN,                                         /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */    /* Can assign a buffer here, or can be assigned in the application by user */
                                                },             
        // Characteristic 1 User Description
    [VD_IDX_CHAR1_USER_DESCRIPTION]     =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CHAR_USER_DESC_UUID) },      /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    SP_CHAR1_DESC_LEN,                                          /* Max size of the value */
                                                    (uint8_t *)sp_char1_desc,                                   /* Value of the attribute */
                                                },


        // Characteristic 2 Declaration
    [VD_IDX_CHAR2_DECLARATION]          =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CHARACTER_UUID) },           /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    0,                                                          /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */
                                                },
        // Characteristic 2 Value   
    [VD_IDX_CHAR2_VALUE]                =   {
                                                    { UUID_SIZE_2, UUID16_ARR(VD_CHAR2_UUID) },                 /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    SP_CHAR2_VALUE_LEN,                                         /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */	/* Can assign a buffer here, or can be assigned in the application by user */
                                                },   
        // Characteristic 2 User Description
    [VD_IDX_CHAR2_USER_DESCRIPTION]     =   {
                                                   { UUID_SIZE_2, UUID16_ARR(GATT_CHAR_USER_DESC_UUID) },       /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    SP_CHAR2_DESC_LEN,                                          /* Max size of the value */
                                                    (uint8_t *)sp_char2_desc,                                   /* Value of the attribute */
                                                },


        // Characteristic 3 Declaration
    [VD_IDX_CHAR3_DECLARATION]          =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CHARACTER_UUID) },           /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    0,                                                          /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */
                                                },
        // Characteristic 3 Value
    [VD_IDX_CHAR3_VALUE]                =   {
                                                    { UUID_SIZE_2, UUID16_ARR(VD_CHAR3_UUID) },                 /* UUID */
                                                    GATT_PROP_WRITE,                                            /* Permissions */
                                                    SP_CHAR3_VALUE_LEN,                                         /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */    /* Can assign a buffer here, or can be assigned in the application by user */
                                                },
        // Characteristic 3 User Description
    [VD_IDX_CHAR3_USER_DESCRIPTION]     =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CHAR_USER_DESC_UUID) },      /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    SP_CHAR3_DESC_LEN,                                          /* Max size of the value */
                                                    (uint8_t *)sp_char3_desc,                                   /* Value of the attribute */
                                                },


        // Characteristic 4 Declaration
    [VD_IDX_CHAR4_DECLARATION]          =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CHARACTER_UUID) },           /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    0,                                                          /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */
                                                },
        // Characteristic 4 Value
    [VD_IDX_CHAR4_VALUE]                =   {
                                                    { UUID_SIZE_2, UUID16_ARR(VD_CHAR4_UUID) },                 /* UUID */
                                                    GATT_PROP_WRITE | GATT_PROP_NOTI,                           /* Permissions */
                                                    SP_CHAR4_VALUE_LEN,                                         /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */    /* Can assign a buffer here, or can be assigned in the application by user */
                                                },  
        // Characteristic 4 client characteristic configuration
    [VD_IDX_CHAR4_CFG]                  =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CLIENT_CHAR_CFG_UUID) },     /* UUID */
                                                    GATT_PROP_READ | GATT_PROP_WRITE,                           /* Permissions */
                                                    SP_CHAR4_CCC_LEN,                                           /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */    /* Can assign a buffer here, or can be assigned in the application by user */
                                                }, 
        // Characteristic 4 User Description
    [VD_IDX_CHAR4_USER_DESCRIPTION]     =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CHAR_USER_DESC_UUID) },      /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    SP_CHAR4_DESC_LEN,                                          /* Max size of the value */
                                                    (uint8_t *)sp_char4_desc,                                   /* Value of the attribute */
                                                },


        // Characteristic 5 Declaration 
    [VD_IDX_CHAR5_DECLARATION]          =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CHARACTER_UUID) },           /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    0,                                                          /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */
                                                },
        // Characteristic 5 Value
    [VD_IDX_CHAR5_VALUE]                =   {
                                                    { UUID_SIZE_2, UUID16_ARR(VD_CHAR5_UUID) },                 /* UUID */
                                                    GATT_PROP_WRITE | GATT_PROP_READ,             /* Permissions */
                                                    SP_CHAR5_VALUE_LEN,                                         /* Max size of the value */
                                                    NULL,                                                       /* Value of the attribute */    /* Can assign a buffer here, or can be assigned in the application by user */
                                                },
        // Characteristic 5 User Description
    [VD_IDX_CHAR5_USER_DESCRIPTION]     =   {
                                                    { UUID_SIZE_2, UUID16_ARR(GATT_CHAR_USER_DESC_UUID) },      /* UUID */
                                                    GATT_PROP_READ,                                             /* Permissions */
                                                    SP_CHAR5_DESC_LEN,                                          /* Max size of the value */
                                                    (uint8_t *)sp_char5_desc,                                   /* Value of the attribute */
                                                },
};

/*********************************************************************
 * @fn      vd_gatt_read
 *
 * @brief   Simple Profile user application handles read request in this callback.
 *			应用层在这个回调函数里面处理读的请求。
 *
 * @param   p_read  - the pointer to read buffer. NOTE: It's just a pointer from lower layer, please create the buffer in application layer.
 *					  指向读缓冲区的指针。 请注意这只是一个指针，请在应用程序中分配缓冲区. 为输出函数, 因此为指针的指针.
 *          len     - the pointer to the length of read buffer. Application to assign it.
 *                    读缓冲区的长度，用户应用程序去给它赋值.
 *          att_idx - index of the attribute value in it's attribute table.
 *					  Attribute的偏移量.
 *
 * @return  读请求的长度.
 */
vd_void_t vd_gatt_read(vd_uint8_t *p_read, vd_uint16_t *len, vd_uint16_t att_idx)
{
    switch (att_idx)
    {
        case VD_IDX_CHAR1_VALUE:
            for (int i = 0; i < SP_CHAR1_VALUE_LEN; i++)
                sp_char1_value[i] = sp_char1_value[0] + i + 1;
            memcpy(p_read, sp_char1_value, SP_CHAR1_VALUE_LEN);
            *len = SP_CHAR1_VALUE_LEN;
        break;

        case VD_IDX_CHAR2_VALUE:
            for (int i = 0; i < SP_CHAR2_VALUE_LEN; i++)
                sp_char2_value[i] = sp_char2_value[0] + i + 1;
            memcpy(p_read, sp_char2_value, SP_CHAR2_VALUE_LEN);
            *len = SP_CHAR2_VALUE_LEN;
       break;
        
        case VD_IDX_CHAR4_CFG:
            *len = 2;
            memcpy(p_read, sp_char4_ccc, 2);
        break;
        
        case VD_IDX_CHAR5_VALUE:
            for (int i = 0; i < SP_CHAR5_VALUE_LEN; i++)
                sp_char5_value[i] = sp_char3_value[0] + i + 1;
            memcpy(p_read, sp_char5_value, SP_CHAR5_VALUE_LEN);
           *len = SP_CHAR5_VALUE_LEN;
        break;
        
        default:
        break;
    }
    
	co_printf("Read request: len: %d  value: 0x%x 0x%x \r\n", *len, (p_read)[0], (p_read)[*len-1]);
    
}

/*********************************************************************
 * @fn      vd_gatt_write
 *
 * @brief   Simple Profile user application handles write request in this callback.
 *			应用层在这个回调函数里面处理写的请求。
 *
 * @param   write_buf   - the buffer for write
 *			              写操作的数据.
 *					  
 *          len         - the length of write buffer.
 *                        写缓冲区的长度.
 *          att_idx     - index of the attribute value in it's attribute table.
 *					      Attribute的偏移量.
 *
 * @return  写请求的长度.
 */
vd_void_t vd_gatt_write(vd_uint8_t *write_buf, vd_uint16_t len, vd_uint16_t att_idx)
{
	for (int i = 0; i < len; i++)
    {
		co_printf("Write request: len: %d, 0x%x \r\n", len, write_buf[i]);
        if (att_idx == VD_IDX_CHAR1_VALUE)
            memcpy(sp_char1_value, write_buf, len);
        
        if (att_idx == VD_IDX_CHAR3_VALUE)
            memcpy(sp_char3_value, write_buf, len);
        
        if (att_idx == VD_IDX_CHAR5_VALUE)
            memcpy(sp_char5_value, write_buf, len);
    }
	
	uint16_t uuid = BUILD_UINT16( vd_profile_att_table[att_idx].uuid.p_uuid[0], vd_profile_att_table[att_idx].uuid.p_uuid[1] );
	if (uuid == GATT_CLIENT_CHAR_CFG_UUID)
    {
		co_printf("Notification status changed\r\n");
        if (att_idx == VD_IDX_CHAR4_CFG)
        {
            sp_char4_ccc[0] = write_buf[0];
            sp_char4_ccc[1] = write_buf[1];
            co_printf("Char4 ccc: 0x%x 0x%x \r\n", sp_char4_ccc[0], sp_char4_ccc[1]);
        }
    }

}

vd_void_t vd_gatt_ntf(vd_uint8_t att_idx,vd_uint8_t *data,vd_uint8_t data_len)
{
    batt_gatt_notify(att_idx,*data,data_len);
}


