#ifndef __MQ_Queue_H__
#define __MQ_Queue_H__

//#define configSUPPORT_STATIC_ALLOCATION 0 // dynamic with rtos
//#define configSUPPORT_STATIC_ALLOCATION 1 // static with rtos
#define configSUPPORT_STATIC_ALLOCATION 2  // static without rtos
//#define configSUPPORT_STATIC_ALLOCATION 3 // dynamic without alios

/* message queue types */
typedef int          mqd_t;
typedef int          mode_t;
typedef unsigned int size_t;
typedef int          ssize_t;

struct mq_attr {
    long mq_flags;   /* message queue flags */
    long mq_maxmsg;  /* maximum number of messages */
    long mq_msgsize; /* maximum message size */
    long mq_curmsgs; /* number of messages currently queued */
};

#define O_NONBLOCK 1 /* nonblocking I/O */
#define O_CREAT    00000100

#define MQ_PRIO_MAX 16

/* prototypes */
mqd_t   vd_mq_open(const char* name, int oflag, mode_t mode, struct mq_attr* attr);
int     vd_mq_close(mqd_t msgid);
int     vd_mq_send(mqd_t msgid, const char* msg, size_t msg_len, unsigned int msg_prio);
ssize_t vd_mq_receive(mqd_t msgid, char* msg, size_t msg_len, unsigned int* msg_prio);
void    vd_mq_clean(mqd_t msgid);

mqd_t mq_queue_test_init(void);
void  mq_queue_test(mqd_t queue);
//���ض����е���������
unsigned int vd_get_mq_queue_msg_cout(mqd_t msgid) ;
//���ض��в���״̬
unsigned int vd_get_mq_queue_msg_lock(mqd_t msgid) ;
#endif
