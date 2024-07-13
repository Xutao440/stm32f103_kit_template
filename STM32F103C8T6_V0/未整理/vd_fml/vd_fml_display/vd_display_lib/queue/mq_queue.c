// Add by meizhen 20210830 start
#include "mq_queue.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "vd_bsp_global_switch/vd_bsp_global_switch.h"
/**
 * POSIX message queue handle implementation definition.
 */
typedef struct {
    void*         xQueue;      ///< Native message queue.
    unsigned long uxItemSize;  ///< Queue item size.
    int           oflag;       ///< Open flags.
		volatile int           capacity;
    int           msg_size;
    int           head;
    int           tail;
		volatile int  lock;
} mqQueue;

#if (configSUPPORT_STATIC_ALLOCATION == 0)

#include "FreeRTOS.h"
#include "queue.h"
#include "dt_log.h"

#elif (configSUPPORT_STATIC_ALLOCATION == 1)

#include "FreeRTOS.h"
#include "queue.h"
#define QUEUE_STATIC_MAX 3
static uint8_t       queue_idx = 0;
static mqQueue       queue_head[QUEUE_STATIC_MAX];
static StaticQueue_t queue_static_buffer[QUEUE_STATIC_MAX];
static uint8_t       queue_storage_arae[3 * 300 + 5 * 14];
static uint16_t      queue_arae_idx = 0;

#elif (configSUPPORT_STATIC_ALLOCATION == 2)
//#include "bx_dbg.h"
#include "vd_log.h"

#define QUEUE_COUNT_MAX 4
#define QUEUE_MSG_COUNT 20
#define QUEUE_MSG_SIZE  100
static mqQueue queue[QUEUE_COUNT_MAX];
static uint8_t queue_idx = 0;
static uint8_t queue_arae[QUEUE_COUNT_MAX * (QUEUE_MSG_COUNT * QUEUE_MSG_SIZE)];

#elif (configSUPPORT_STATIC_ALLOCATION == 3)
#include <aos/kernel.h>
#include <k_api.h>
#endif

#define mq_log vd_logln

mqd_t vd_mq_open(const char* name, int oflag, mode_t mode, struct mq_attr* attr) {
    if (oflag & O_CREAT) {
        mqQueue* q;
#if (configSUPPORT_STATIC_ALLOCATION == 0)

        q = pvPortMalloc(sizeof(mqQueue));
        if (!q) {
            return -1;
        }

        q->xQueue = xQueueCreate(attr->mq_maxmsg, attr->mq_msgsize);

        if (!q->xQueue) {
            vPortFree(q);
            return -1;
        }

#elif (configSUPPORT_STATIC_ALLOCATION == 1)

        if (queue_idx >= QUEUE_STATIC_MAX || queue_arae_idx >= sizeof(queue_storage_arae)) {
            mq_log("mq_open error: %d, %d\r\n", queue_idx, queue_arae_idx);
            return -1;
        }

        q         = &queue_head[queue_idx];
        q->xQueue = xQueueCreateStatic(
            attr->mq_maxmsg, attr->mq_msgsize, &queue_storage_arae[queue_arae_idx], &queue_static_buffer[queue_idx]);

        queue_arae_idx += attr->mq_maxmsg * attr->mq_msgsize;
        queue_idx++;

#elif (configSUPPORT_STATIC_ALLOCATION == 2)
        if (attr->mq_msgsize > QUEUE_MSG_SIZE || attr->mq_maxmsg > QUEUE_MSG_COUNT) {
					mq_log("mq_open error:msg_size ro msg_count to big msgsize:%d SIZE:%d maxmsg:%d COUNT:%d\r\n", attr->mq_msgsize, QUEUE_MSG_SIZE, attr->mq_maxmsg, QUEUE_MSG_COUNT);
            return -1;
        }

        if (queue_idx > QUEUE_COUNT_MAX) {
            mq_log("mq_open error:queue_idx > %d\r\n", QUEUE_COUNT_MAX);
            return -1;
        }
        q = &queue[queue_idx];
        memset(q, 0, sizeof(mqQueue));

        q->xQueue = &queue_arae[queue_idx * (QUEUE_MSG_COUNT * QUEUE_MSG_SIZE)];
        memset(q->xQueue, 0, QUEUE_MSG_COUNT * QUEUE_MSG_SIZE);

        mq_log("q=%p, xQueue=%p,queue_idx=%d\r\n", q, q->xQueue, queue_idx);
        queue_idx++;
#elif (configSUPPORT_STATIC_ALLOCATION == 3)

        q = aos_malloc(sizeof(mqQueue));
        if (q == NULL) {
            return -1;
        }

        q->buf = aos_malloc(attr->mq_msgsize * attr->mq_maxmsg);
        if (q->buf == NULL) {
            aos_free(q);
            return -1;
        }

        ret = aos_queue_new(&(q->xQueue), q->buf, attr->mq_msgsize * attr->mq_maxmsg, attr->mq_msgsize);
        if (ret || !q->xQueue.hdl) {
            aos_free(q);
            aos_free(q->buf);
            return -1;
        }
        q->name = name;
#endif

        q->uxItemSize = attr->mq_msgsize;
        q->oflag      = oflag | attr->mq_flags;
				q->lock = 2;
        return (mqd_t)q;
    } else {
        return -1;
    }
}

