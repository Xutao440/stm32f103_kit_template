#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdio.h>

#define DEBUG_I2C
#define INFO_I2C

#ifndef NULL
#define NULL     0
#endif

#ifndef FALSE
#define FALSE  0
#endif

#ifndef TRUE
#define TRUE    1
#endif

typedef unsigned char uint8_t;
typedef unsigned short  uint16_t;
typedef signed   char   int8_t;
typedef signed   short  int16_t;


#if 1
#define PRINT_BUF(x,y,z)  {                \
    int16_t loop ; printf(x);              \
    for(loop=0;loop<(z);loop++)           \
    printf("%02x ", (uint8_t)((int8_t*)y)[loop]); \
    printf("\r\n");}
#else
#define PRINT_BUF(x,y,z) do {} while (0);
#endif


#define QUEUE_LEN 2
#define QUEUE_DATA_LEN 255

struct queue_data
{
    uint8_t buff[QUEUE_DATA_LEN];
    uint8_t data_len;
};

typedef enum
{
    QUEUE_EMPTY,
    QUEUE_NORMAL,
    QUEUE_FULL
} queue_flag;

struct queue
{
    queue_flag flag;
    uint8_t rd_idx;
    uint8_t wr_idx;
    struct queue_data item[QUEUE_LEN];
};

typedef enum
{
    COMM_OK     = 0,
    COMM_FAIL   = 1,
    COMM_RF_EXIST,
    COMM_TIMEOUT,
    COMM_NO_CARD,
    COMM_NO_DATA,
    COMM_LEN_UNMATCH,
    COMM_PROTOCAL_UNSPPORT,
    COMM_COLLISION_OUTTIMES,
    COMM_COLLISION_PRESENT,
    COMM_STAT_UNMATCH,
    COMM_CMD_ERROR,
    COMM_DATA_OVERFLOW,
} comm_err;

#if 0
// TODO: temp put here
enum EXC_STEP
{
    RECV_FIRST = 0,
    ORG_IBLOCK = 1,
    ORG_ACKBLOCK,
    ORG_NACKBLOCK,
    ORG_SBLOCK,
    ORG_TRARCV, /*transmitted and received*/
    RCV_IBLOCK,
    RCV_RBLOCK,
    RCV_SBLOCK,
    RCV_INVBLOCK,/*protocol error*/
    NON_EVENT /*end*/
};
#endif
typedef struct
{
    uint8_t * ptr;
    uint8_t len  ;
} flash_data_t;

#endif
