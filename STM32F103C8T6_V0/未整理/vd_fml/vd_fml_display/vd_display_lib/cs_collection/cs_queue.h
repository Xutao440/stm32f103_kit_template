#ifndef __CS_QUEUE_H
#define __CS_QUEUE_H
#include "cs_collection_common.h"

typedef struct cs_queue_s{
    vd_void_t* item;
    struct cs_queue_s* next;
    vd_uint32_t size;
} cs_queue_t;

/**
 * @brief ��ʼ��
 *
 * @param queue
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_new(cs_queue_t** queue);

/**
 * @brief ���
 *
 * @param queue
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_enqueue(cs_queue_t* queue, vd_void_t* item);

/**
 * @brief ����
 *
 * @param queue
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_poll(cs_queue_t* queue, vd_void_t** item);

/**
 * @brief ����
 *
 * @param deque
 * @param enum CS_STATE
 */
enum CS_STATE  cs_qeque_foreach(cs_queue_t* queue, void (*fn)(void*));

/**
 * @brief ����
 *
 * @param queue
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_destory(cs_queue_t* queue);

/**
 * @brief ����(�ص�)
 *
 * @param queue
 * @param cb
 * @return enum CS_STATE
 */
enum CS_STATE cs_queue_destory_cb(cs_queue_t* queue, void (*cb)(void*));

#endif // !__CS_QUEUE_H