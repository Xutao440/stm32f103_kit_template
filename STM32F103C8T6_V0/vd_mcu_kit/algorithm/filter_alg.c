#include "filter_alg.h"
#include "base_type.h"
#include "log.h"

//参考资料：https://blog.csdn.net/qq_46336588/article/details/120272707

#if USE_MOVE_FILTER 
/*
************************************************************
*	函数名称：	moveAverageFilter
*	函数功能：	滑动平均滤波法
*	入口参数：	无
*	返回参数：	滤波后的值
*   优点：对周期性干扰有良好的抑制作用，平滑度高；试用于高频振荡的系统
*   缺点：灵敏度低；对偶然出现的脉冲性干扰的抑制作用较差，不适于脉冲干扰较严重的场合
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