int vd_mq_send(mqd_t msgid, const char* msg, size_t msg_len, unsigned int msg_prio) {
    mqQueue* q;

    if (msgid <= 0) {
        log_error("mq_send error: msgid=%d\r\n", msgid);
        return -1;
    }
     vd_global_int_disable();
    q = (mqQueue*)msgid;
		
#if (configSUPPORT_STATIC_ALLOCATION == 0 || configSUPPORT_STATIC_ALLOCATION == 1)
    TickType_t xTicksToWait;

    xTicksToWait = (q->oflag & O_NONBLOCK) ? 0 : portMAX_DELAY;
    if (xQueueSend(q->xQueue, (const void*)msg, xTicksToWait) != pdTRUE) {
        vd_global_int_restore();   
        return -1;
    }

#elif (configSUPPORT_STATIC_ALLOCATION == 2)

    if (q->capacity >= QUEUE_MSG_COUNT) {
        log_error("mq_send error, mq full!");
        q->capacity = QUEUE_MSG_COUNT;
				vd_global_int_restore();
        return -2;
    }

    if (msg_len > QUEUE_MSG_SIZE) {
        log_error("msg_len to long,max=%d", QUEUE_MSG_SIZE);
				vd_global_int_restore();
        return -1;
    }
		q->lock = 1;
    memcpy(&(((uint8_t*)q->xQueue)[q->tail * QUEUE_MSG_SIZE]), msg, msg_len);
    q->msg_size = msg_len;

    q->tail = (q->tail + 1) % QUEUE_MSG_COUNT;
    q->capacity++;
 //    log_debug("mq_send,msgid=%p, xQueue=%p,tail=%d, head =%d, capacity=%d,msg_len=%d\r\n", q, q->xQueue, q->tail,q->head, q->capacity, msg_len);

#elif (configSUPPORT_STATIC_ALLOCATION == 3)

    int ret;
    if (msgid <= 0) {
         vd_global_int_restore();   
        return -1;
    }
    ret = aos_queue_send(&(q->xQueue), (void*)msg, msg_len);
    if (ret) {
        mq_log("aos_queue_send error,ret=%d\r\n", ret);
        vd_global_int_restore();   
        return -1;
    }

#endif
		q->lock = 2;
    vd_global_int_restore();
    return msg_len;
}

