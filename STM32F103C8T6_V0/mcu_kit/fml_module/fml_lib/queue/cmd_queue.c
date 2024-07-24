/*
 * Project: mcu-kit
 * Module:
 * File: cmd_queue.c
 * Created Date: 2021-03-05 20:42:12
 * Author: meizhen
 * Description: 循环队列.
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */


/* ======================================================================================
 * includes
 */
#include "cmd_queue.h"




/**
 * @brief : 初始化队列
 *
 * @param queue
 * @return uint8_t
 */
uint8_t queue_init(cmd_queue_t* queue) {
    memset(queue, 0, sizeof(cmd_queue_t));
    return 0;
}

/**
 * @brief : 获取队列中数据长度
 *
 * @param queue
 * @return uint32_t
 */
uint32_t _queue_size(cmd_queue_t* queue) {
    if (queue) return queue->count;
    return 0;
}

/**
 * @brief : 出队pop_len这个字节
 *
 * @param queue
 * @param pop_len
 * @return uint32_t
 */
uint32_t queue_pop(cmd_queue_t* queue, uint32_t pop_len) {
    if (!queue || !pop_len) return 0;

    if (queue->count <= 0) {
        //log_error("[zmz] queue empty!");
        return 0;
    }

    // ithEnterCritical();

    if (pop_len > queue->count) {
        pop_len = queue->count;
    }
    queue->count -= pop_len;
    queue->read_pos = (queue->read_pos + pop_len) % QUEUE_BUF_LEN_MAX;

    // ithExitCritical();


    return pop_len;
}

/**
 * @brief : 入队data_len字节
 *
 * @param queue
 * @param data
 * @param data_len
 */
void queue_push(cmd_queue_t* queue, uint8_t* data, uint32_t data_len) {
    if (!queue) return;

    if (queue->count >= QUEUE_BUF_LEN_MAX) {
//        log_error("[zmz] queue full!,max_size=%d", QUEUE_BUF_LEN_MAX);
//        log_error("write_pos=%d,read_pos=%d,count=%d", queue->write_pos, queue->read_pos, queue->count);
        return;
    }

    if (queue->count + data_len > QUEUE_BUF_LEN_MAX) {
//        log_error(
//            "[zmz] queue not enough space(count=%d,data_len=%d,max=%d)\r\n", queue->count, data_len, QUEUE_BUF_LEN_MAX);
        return;
    }

    // ithEnterCritical();

    if (queue->write_pos + data_len <= QUEUE_BUF_LEN_MAX) {
        memcpy(&queue->buf[queue->write_pos], &data[0], data_len);
    } else {
        memcpy(&queue->buf[queue->write_pos], &data[0], QUEUE_BUF_LEN_MAX - queue->write_pos);
        memcpy(
            &queue->buf[0],
            &data[QUEUE_BUF_LEN_MAX - queue->write_pos],
            data_len - (QUEUE_BUF_LEN_MAX - queue->write_pos));
    }

    queue->write_pos = (queue->write_pos + data_len) % QUEUE_BUF_LEN_MAX;
    queue->count += data_len;

    // ithExitCritical();
	if(queue->count>=QUEUE_BUF_LEN_MAX-1){queue->count=0;}

}

/**
 * @brief : 获取队列中max_size个字节，不出队
 *
 * @param queue
 * @param value
 * @param max_size
 * @return uint32_t
 */
uint32_t queue_value(cmd_queue_t* queue, uint8_t* value, uint32_t max_size) {
    uint32_t need_size = 0;
    if (!queue || !value) return 0;

    need_size = queue->count < max_size ? queue->count : max_size;

    if (need_size > 0) {
        // ithEnterCritical();

        if (queue->read_pos + need_size <= QUEUE_BUF_LEN_MAX) {
            memcpy(value, &queue->buf[queue->read_pos], need_size);
        } else {
            memcpy(value, &queue->buf[queue->read_pos], QUEUE_BUF_LEN_MAX - queue->read_pos);
            //log_array("value1", value, QUEUE_BUF_LEN_MAX - queue->read_pos);
            memcpy(
                &value[QUEUE_BUF_LEN_MAX - queue->read_pos],
                &queue->buf[0],
                need_size - (QUEUE_BUF_LEN_MAX - queue->read_pos));
            //log_array("value2", value, need_size);
        }
        // ithExitCritical();
    }

    return need_size;
}

/**
 * @brief
 *
 * @param queue : 获取队列中max_size个字节，并出队
 * @param value
 * @param max_size
 * @return uint32_t
 */
uint32_t queue_read(cmd_queue_t* queue, uint8_t* value, uint32_t max_size) {
    uint32_t len = 0;

    len = queue_value(queue, value, max_size);
    if (len) {
        queue_pop(queue, len);
    }

    return len;
}

/**
 * @brief : 清空队列
 *
 * @param queue
 * @return uint8_t
 */
uint8_t queue_clear(cmd_queue_t* queue) {
    if (queue) memset(queue, 0, sizeof(cmd_queue_t));
    return 0;
}
// Add end by meizyen 20210305.
