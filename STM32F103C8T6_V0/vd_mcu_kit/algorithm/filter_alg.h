#ifndef __FILTER_ALG_H
#define __FILTER_ALG_H

#include "stm32f10x.h"

#define USE_MOVE_FILTER   1          //滑动滤波，0：不使用，1：使用



//滑动滤波
#if USE_MOVE_FILTER 
	#define FILTER_COUNT     10      //滤波次数
	typedef struct {
		int16_t buf[FILTER_COUNT];   //滤波buf
		int32_t sum;                 //保存滤波值和
		int16_t curNum; 			 //第一次记录使用
	}move_filter_t;
#endif



#endif
