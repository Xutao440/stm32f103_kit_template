#ifndef _QUEUE_H
#define _QUEUE_H

#include "base_type.h"

#define BUFF_SPACE  200    //buff��С

//#define NEXT_PLACE(index, size) ((i+1)&0xFF)    // - 1

/*cycle buffer*/
typedef struct {
	         uint8_t* buff;     //buff
	         uint8_t  size;     //buff��С
	volatile uint8_t  len;      //���ݳ���
	volatile uint8_t  r_index;  //��λ��
	volatile uint8_t  w_index;  //дλ��
}queue_t;

uint8_t creat_queue(queue_t* queue, uint8_t size);                      //����һ������
vd_bool_t queue_is_empty(queue_t* queue);                               //�ж϶����Ƿ�Ϊ��
vd_bool_t queue_is_full(queue_t* queue);                                //�ж϶����Ƿ�Ϊ��
vd_int8_t queue_push_bytes(queue_t* queue, uint8_t* pval, uint8_t len); //���
int8_t queue_pull_bytes(queue_t* queue, uint8_t* pval, uint8_t len);    //����
uint8_t queue_peep_all_bytes(queue_t* queue, uint8_t* pval);            //���Ӷ���������
void queue_clear(queue_t* queue);                                       //��ն���

#endif




