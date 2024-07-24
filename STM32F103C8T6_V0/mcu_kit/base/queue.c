/* 环形缓冲区cycle buffer */
#include "queue.h"
#include "log.h"

uint8_t  buff[BUFF_SPACE];    //buff总大小
uint32_t used_buff_size = 0;  //已使用buff大小   

/*
************************************************************
*	函数名称：	NEXT_PLACE
*	函数功能：	返回目标位置的下一个位置
*	入口参数：	size：大小， index：目标位置
*	返回参数：	下一个位置
************************************************************
*/
uint8_t NEXT_PLACE(uint8_t index, uint8_t size)
{
	uint8_t ret_value = 0;
	
	if(index >= size - 1){
		ret_value = 0;
	}else{
		ret_value = index + 1;
	}
	
	return ret_value;
}

/*
************************************************************
*	函数名称：	vd_creat_queue
*	函数功能：	创建循环buff
*	入口参数：	size：大小
*	返回参数：	成功返回0， 失败返回大于0
************************************************************
*/
uint8_t creat_queue(queue_t* queue, uint8_t size)
{
	uint8_t ret_value = 0;
	
	if(BUFF_SPACE - used_buff_size < size){
		ret_value = 1;
		goto erro;
	}
	queue->buff = &buff[used_buff_size];
	queue->size = size;
	queue->len = 0;
	queue->r_index = 0;
	queue->w_index = 0;	
	used_buff_size += size;
	
erro:
	return ret_value;
	
}


/*
************************************************************
*	函数名称：	vd_queue_is_empty
*	函数功能：	判断队列是否为空
*	入口参数：	queue:队列
*	返回参数：	ture:空， false：非空
************************************************************
*/
vd_bool_t queue_is_empty(queue_t* queue)
{
	return queue -> r_index == queue -> w_index;
}

/*
************************************************************
*	函数名称：	vd_queue_is_full
*	函数功能：	判断队列是否为满
*	入口参数：	queue:队列
*	返回参数：	ture：满， false：不满
************************************************************
*/
vd_bool_t queue_is_full(queue_t* queue)
{
	return NEXT_PLACE(queue -> w_index, queue -> size) == queue -> r_index;
}

/*
************************************************************
*	函数名称：	vd_queue_push_byte
*	函数功能：	入队一个字节
*	入口参数：	queue：队列， val：值
*	返回参数：	成功返回0
************************************************************
*/
static vd_int8_t queue_push_byte(queue_t* queue, vd_uint8_t val)
{
	vd_int8_t ret = 0;
	
	queue -> buff[queue -> w_index] = val;
	queue -> w_index = NEXT_PLACE(queue -> w_index, queue -> size);
	queue -> len++;
	
	return ret;
}

/*
************************************************************
*	函数名称：	vd_queue_pull_byte
*	函数功能：	出队一个字节
*	入口参数：	queue：队列， val：出队值
*	返回参数：	成功返回0
************************************************************
*/
static int queue_pull_byte(queue_t* queue, vd_uint8_t* pval)
{
	vd_int8_t ret = 0;
	
	*pval = queue -> buff[queue -> r_index] ;
	queue -> r_index = NEXT_PLACE(queue -> r_index, queue -> size);
	queue -> len--;
	
	return ret;
}

/*
************************************************************
*	函数名称：	queue_push_bytes
*	函数功能：	入队n个字节
*	入口参数：	queue：队列， pval：数据地址， len：数据长度
*	返回参数：	成功返回0 ，失败返回<0
************************************************************
*/
vd_int8_t queue_push_bytes(queue_t* queue, uint8_t* pval, uint8_t len)
{
	uint8_t index = 0;
	
	if((len > (queue ->size - queue -> len)) || (pval == NULL)){
		log_error("push bytes fail! len=%d, cycbuf->len=%d\r\n",len,  queue -> len);
		return -1;	
	}
	
	for(index = 0; index < len; index++){
		queue_push_byte(queue, pval[index]);
	}
	
//	vd_log.debug("cycle_buff_push_bytes: ");
//	for(int i=0; i<len; i++){
//		vd_log.debug("%d ",cycbuf->buff[cycbuf->r_index + i]);
//	}
//	vd_log.debug("\r\n");
	return 0;
}

/*
************************************************************
*	函数名称：	is_cycle_buff_empty
*	函数功能：	buff是否为空
*	入口参数：	cycbuf:环形buff结构体
*	返回参数：	0：空， >0：非空
************************************************************
*/
int8_t queue_pull_bytes(queue_t* queue, uint8_t* pval, uint8_t len)
{
	uint8_t index = 0;
	
	if(len > queue -> len || (pval == NULL) || (queue == NULL)){
		log_error("pull bytes fail! len= %d, cycbuf->len= %d\r\n",len, queue->len);
		return -1;	
	}
	
	for(index = 0; index < len; index++){
		queue_pull_byte(queue, pval++);
	}
	
	return 0;

}

/*
************************************************************
*	函数名称：	queue_peep_all_bytes
*	函数功能：	窥视队列数据
*	入口参数：	queue：队列， pval: 窥视到的数据
*	返回参数：	窥视到的数据长度
************************************************************
*/
uint8_t queue_peep_all_bytes(queue_t* queue, uint8_t* pval)
{
	if((pval == NULL) || (queue == NULL)){
		log_error("peep read all bytes fail! \r\n");
		return 1;	
	}	
	for(uint8_t i = 0; i< queue -> len; i++){
		pval[i] = queue -> buff[queue -> r_index + i];
	}
	
	return queue -> len;
}



/*
************************************************************
*	函数名称：	queue_clear
*	函数功能：	清空队列
*	入口参数：	queue：队列
*	返回参数：	无
************************************************************
*/
void queue_clear(queue_t* queue)
{
	queue -> r_index = queue -> w_index;
	queue -> len = 0;
}
