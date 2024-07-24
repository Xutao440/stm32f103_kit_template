#ifndef __FILTER_ALG_H
#define __FILTER_ALG_H

#include "stm32f10x.h"

#define USE_MOVE_FILTER   1          //�����˲���0����ʹ�ã�1��ʹ��



//�����˲�
#if USE_MOVE_FILTER 
	#define FILTER_COUNT     10      //�˲�����
	typedef struct {
		int16_t buf[FILTER_COUNT];   //�˲�buf
		int32_t sum;                 //�����˲�ֵ��
		int16_t curNum; 			 //��һ�μ�¼ʹ��
	}move_filter_t;
#endif



#endif
