/*
 * Project: mcu-kit
 * Module: module_name
 * File: vd_fml_per_infrared_remote_control.c
 * Created Date: 2023年2月13日10:13:44
 * Author: hjl
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
#include "vd_fml_per_infrared_remote_control.h"
#if (PROJECT_DRIVER_FML_INFRARED_EN == ON)
/* ======================================================================================
 * macros
 */
//#include "kit_driver_bsp.h"
#include "fsm_common.h"
#if (DRIVER_FML_TIME_EN == ON) //模块之间互相调用直接加载使用对应模块头文件
#include "time/hrd_time_module.h"
#endif
#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "vd_log.h"

/*==============加入app后包含以下头文件================*/
//#include "app_manager.h"
//#include "control_server.h"

/* ======================================================================================
 * types
 */

volatile vd_uint16_t IR_Data=0;	//获取到的红外数据
vd_uint8_t IR_Address;	//获取到的红外地址
vd_uint8_t IR_Command;	//获取到的红外键值

/**********加入app时，使用以下=========================================
extern volatile vd_uint8_t vd_fml_per_infrared_remote_control_data_flag;	//红外遥控数据获取成功标志位
extern volatile vd_uint16_t vd_fml_per_infrared_remote_control_high_level_flag;
extern volatile vd_uint16_t vd_fml_per_infrared_remote_control_low_level_flag;
extern volatile vd_uint16_t data_buf , data_cnt;
extern vd_uint8_t signal_flag;
*/

/*======================未加入app 确保模块编译不报错=====================================*/
 volatile vd_uint8_t vd_fml_per_infrared_remote_control_data_flag;	//红外遥控数据获取成功标志位
 volatile vd_uint16_t vd_fml_per_infrared_remote_control_high_level_flag;
 volatile vd_uint16_t vd_fml_per_infrared_remote_control_low_level_flag;
 volatile vd_uint16_t data_buf , data_cnt;
 vd_uint8_t signal_flag;
/*========================================================================================*/

vd_uint8_t IR_Data_flag = 0;
vd_uint8_t vd_fml_per_infrared_remote_control_pin_isr_flag = 0;
vd_uint8_t vd_fml_per_infrared_remote_control_pin_isr_cnt = 0;
vd_uint8_t vd_fml_per_infrared_remote_control_refresh_flag = 0;

vd_fml_per_infrared_key_type_t vd_fml_per_infrared_key;

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
 * @brief : 红外模块初始化
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_infrared_remote_control_init(vd_void_t)
{
		vd_uint8_t ret = vd_false;
		//io口初始化
		ret = vd_bsp_gpio_init(VD_FML_PER_INFRARED_REMOTE_CONTROL_PIN,VD_GPIO_MODE_INPUT);
//		ret = vd_bsp_gpio_requst_irq(vd_bsp_gpio.pin, DRV_GPIO_MODE_EXTI_IT_FALLING, vd_fml_per_infrared_remote_control_pin_isr);

		return ret;
}

uint8_t test_flag = 0;
/**
 * @brief : 红外模块遥控器数据获取 (目前的判断条件是基于定时器20us设置)
 *
 * @return vd_bool_t
 */
