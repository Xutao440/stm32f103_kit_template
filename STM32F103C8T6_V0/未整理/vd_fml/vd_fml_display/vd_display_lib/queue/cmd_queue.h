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

#include "base.h"

/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */

#define CMD_QUEUE_DBG     0
/* ======================================================================================
 * types
 */
struct vd_cmd_queue
{
    vd_uint8_t *buff_ptr;
    vd_uint32_t read_mirror :1;
    vd_uint32_t read_index :15;
    vd_uint32_t write_mirror :1;
    vd_uint32_t write_index :15;
    vd_int32_t buffer_size;
};

enum cmd_queue_state
{
    vd_cmd_queue_empty,
    vd_cmd_queue_full,
    vd_cmd_queue_halffull,
};
/* ======================================================================================
 * declaration
 */
#define vd_cmd_queue_space_len(cmd_q) ((cmd_q)->buffer_size - vd_queue_size(cmd_q))

typedef struct vd_cmd_queue cmd_queue_t;
/* ======================================================================================
 * extern
 */
enum cmd_queue_state vd_cmd_queue_state(struct vd_cmd_queue *cmd_q);

uint8_t vd_queue_init(struct vd_cmd_queue* cmd_q,vd_uint8_t *pool,vd_int32_t size);

uint32_t vd_queue_size(struct vd_cmd_queue* cmd_q);

uint8_t vd_queue_clear(struct vd_cmd_queue* cmd_q);

uint32_t vd_queue_push(struct vd_cmd_queue* cmd_q, uint8_t* data, uint32_t data_len);

uint32_t vd_queue_pull(struct vd_cmd_queue* cmd_q, uint8_t* value, uint32_t read_len);

uint32_t vd_queue_pull_notout(struct vd_cmd_queue* cmd_q, uint8_t* value, uint32_t read_len);


__vd_extern_c_leave__
#endif  // _CMD_QUEUE_H
