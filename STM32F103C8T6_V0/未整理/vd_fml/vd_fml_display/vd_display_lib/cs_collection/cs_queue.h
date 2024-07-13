#ifndef __CS_QUEUE_H
#define __CS_QUEUE_H
#include "cs_collection_common.h"

typedef struct cs_queue_s{
    vd_void_t* item;
    struct cs_queue_s* next;
    vd_uint32_t size;
} cs_queue_t;

/**
 * @brief 初始化
 *
 * @param queue
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_new(cs_queue_t** queue);

/**
 * @brief 入队
 *
 * @param queue
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_enqueue(cs_queue_t* queue, vd_void_t* item);

/**
 * @brief 出队
 *
 * @param queue
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_poll(cs_queue_t* queue, vd_void_t** item);

/**
 * @brief 遍历
 *
 * @param deque
 * @param enum CS_STATE
 */
enum CS_STATE  cs_qeque_foreach(cs_queue_t* queue, void (*fn)(void*));

/**
 * @brief 销毁
 *
 * @param queue
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_destory(cs_queue_t* queue);

/**
 * @brief 销毁(回调)
 *
 * @param queue
 * @param cb
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_destory_cb(cs_queue_t* queue, void (*cb)(void*));

#endif // !__CS_QUEUE_H