#include "filter_alg.h"
#include "base_type.h"
#include "log.h"

//�ο����ϣ�https://blog.csdn.net/qq_46336588/article/details/120272707

#if USE_MOVE_FILTER 
/*
************************************************************
*	�������ƣ�	moveAverageFilter
*	�������ܣ�	����ƽ���˲���
*	��ڲ�����	��
*	���ز�����	�˲����ֵ
*   �ŵ㣺�������Ը��������õ��������ã�ƽ���ȸߣ������ڸ�Ƶ�񵴵�ϵͳ
*   ȱ�㣺�����ȵͣ���żȻ���ֵ������Ը��ŵ��������ýϲ������������Ž����صĳ���
************************************************************
*/
int16_t moveAverageFilter(move_filter_t* move_filter, int16_t data)
{
	int16_t ret_value = 0;
	
	if(move_filter == NULL){
		goto erro;
	}
		
    if(move_filter->curNum < FILTER_COUNT){
		move_filter->buf[move_filter->curNum] = data;		
        move_filter->sum += data;
		move_filter->curNum++;	  
        ret_value = (move_filter->sum) / (move_filter->curNum);
		
    }else{
		move_filter->sum -= (move_filter->sum) / FILTER_COUNT;
        move_filter->sum += data;
        ret_value = (move_filter->sum) / FILTER_COUNT;
    }

erro:
	return ret_value;
	
} 
#endif


