#include "cs_queue.h"

enum CS_STATE cs_queue_new(cs_queue_t** queue) {
    cs_queue_t* q;
    q = os_malloc(sizeof(cs_queue_t));
    if (vd_null == q) {
        return CS_ERROR;
    }

    q->item = vd_null;
    q->next = vd_null;
    q->size = 0;
    *queue = q;
    return CS_OK;
}

enum CS_STATE cs_queue_enqueue(cs_queue_t* queue, vd_void_t* item) {
    cs_queue_t* q;
    cs_queue_t* next_q;

    if (vd_null == queue) {
        return CS_ERROR;
    }
    if (0 == queue->size) {
        queue->item = item;
        queue->size++;
        return CS_OK;
    }

    q = queue;
    next_q = queue->next;
    while (vd_null != next_q) {
        q = next_q;
        next_q = next_q->next;
    }

    q->next = os_malloc(sizeof(cs_queue_t));
    if (vd_null == q->next) {
        return CS_ERROR;
    }
    q->next->item = item;
    q->next->next = vd_null;
    queue->size++;
    return CS_OK;
}

enum CS_STATE cs_queue_poll(cs_queue_t* queue, vd_void_t** item) {
    cs_queue_t* q;

    if (vd_null == queue) {
        return CS_ERROR;
    }

    if (0 == queue->size) {
        return CS_OUT_QUEUE;
    }

    if (1 == queue->size) {
        *item = queue->item;
        queue->next = vd_null;
        queue->size = 0;
        return CS_OK;
    }

    // bef_q = queue;
    // q = queue->next;
    // while (vd_null != q->next) {
    //     bef_q = q;
    //     q = q->next;
    // }
    // *item = q->item;
    // vd_safe_free(bef_q->next);
    // bef_q->next = vd_null;

    q = queue->next;
    if (vd_null == q) {
        return CS_ERROR;
    }
    *item = queue->item;
    queue->item = queue->next->item;
    queue->next = queue->next->next;
    os_free(q);

    queue->size--;
    return CS_OK;
}

enum CS_STATE cs_qeque_foreach(cs_queue_t* queue, void (*fn)(void*)) {
    cs_queue_t* bef_q;
    cs_queue_t* q;

    if (vd_null == queue) {
        return CS_ERROR;
    }

    bef_q = queue;
    q     = queue->next;
    fn(bef_q->item);
    while (vd_null != q) {
        bef_q = q;
        q     = q->next;
        fn(bef_q->item);
    }
    return CS_OK;
}

enum CS_STATE cs_queue_destory(cs_queue_t* queue) {
    cs_queue_t* bef_q;
    cs_queue_t* q;

    if (vd_null == queue) {
        return CS_ERROR;
    }

    bef_q = queue;
    q = queue->next;
    while (vd_null != q) {
        bef_q = q;
        q = q->next;
        os_free(bef_q);
    }
    os_free(queue);
    return CS_OK;
}

enum CS_STATE cs_queue_destory_cb(cs_queue_t* queue, void (*cb)(void*)) {
    vd_int16_t e;

    e = cs_qeque_foreach(queue, cb);
    if (CS_OK == e) {
        return cs_queue_destory(queue);
    } else {
        return e;
    }
}