#ifndef _VD_CYCLE_BUFFER_H
#define _VD_CYCLE_BUFFER_H

#include "base_type.h"
#include "mm32_device.h"

#define BUFF_SPACE  200    //buff大小

//#define NEXT_PLACE(index, size) ((i+1)&0xFF)    // - 1

/*cycle buffer*/
typedef struct {
	         uint8_t* buff;     //buff
	         uint8_t  size;     //buff大小
	volatile uint8_t  len;      //数据长度
	volatile uint8_t  r_index;  //读位置
	volatile uint8_t  w_index;  //写位置
}cycbuf_t;

int is_cycle_buff_empty(cycbuf_t* cycbuf);
int is_cycle_buff_full(cycbuf_t* cycbuf);
int8_t cycle_buff_push_bytes(cycbuf_t* cycbuf, uint8_t* pval, uint8_t len);
int8_t cycle_buff_pull_bytes(cycbuf_t* cycbuf, uint8_t* pval, uint8_t len);
uint8_t creat_cycle_buff(cycbuf_t* cycbuf, uint8_t size);
int cycle_buff_pull_byte(cycbuf_t* cycbuf, unsigned char *pval);
int cycle_buff_push_byte(cycbuf_t* cycbuf, unsigned char val);

uint8_t cycle_buff_peep_read_all_bytes(cycbuf_t* cycbuf, uint8_t* pval);   //不出队读出数据

void cycle_buff_clear(cycbuf_t* cycbuf);

#endif




