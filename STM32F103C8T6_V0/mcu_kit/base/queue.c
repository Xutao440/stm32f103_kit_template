/* ���λ�����cycle buffer */
#include "queue.h"
#include "log.h"

uint8_t  buff[BUFF_SPACE];    //buff�ܴ�С
uint32_t used_buff_size = 0;  //��ʹ��buff��С   

/*
************************************************************
*	�������ƣ�	NEXT_PLACE
*	�������ܣ�	����Ŀ��λ�õ���һ��λ��
*	��ڲ�����	size����С�� index��Ŀ��λ��
*	���ز�����	��һ��λ��
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
*	�������ƣ�	vd_creat_queue
*	�������ܣ�	����ѭ��buff
*	��ڲ�����	size����С
*	���ز�����	�ɹ�����0�� ʧ�ܷ��ش���0
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
*	�������ƣ�	vd_queue_is_empty
*	�������ܣ�	�ж϶����Ƿ�Ϊ��
*	��ڲ�����	queue:����
*	���ز�����	ture:�գ� false���ǿ�
************************************************************
*/
vd_bool_t queue_is_empty(queue_t* queue)
{
	return queue -> r_index == queue -> w_index;
}

/*
************************************************************
*	�������ƣ�	vd_queue_is_full
*	�������ܣ�	�ж϶����Ƿ�Ϊ��
*	��ڲ�����	queue:����
*	���ز�����	ture������ false������
************************************************************
*/
vd_bool_t queue_is_full(queue_t* queue)
{
	return NEXT_PLACE(queue -> w_index, queue -> size) == queue -> r_index;
}

/*
************************************************************
*	�������ƣ�	vd_queue_push_byte
*	�������ܣ�	���һ���ֽ�
*	��ڲ�����	queue�����У� val��ֵ
*	���ز�����	�ɹ�����0
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
*	�������ƣ�	vd_queue_pull_byte
*	�������ܣ�	����һ���ֽ�
*	��ڲ�����	queue�����У� val������ֵ
*	���ز�����	�ɹ�����0
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
*	�������ƣ�	queue_push_bytes
*	�������ܣ�	���n���ֽ�
*	��ڲ�����	queue�����У� pval�����ݵ�ַ�� len�����ݳ���
*	���ز�����	�ɹ�����0 ��ʧ�ܷ���<0
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
*	�������ƣ�	is_cycle_buff_empty
*	�������ܣ�	buff�Ƿ�Ϊ��
*	��ڲ�����	cycbuf:����buff�ṹ��
*	���ز�����	0���գ� >0���ǿ�
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
*	�������ƣ�	queue_peep_all_bytes
*	�������ܣ�	���Ӷ�������
*	��ڲ�����	queue�����У� pval: ���ӵ�������
*	���ز�����	���ӵ������ݳ���
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
*	�������ƣ�	queue_clear
*	�������ܣ�	��ն���
*	��ڲ�����	queue������
*	���ز�����	��
************************************************************
*/
void queue_clear(queue_t* queue)
{
	queue -> r_index = queue -> w_index;
	queue -> len = 0;
}
