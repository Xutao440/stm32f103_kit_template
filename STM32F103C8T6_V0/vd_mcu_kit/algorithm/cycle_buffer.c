/* 环形缓冲区cycle buffer */
#include "cycle_buffer.h"
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
*	函数名称：	creat_cycle_buff
*	函数功能：	创建循环buff
*	入口参数：	size：大小
*	返回参数：	成功返回0， 失败返回大于0
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
*	函数名称：	is_cycle_buff_empty
*	函数功能：	buff是否为空
*	入口参数：	cycbuf:环形buff结构体
*	返回参数：	0：空， >0：非空
************************************************************
*/
int is_cycle_buff_empty(cycbuf_t* cycbuf)
{
	return cycbuf->r_index == cycbuf->w_index;
}

/*
************************************************************
*	函数名称：	is_cycle_buff_full
*	函数功能：	buff是否为满
*	入口参数：	cycbuf:环形buff结构体
*	返回参数：	0：满， >0：不满
************************************************************
*/
int is_cycle_buff_full(cycbuf_t* cycbuf)
{
	return NEXT_PLACE(cycbuf->w_index, cycbuf->size) == cycbuf->r_index;
}

/*
************************************************************
*	函数名称：	cycle_buff_put
*	函数功能：	buff是否为空
*	入口参数：	cycbuf:环形buff结构体
*	返回参数：	0：空， >0：非空
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
*	函数名称：	cycle_buff_get
*	函数功能：	buff是否为空
*	入口参数：	cycbuf:环形buff结构体
*	返回参数：	0：空， >0：非空
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
*	函数名称：	is_cycle_buff_empty
*	函数功能：	buff是否为空
*	入口参数：	cycbuf:环形buff结构体
*	返回参数：	0：空， >0：非空
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
*	函数名称：	is_cycle_buff_empty
*	函数功能：	buff是否为空
*	入口参数：	cycbuf:环形buff结构体
*	返回参数：	0：空， >0：非空
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
*	函数名称：	is_cycle_buff_empty
*	函数功能：	buff是否为空
*	入口参数：	cycbuf:环形buff结构体
*	返回参数：	0：空， >0：非空
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
*	函数名称：	cycle_buff_clear
*	函数功能：	buff是否为空
*	入口参数：	cycbuf:环形buff结构体
*	返回参数：	0：空， >0：非空
************************************************************
*/
void cycle_buff_clear(cycbuf_t* cycbuf)
{
	cycbuf->r_index = cycbuf->w_index;
	cycbuf->len = 0;
}
