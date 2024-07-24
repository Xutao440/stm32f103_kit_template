/*
 * Project: mcu-kit
 * Module:
 * File: cmd_queue2.h
 * Created Date: 2021-03-05 20:39:20
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef _CMD_QUEUE_H
#define _CMD_QUEUE_H

/* ======================================================================================
 * includes
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//#include "base.h"


/* ======================================================================================
 * macros
 */

#define QUEUE_BUF_LEN_MAX (128)
#define CMD_QUEUE_DBG     0
/* ======================================================================================
 * types
 */
typedef struct {
    uint32_t read_pos;
    uint32_t write_pos;
    uint32_t count;  // current element count in buf.
    uint8_t  buf[QUEUE_BUF_LEN_MAX];
} cmd_queue_t;

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */

uint8_t queue_init(cmd_queue_t* queue);

uint32_t _queue_size(cmd_queue_t* queue);

uint32_t queue_pop(cmd_queue_t* queue, uint32_t pop_len);

void queue_push(cmd_queue_t* queue, uint8_t* data, uint32_t data_len);

uint32_t queue_value(cmd_queue_t* queue, uint8_t* value, uint32_t max_size);

uint32_t queue_read(cmd_queue_t* queue, uint8_t* value, uint32_t max_size);

uint8_t queue_clear(cmd_queue_t* queue);


#endif  // _CMD_QUEUE_H