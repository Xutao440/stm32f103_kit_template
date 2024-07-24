/* ���λ�����cycle buffer */
#include "cycle_buffer.h"
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
*	�������ƣ�	creat_cycle_buff
*	�������ܣ�	����ѭ��buff
*	��ڲ�����	size����С
*	���ز�����	�ɹ�����0�� ʧ�ܷ��ش���0
************************************************************
*/
uint8_t creat_cycle_buff(cycbuf_t* cycbuf, uint8_t size)
{
	uint8_t ret_value = 0;
	
	if(BUFF_SPACE - used_buff_size < size){
		ret_value = 1;
		goto erro;
	}
	cycbuf->buff = &buff[used_buff_size];
	cycbuf->size = size;
	cycbuf->len = 0;
	cycbuf->r_index = 0;
	cycbuf->w_index = 0;	
	used_buff_size += size;
	
erro:
	return ret_value;
	
}


/*
************************************************************
*	�������ƣ�	is_cycle_buff_empty
*	�������ܣ�	buff�Ƿ�Ϊ��
*	��ڲ�����	cycbuf:����buff�ṹ��
*	���ز�����	0���գ� >0���ǿ�
************************************************************
*/
int is_cycle_buff_empty(cycbuf_t* cycbuf)
{
	return cycbuf->r_index == cycbuf->w_index;
}

/*
************************************************************
*	�������ƣ�	is_cycle_buff_full
*	�������ܣ�	buff�Ƿ�Ϊ��
*	��ڲ�����	cycbuf:����buff�ṹ��
*	���ز�����	0������ >0������
************************************************************
*/
int is_cycle_buff_full(cycbuf_t* cycbuf)
{
	return NEXT_PLACE(cycbuf->w_index, cycbuf->size) == cycbuf->r_index;
}

/*
************************************************************
*	�������ƣ�	cycle_buff_put
*	�������ܣ�	buff�Ƿ�Ϊ��
*	��ڲ�����	cycbuf:����buff�ṹ��
*	���ز�����	0���գ� >0���ǿ�
************************************************************
*/
int cycle_buff_push_byte(cycbuf_t* cycbuf, unsigned char val)
{
	cycbuf->buff[cycbuf->w_index] = val;
	cycbuf->w_index = NEXT_PLACE(cycbuf->w_index, cycbuf->size);
	cycbuf->len++;
	return 0;
}

/*
************************************************************
*	�������ƣ�	cycle_buff_get
*	�������ܣ�	buff�Ƿ�Ϊ��
*	��ڲ�����	cycbuf:����buff�ṹ��
*	���ز�����	0���գ� >0���ǿ�
************************************************************
*/
int cycle_buff_pull_byte(cycbuf_t* cycbuf, unsigned char *pval)
{
	*pval = cycbuf->buff[cycbuf->r_index] ;
	cycbuf->r_index = NEXT_PLACE(cycbuf->r_index, cycbuf->size);
	cycbuf->len--;
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
int8_t cycle_buff_push_bytes(cycbuf_t* cycbuf, uint8_t* pval, uint8_t len)
{
	uint8_t index = 0;
	
	if((len > (cycbuf->size - cycbuf->len)) || (pval == NULL)){
		//vd_log.erro("push bytes fail! len=%d, cycbuf->len=%d\r\n",len, cycbuf->len);
		return -1;	
	}
	
	for(index = 0; index < len; index++){
		cycle_buff_push_byte(cycbuf, pval[index]);
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
int8_t cycle_buff_pull_bytes(cycbuf_t* cycbuf, uint8_t* pval, uint8_t len)
{
	uint8_t index = 0;
	
	if(len > cycbuf->len || (pval == NULL) || (cycbuf == NULL)){
		//vd_log.erro("pull bytes fail! len= %d, cycbuf->len= %d\r\n",len, cycbuf->len);
		return -1;	
	}
	
	for(index = 0; index < len; index++){
		cycle_buff_pull_byte(cycbuf, pval++);
	}
	
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
uint8_t cycle_buff_peep_read_all_bytes(cycbuf_t* cycbuf, uint8_t* pval)
{
	if((pval == NULL) || (cycbuf == NULL)){
		//vd_log.erro("peep read all bytes fail! \r\n");
		return 1;	
	}	
	for(uint8_t i = 0; i< cycbuf->len; i++){
		pval[i] = cycbuf->buff[cycbuf->r_index + i];
	}
	
	return cycbuf->len;
}



/*
************************************************************
*	�������ƣ�	cycle_buff_clear
*	�������ܣ�	buff�Ƿ�Ϊ��
*	��ڲ�����	cycbuf:����buff�ṹ��
*	���ز�����	0���գ� >0���ǿ�
************************************************************
*/
void cycle_buff_clear(cycbuf_t* cycbuf)
{
	cycbuf->r_index = cycbuf->w_index;
	cycbuf->len = 0;
}
