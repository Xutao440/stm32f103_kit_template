/*
 * Project: WH001
 * Module: 
 * File: ntc_table.c
 * Created Date: 2022-11-25 16:57:55
 * Author: your_name
 * Description: description
 * -----
 * todo: modified 
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)

/* ======================================================================================
 * includes
 */
#include "ntc_table.h"

/* ======================================================================================
 * macros
 */
const unsigned int ntc_table[NUM] ={//温度和adc对应表
    17,	    18,	    19,	    21,	    22,	    23,	    24,	    26,	    27,	    29,	    //-50~ -41	     
    31,	    32,	    34,	    36,	    39,	    41,	    44,	    47,	    50,	    53,	    //-40~ -31
    56,	    59,	    63,	    66,	    70,	    74,	    78,	    83,	    87,	    92,	    //-50~ -21	
    97,	    102,	107,	113,	119,	125,	131,	137,	144,	151,	//-20~ -11	
    158,	165,	173,	181,	189,	197,	205,	214,	223,	232,	//-10~ -1	
    241,	250,	260,	270,	280,	290,	301,	311,	322,	333,	//0~ 9
    344,	355,	366,	377,	388,	400,	411,	423,	434,	446,	//10~ 19
    457,	469,	480,	491,	503,	512,	521,	532,	543,	554,	//20~ 29
    565,	576,	587,	597,	608,	621,	628,	638,	648,	658,	//30~ 39
    667,	677,	686,	695,	704,	712,	721,	729,	737,	745,	//40~ 49
    753,	760,	767,	775,	782,	788,	795,	693,	808,	814,	//50~ 59
    820,	826,	831,	837,	842,	847,	853,	857,	862,	867,	//60~ 69
    871,	876,	880,	884,	888,	892,	896,	899,	903,	906,	//70~ 79
    910,	913,	916,	919,	922,	926,	928,	930,	933,	936,	//80~ 89
    938,	940,	943,	945,	947,	949,	951,	953,	955,	957,	//90~ 99
    959,	961,	962,	964,	966,	967,	969,	970,	972,	973,	//100~ 109
    974,	976,	977,	978,	979,	981,	982,	983,	984,	985,	//110~ 119
    986,	987,	988,	989,	990,	990,	991,	992,	993,	994,	//120~ 129
    994,	995,	996,	997,	997,	998,	999,	999,	1000,	1000,	//130~ 139
    1001,	1002,	1002,	1003,	1003,	1004,	1004,	1005,	1005,	1006,	//140~ 149
    1006,	1006,	1007,	1006,	1007,	1008,	1008,	1008,	1009,	1009,	//150~ 159
    1009,	1010,	1009,	1010,	1011,	1011,	1011,	1011,	1012,	1012,	//160~ 169
    1012,	1012,	1013,	1013,	1013,	1013,	1013,	1014,	1014,	1014,	//170~ 179
    1014,	1014,	1015,	1015,	1015,	1015,	1015,	1015,	1016,	1016,	//180~ 189
    1016,	1016,	1016,	1016,	1014,	1017,	1017,	1017,	1017,	1017,	//190~ 199
    1017,

};

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/**
 * @brief adc转换成温度
 * 
 * @param r_adc 
 * @return float 
 */
vd_int_t read_ntc_temperature(unsigned int r_adc)
{
	vd_int32_t middle; //存每次的中间点
	vd_int32_t right; //终点
	vd_int32_t left; //起点
	vd_float_t k, j, fraction;
	right = (sizeof(ntc_table) / sizeof(ntc_table[0]) - 1);
	// co_printf("right = %d\n",right);
	left = 0;
	middle = (right - left) / 2;
	// co_printf("middle = %d\n",middle);
	while(1)
	{
	    // co_printf("r_adc=%d\n",r_adc);
		if(r_adc > ntc_table[middle])
		{
			if(r_adc <= ntc_table[middle + 1]) //找到了
			{
				k = ntc_table[middle + 1] - ntc_table[middle];
				j = r_adc - ntc_table[middle];
				fraction = j / k;
                // co_printf("1\n");
				return (middle - 72 + fraction);
                
			}
			else //没找到
			{
				left = middle;
                // co_printf("2\n");
			}
		}
		else if(r_adc < ntc_table[middle])
		{	
			if(r_adc >= ntc_table[middle - 1]) //找到了
			{
				k = ntc_table[middle] - ntc_table[middle - 1];
				j = r_adc - ntc_table[middle - 1];
				fraction = j / k;
                // co_printf("3\n");
				return (middle - 73 + fraction);
			}
			else //没找到
			{
				if(r_adc < 17)
				{
                    // co_printf("4\n");
					return 0;
				}
				else
					right = middle;
			}
		}
		else if(r_adc == ntc_table[middle]) //找到了
		{
            // co_printf("5\n");
			return (middle - 72);
		}
		middle = (right - left) / 2;
		middle = left + middle; //相当重要，光除2不是中间点，加上起点之后才是中间点
	}	
}
/* ======================================================================================
 * implementation
 */