ssize_t vd_mq_receive(mqd_t msgid, char* msg, size_t msg_len, unsigned int* msg_prio) {
    mqQueue* q;
    if (msgid <= 0 || msg == NULL) {
        log_error("mq_receive error: msgid=%d, or msg null\r\n", msgid);
        return -1;
    }
     vd_global_int_disable();
    q = (mqQueue*)msgid;
		
#if (configSUPPORT_STATIC_ALLOCATION == 0 || configSUPPORT_STATIC_ALLOCATION == 1)
    TickType_t xTicksToWait;

    xTicksToWait = (q->oflag & O_NONBLOCK) ? 0 : portMAX_DELAY;
    if (xQueueReceive(q->xQueue, msg, xTicksToWait) != pdTRUE) {
         vd_global_int_restore();   
        return -1;
    }

#elif (configSUPPORT_STATIC_ALLOCATION == 2)
    if (q->capacity <= 0) {
			if(q->head != q->tail){
				q->capacity = 1;
			}else{
        q->capacity = 0;
				vd_global_int_restore();   
        return -1;
			}
    }
		q->lock = 1;
    msg_len = msg_len > q->msg_size ? q->msg_size : msg_len;

    memcpy(msg, &((uint8_t*)q->xQueue)[q->head * QUEUE_MSG_SIZE], msg_len);
   // log_debug("mq_receive,msgid=%p, xQueue=%p,tail=%d, head =%d, capacity=%d,msg_len=%d\r\n", q, q->xQueue, q->tail,q->head, q->capacity, msg_len);

    q->head = (q->head + 1) % QUEUE_MSG_COUNT;
    q->capacity--;

#elif (configSUPPORT_STATIC_ALLOCATION == 3)

    unsigned int wait = 0, recv_size = 0;
    if (q->oflag & O_NONBLOCK)
        wait = AOS_NO_WAIT;
    else
        wait = AOS_WAIT_FOREVER;

    if (aos_queue_recv(&(q->xQueue), wait, msg, &recv_size)) {
        vd_global_int_restore();   
        return -1;
    }
    // mq_log("check mq_queue recv Len[%d][%s]\r\n", recv_size, msg);

#endif
		q->lock = 2;
    vd_global_int_restore();   
    return msg_len;
}

void vd_mq_clean(mqd_t msgid) {
    mqQueue* q;
    if (msgid <= 0) {
        mq_log("mq_clean error: msgid=%d\r\n", msgid);
        return;
    }

    q       = (mqQueue*)msgid;
    q->head = q->tail = 0;
    q->capacity       = 0;
}

int vd_mq_close(mqd_t msgid) {
    mqQueue* q;
    if (msgid <= 0) {
        return -1;
    }

    q = (mqQueue*)msgid;

#if (configSUPPORT_STATIC_ALLOCATION == 0 || configSUPPORT_STATIC_ALLOCATION == 1)
    vQueueDelete(q->xQueue);
#if (configSUPPORT_STATIC_ALLOCATION == 0)
    vPortFree(q);
#endif
#elif (configSUPPORT_STATIC_ALLOCATION == 2)
    // no need to free static memmory
#endif
    return 0;
}

// Test case
mqd_t mq_queue_test_init(void) {
    mqd_t          queue;
    struct mq_attr qattr;
    qattr.mq_flags   = 0;
    qattr.mq_maxmsg  = 5;
    qattr.mq_msgsize = 20;
    queue            = vd_mq_open("msg_test", O_CREAT, 0644, &qattr);
    mq_log("mq_open success\r\n");

    return queue;
}

void mq_queue_test(mqd_t queue) {
    // 1, create mq_queue

    // 2, send msg
    char str1[] = "hello world!";
    vd_mq_send(queue, str1, sizeof(str1), 0);
    mq_log("mq_send str1=%s, len=%d\r\n", str1, sizeof(str1));

    // 3, receive msg
    char str2[20];
    int  str2_len;
    str2_len = vd_mq_receive(queue, str2, sizeof(str2), NULL);
    mq_log("mq_receive str2=%s, len=%d\r\n", str2, str2_len);
}

//返回队列中的容器数量
unsigned int vd_get_mq_queue_msg_cout(mqd_t msgid) {
    mqQueue* q;
    if (msgid <= 0) {
        mq_log("mq_send error: msgid=%d\r\n", msgid);
        return 0;
    }
    q = (mqQueue*)msgid;
    return q->capacity;
}
//返回队列操作状态
unsigned int vd_get_mq_queue_msg_lock(mqd_t msgid) {
    mqQueue* q;
    if (msgid <= 0) {
        mq_log("mq_send error: msgid=%d\r\n", msgid);
        return 0;
    }
    q = (mqQueue*)msgid;
    return q->lock;
}
// Add by meizhen 20210830 end
