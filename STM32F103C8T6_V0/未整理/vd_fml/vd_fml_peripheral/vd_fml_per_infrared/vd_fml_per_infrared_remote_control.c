/*
 * Project: mcu-kit
 * Module: module_name
 * File: vd_fml_per_infrared_remote_control.c
 * Created Date: 2023��2��13��10:13:44
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
#if (DRIVER_FML_TIME_EN == ON) //ģ��֮�以�����ֱ�Ӽ���ʹ�ö�Ӧģ��ͷ�ļ�
#include "time/hrd_time_module.h"
#endif
#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "vd_log.h"

/*==============����app���������ͷ�ļ�================*/
//#include "app_manager.h"
//#include "control_server.h"

/* ======================================================================================
 * types
 */

volatile vd_uint16_t IR_Data=0;	//��ȡ���ĺ�������
vd_uint8_t IR_Address;	//��ȡ���ĺ����ַ
vd_uint8_t IR_Command;	//��ȡ���ĺ����ֵ

/**********����appʱ��ʹ������=========================================
extern volatile vd_uint8_t vd_fml_per_infrared_remote_control_data_flag;	//����ң�����ݻ�ȡ�ɹ���־λ
extern volatile vd_uint16_t vd_fml_per_infrared_remote_control_high_level_flag;
extern volatile vd_uint16_t vd_fml_per_infrared_remote_control_low_level_flag;
extern volatile vd_uint16_t data_buf , data_cnt;
extern vd_uint8_t signal_flag;
*/

/*======================δ����app ȷ��ģ����벻����=====================================*/
 volatile vd_uint8_t vd_fml_per_infrared_remote_control_data_flag;	//����ң�����ݻ�ȡ�ɹ���־λ
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
 * @brief : ����ģ���ʼ��
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_infrared_remote_control_init(vd_void_t)
{
		vd_uint8_t ret = vd_false;
		//io�ڳ�ʼ��
		ret = vd_bsp_gpio_init(VD_FML_PER_INFRARED_REMOTE_CONTROL_PIN,VD_GPIO_MODE_INPUT);
//		ret = vd_bsp_gpio_requst_irq(vd_bsp_gpio.pin, DRV_GPIO_MODE_EXTI_IT_FALLING, vd_fml_per_infrared_remote_control_pin_isr);

		return ret;
}

uint8_t test_flag = 0;
/**
 * @brief : ����ģ��ң�������ݻ�ȡ (Ŀǰ���ж������ǻ��ڶ�ʱ��20us����)
 *
 * @return vd_bool_t
 */