vd_void_t vd_fml_per_infrared_remote_control_data_get(vd_void_t){
	if(vd_bsp_gpio_read(VD_FML_PER_INFRARED_REMOTE_CONTROL_PIN) == 1){ //当读取的电平为高电平时
		vd_fml_per_infrared_remote_control_high_level_flag++; //高电平计数累加
		//当高电平为1.4ms~1.8ms时
		if(vd_fml_per_infrared_remote_control_high_level_flag>80 && vd_fml_per_infrared_remote_control_high_level_flag< 100){
			vd_fml_per_infrared_remote_control_high_level_flag =100;//高电平计数值置为90
			data_buf = 0; //存放数据的buffer清零
			data_cnt = 0; //一帧数据中的数据次数清零
		}
		if(vd_fml_per_infrared_remote_control_high_level_flag==500){//当高电平计数值达到500，及高电平持续时间达到10ms时
//			signal_flag = 1;//两次按键中的空闲时期，将上报按键信号的标志位置为1
			IR_Data = 0;
		}
		if(vd_fml_per_infrared_remote_control_high_level_flag>1000){
			vd_fml_per_infrared_remote_control_high_level_flag = 600;
		}
		//当低电平为1ms~1.36ms时
		if(vd_fml_per_infrared_remote_control_low_level_flag>50 && vd_fml_per_infrared_remote_control_low_level_flag <80)
		{
			data_buf |= 1<<data_cnt;  //该位数据为1，存入数据buffer
			data_cnt++;	 //位数增加
			vd_fml_per_infrared_remote_control_low_level_flag = 0;//低电平计数清零
		}
		else if(vd_fml_per_infrared_remote_control_low_level_flag>18 && vd_fml_per_infrared_remote_control_low_level_flag <45)		//当低电平为320us~800us时
		{
			data_cnt++;	 //由于默认为0，当数据为0时，只需位数增加
			vd_fml_per_infrared_remote_control_low_level_flag = 0;//低电平计数清零
		}
		else{
			vd_fml_per_infrared_remote_control_low_level_flag = 0;
		}
		if((data_cnt >=12 && IR_Data !=data_buf)){	//当上报按键信号的标志位置为1，且一组12位的数据读到时 signal_flag && IR_Data_recive_flag == 0 && 
			vd_fml_per_infrared_remote_control_data_flag = 1;	//红外遥控数据获取成功标志位置为1
			
			IR_Data = data_buf;	//将data_buf里的数据传给IR_Data
			data_buf = 0; //存放数据的buffer清零
			data_cnt = 0; //一帧数据中的数据次数清零
//			signal_flag = 0; //上报按键信号的标志位置为0
		}
	}else{	
		vd_fml_per_infrared_remote_control_low_level_flag++;	//低电平计数累加
		if(vd_fml_per_infrared_remote_control_low_level_flag>18){
			vd_fml_per_infrared_remote_control_high_level_flag = 0; //高电平计数清零
		}
		test_flag = 1;
	}
}


/**
 * @brief : 红外模块遥控器按键识别
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_infrared_remote_control_detect(vd_void_t){
//	if(if_over_flag == 1){
//		if_over_flag = 0;
//		printf("ir_data = %x,ir_rec_cnt = %d\r",ir_buf_data,ir_rec_cnt);
//	}
	vd_bool_t   ret = vd_false;
	if(vd_fml_per_infrared_remote_control_data_flag){ //红外遥控数据获取成功
		log_debug("IR_Data = %x\n",IR_Data);
		vd_fml_per_infrared_remote_control_data_flag = 0;	//标志位置零
		IR_Address = IR_Data&0x000f;	//红外地址获取
		IR_Command = (IR_Data&0x0ff0)>>4;	//红外键值获取
		if(IR_Address == 0x0b){	//红外地址正确为0b
//			log_debug("IR_Address = 0b is right!!!\n");
			vd_fml_per_infrared_remote_control_refresh_flag = 1;
			ret = vd_true;
			switch (IR_Command){
				case 0x81: //电源
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_POWER;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0xc3:	//取水按键
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_GET_WATER;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0x41:	//加热按键
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_HEAT;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0x11:	//调温按键
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_ATTEMPERATION;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0x21:	//保温按键
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_KEEP_WARM;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0x09:	//取冷水按键
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_GET_COLD_WATER;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				default:
					ret = vd_false;
					log_debug("IR_Command is wrong\n");
					break;
			}
		}
		return ret;
//		vd_fml_per_infrared_remote_control_detect_handle(vd_fml_per_infrared_key);
	}
//	if(test_flag && !vd_fml_per_infrared_remote_control_data_flag){
//		test_flag = 0;
//		log_debug("vd_fml_per_infrared_remote_control_low_level_flag = %d\n",vd_fml_per_infrared_remote_control_low_level_flag);
//		log_debug("vd_fml_per_infrared_remote_control_high_level_flag = %d\n",vd_fml_per_infrared_remote_control_high_level_flag);
//	}
}

/*
 *vd_fml_per_infrared_remote_control_loop执行函数
 */
vd_void_t vd_fml_per_infrared_remote_control_loop(vd_void_t)
{
	//hrd_infrared_module_dust_detection_factory(); //工厂模式检测
	vd_fml_per_infrared_remote_control_detect();
}
/*
infrared module测试代码
*/
#if (DRIVER_FML_INFRARED_TEST_EN == ON)
extern vd_uint8_t bsp_test_value;
vd_uint8_t vd_fml_per_infrared_remote_control_test(vd_void_t)
{

	return 1;
}
#endif
#endif
