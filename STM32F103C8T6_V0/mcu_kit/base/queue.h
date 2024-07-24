#ifndef _QUEUE_H
#define _QUEUE_H

#include "base_type.h"

#define BUFF_SPACE  200    //buff大小

//#define NEXT_PLACE(index, size) ((i+1)&0xFF)    // - 1

/*cycle buffer*/
typedef struct {
	         uint8_t* buff;     //buff
	         uint8_t  size;     //buff大小
	volatile uint8_t  len;      //数据长度
	volatile uint8_t  r_index;  //读位置
	volatile uint8_t  w_index;  //写位置
}queue_t;

uint8_t creat_queue(queue_t* queue, uint8_t size);                      //创建一个队列
vd_bool_t queue_is_empty(queue_t* queue);                               //判断队列是否为空
vd_bool_t queue_is_full(queue_t* queue);                                //判断队列是否为满
vd_int8_t queue_push_bytes(queue_t* queue, uint8_t* pval, uint8_t len); //入队
int8_t queue_pull_bytes(queue_t* queue, uint8_t* pval, uint8_t len);    //出队
uint8_t queue_peep_all_bytes(queue_t* queue, uint8_t* pval);            //窥视队列中数据
void queue_clear(queue_t* queue);                                       //清空队列

#endif