vd_void_t vd_fml_per_infrared_remote_control_data_get(vd_void_t){
	if(vd_bsp_gpio_read(VD_FML_PER_INFRARED_REMOTE_CONTROL_PIN) == 1){ //����ȡ�ĵ�ƽΪ�ߵ�ƽʱ
		vd_fml_per_infrared_remote_control_high_level_flag++; //�ߵ�ƽ�����ۼ�
		//���ߵ�ƽΪ1.4ms~1.8msʱ
		if(vd_fml_per_infrared_remote_control_high_level_flag>80 && vd_fml_per_infrared_remote_control_high_level_flag< 100){
			vd_fml_per_infrared_remote_control_high_level_flag =100;//�ߵ�ƽ����ֵ��Ϊ90
			data_buf = 0; //������ݵ�buffer����
			data_cnt = 0; //һ֡�����е����ݴ�������
		}
		if(vd_fml_per_infrared_remote_control_high_level_flag==500){//���ߵ�ƽ����ֵ�ﵽ500�����ߵ�ƽ����ʱ��ﵽ10msʱ
//			signal_flag = 1;//���ΰ����еĿ���ʱ�ڣ����ϱ������źŵı�־λ��Ϊ1
			IR_Data = 0;
		}
		if(vd_fml_per_infrared_remote_control_high_level_flag>1000){
			vd_fml_per_infrared_remote_control_high_level_flag = 600;
		}
		//���͵�ƽΪ1ms~1.36msʱ
		if(vd_fml_per_infrared_remote_control_low_level_flag>50 && vd_fml_per_infrared_remote_control_low_level_flag <80)
		{
			data_buf |= 1<<data_cnt;  //��λ����Ϊ1����������buffer
			data_cnt++;	 //λ������
			vd_fml_per_infrared_remote_control_low_level_flag = 0;//�͵�ƽ��������
		}
		else if(vd_fml_per_infrared_remote_control_low_level_flag>18 && vd_fml_per_infrared_remote_control_low_level_flag <45)		//���͵�ƽΪ320us~800usʱ
		{
			data_cnt++;	 //����Ĭ��Ϊ0��������Ϊ0ʱ��ֻ��λ������
			vd_fml_per_infrared_remote_control_low_level_flag = 0;//�͵�ƽ��������
		}
		else{
			vd_fml_per_infrared_remote_control_low_level_flag = 0;
		}
		if((data_cnt >=12 && IR_Data !=data_buf)){	//���ϱ������źŵı�־λ��Ϊ1����һ��12λ�����ݶ���ʱ signal_flag && IR_Data_recive_flag == 0 && 
			vd_fml_per_infrared_remote_control_data_flag = 1;	//����ң�����ݻ�ȡ�ɹ���־λ��Ϊ1
			
			IR_Data = data_buf;	//��data_buf������ݴ���IR_Data
			data_buf = 0; //������ݵ�buffer����
			data_cnt = 0; //һ֡�����е����ݴ�������
//			signal_flag = 0; //�ϱ������źŵı�־λ��Ϊ0
		}
	}else{	
		vd_fml_per_infrared_remote_control_low_level_flag++;	//�͵�ƽ�����ۼ�
		if(vd_fml_per_infrared_remote_control_low_level_flag>18){
			vd_fml_per_infrared_remote_control_high_level_flag = 0; //�ߵ�ƽ��������
		}
		test_flag = 1;
	}
}


/**
 * @brief : ����ģ��ң��������ʶ��
 *
 * @return vd_bool_t
 */
vd_bool_t vd_fml_per_infrared_remote_control_detect(vd_void_t){
//	if(if_over_flag == 1){
//		if_over_flag = 0;
//		printf("ir_data = %x,ir_rec_cnt = %d\r",ir_buf_data,ir_rec_cnt);
//	}
	vd_bool_t   ret = vd_false;
	if(vd_fml_per_infrared_remote_control_data_flag){ //����ң�����ݻ�ȡ�ɹ�
		log_debug("IR_Data = %x\n",IR_Data);
		vd_fml_per_infrared_remote_control_data_flag = 0;	//��־λ����
		IR_Address = IR_Data&0x000f;	//�����ַ��ȡ
		IR_Command = (IR_Data&0x0ff0)>>4;	//�����ֵ��ȡ
		if(IR_Address == 0x0b){	//�����ַ��ȷΪ0b
//			log_debug("IR_Address = 0b is right!!!\n");
			vd_fml_per_infrared_remote_control_refresh_flag = 1;
			ret = vd_true;
			switch (IR_Command){
				case 0x81: //��Դ
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_POWER;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0xc3:	//ȡˮ����
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_GET_WATER;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0x41:	//���Ȱ���
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_HEAT;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0x11:	//���°���
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_ATTEMPERATION;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0x21:	//���°���
					vd_fml_per_infrared_key = VD_FML_PER_INFRARED_KEEP_WARM;
//					log_debug("IR_Command = %x\n",IR_Command);
					break;
				case 0x09:	//ȡ��ˮ����
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
 *vd_fml_per_infrared_remote_control_loopִ�к���
 */
vd_void_t vd_fml_per_infrared_remote_control_loop(vd_void_t)
{
	//hrd_infrared_module_dust_detection_factory(); //����ģʽ���
	vd_fml_per_infrared_remote_control_detect();
}
/*
infrared module���Դ���
*/
#if (DRIVER_FML_INFRARED_TEST_EN == ON)
extern vd_uint8_t bsp_test_value;
vd_uint8_t vd_fml_per_infrared_remote_control_test(vd_void_t)
{

	return 1;
}
#endif
#endif
