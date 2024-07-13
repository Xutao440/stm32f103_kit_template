/*
 * Project: fr8000
 * Module: lock
 * File: drv_lock.c
 * Created Date: 2023-03-02 11:21:41
 * Author: liaozewei
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2023 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)


/* ======================================================================================
 * includes
 */
#include "vd_fml_per_limit.h"
//#include "machine_pr015.h"
#include "os_timer.h"
#include "system_config.h"


#include "driver_gpio.h"



#include "vd_fml_per_nfc/vd_fml_per_nfc.h"
extern config_table_t config_table;

#include "vd_bsp_flash/vd_bsp_flash.h"
#define NFC_AND_CONFIG_INFO_ADDR 0xFE000  // nfc��Ϣ��һ��ͨ��Ϣ����ѹ������

/* ======================================================================================
 * macros
 */
// �궨��λ��

/* ======================================================================================
 * types
 */
// ���Ͷ���λ��

/* ======================================================================================
 * declaration
 */
// ��������λ��

/* ======================================================================================
 * globals
 */
// ȫ�ֱ���λ��
static vd_uint8_t drv_speed_limit_state=0x02;//0x01������״̬  0x02����״̬
static vd_uint32_t  drv_speed_limit_ret=0;
static vd_uint8_t drv_speed_limit_cnt=0;
static vd_uint8_t drv_speed_limit_trigger_flag=0;
static vd_uint8_t drv_speed_limit_valus=0x00;//
static vd_uint8_t drv_speed_limit_sendcnt=10;//Ĭ�Ϸ�10��
static vd_uint8_t drv_speed_limit_once_flag = 0;

/* ======================================================================================
 * helper
 */
// ��������λ��



/**
 * @brief bit��ʼ�ź�
 * @param 
 * @return vd_uint8_t
 */
/****************bitֵ����ʼ�ź�*******************/
vd_uint8_t mcu_bit_begin(void)//�ߣ�1ms   �ͣ�3ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<40)) //1ms
	 {
	   //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//�� 2023-3-2 todo
	   gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//����������ٷ���     ��
	 }

   if(bit_cnt>=40)
	 {
	   bit_cnt=0;
		 return 1;
		 
	 }
	  return 0;
}
	


/*****************************************/




/**
 * @brief bitֵ��1
 * @param 
 * @return vd_uint8_t
 */
/****************bitֵ��1*******************/
vd_uint8_t mcu_bit_1(void)//�ߣ�3ms   �ͣ�1ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<30)) //3ms
	 {
	   //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//��  todo
	    gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//����������ٷ���     ��
	 }

   if((bit_cnt>=30)&&(bit_cnt<40)) //1ms
	 {
	 	 //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//��  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//����������ٷ���     ��
	 }
	
   if(bit_cnt>=40)
	 {
	   bit_cnt=0;
		 return 1;
		 
	 }
	  return 0;
}
	


vd_uint8_t mcu_bit_t_1(void)//�ߣ�3ms   �ͣ�1ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<20)) //2ms
	 {
	   //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//��  todo
	    gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//����������ٷ���     ��
	 }

   if((bit_cnt>=20)&&(bit_cnt<30)) //1ms
	 {
	 	 //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//��  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//����������ٷ���     ��
	 }
	
   if(bit_cnt>=30)
	 {
	   bit_cnt=0;
		 return 1;
		 
	 }
	  return 0;
}





/*****************************************/

/**
 * @brief bitֵ��0
 * @param 
 * @return vd_uint8_t
 */
/****************bitֵ��0*******************/
vd_uint8_t mcu_bit_0(void)//�ߣ�1ms   �ͣ�3ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<10)) //1ms
	 {
	  // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//��  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//����������ٷ���     ��
	 }

   if((bit_cnt>=10)&&(bit_cnt<40)) //3ms
	 { 
	 	// mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//��  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//����������ٷ���     ��
	 }
	
   if(bit_cnt>=40)
	 {
	   bit_cnt=0;
		 return 1;
		 
	 }
	  return 0;
}





vd_uint8_t mcu_bit_t_0(void)//�ߣ�1ms   �ͣ�3ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<10)) //1ms
	 {
	  // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//��  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//����������ٷ���     ��
	 }

   if((bit_cnt>=10)&&(bit_cnt<30)) //2ms
	 { 
	 	// mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//��  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//����������ٷ���     ��
	 }
	
   if(bit_cnt>=30)
	 {
	   bit_cnt=0;
		 return 1;
		 
	 }
	  return 0;
}








/************************************************************************************************/

/**
 * @brief bitֵ�������ź�
 * @param 
 * @return vd_uint8_t
 */
/****************bitֵ�������ź�*******************/
vd_uint8_t mcu_bit_end(void)//�ߣ�1ms   �ͣ�3ms
{
	   //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//��  todo
	    gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//����������ٷ���     ��
		 return 1; 
}
	

vd_uint8_t mcu_bit_end_t(void)//�ߣ�1ms   �ͣ�3ms
{
		static vd_uint16_t bit_cnt=0;
	  bit_cnt++;
	
	 if((bit_cnt>=0)&&(bit_cnt<10)) //1ms
	 {
	  // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//��  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//����������ٷ���     ��
	 }
	 
   if((bit_cnt>=10)&&(bit_cnt<260)) //2ms
	 { 
	 	// mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//��  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//����������ٷ���     ��
	 }	 

	 
	 if(bit_cnt>=260)
	 {
		 bit_cnt=0;
	   return 1; 
	 }
		 return 0; 
}

/*****************************************/



/* ======================================================================================
 * private implementation
 */
// ˽�к���ʵ��λ��



/* ======================================================================================
 * implementation
 */
// ����ʵ��λ��





/**
 * @brief ���ٲ���
* @param  data�����͵�����
 * @return vd_uint8_t
 */
/***********************************����*****************************************/

vd_uint8_t mcu_speed_limit_info(vd_uint8_t data)
{
 	vd_uint8_t ret=0;
  static   vd_int16_t bit=7;	
	static vd_uint8_t step=0;

  static vd_uint8_t cnt=0;
	
	if(step==0)
	{
	  ret=mcu_bit_begin();
	  if(1==ret)
		{
		  step=1;
		
		}
	}
	
	else if(1==step)
	{
	     if(1==((data>>bit)&0x01))
	     {
	        ret= mcu_bit_1();
	     	 if(1==ret)
	     	 {
	     	   bit=bit-1;
	     	 }
	     }
	     else if(0==((data>>bit)&0x01))
	     {
	        ret= mcu_bit_0();
	     	 if(1==ret)
	     	 {
	     	   bit=bit-1;
	     	 }	
	     
	     }
			 //printf(" bit=%d\r\n",bit);
			if(-1==bit)
			{
			  step=2;
				bit=7;
			//printf("step=%d bit=%d\r\n",step,bit);
			}
  }
	
	else if(2==step)
	{
	     if(1==((data>>bit)&0x01))
	     {
	        ret= mcu_bit_1();
	     	 if(1==ret)
	     	 {
	     	   bit=bit-1;
	     	 }
	     }
	     else if(0==((data>>bit)&0x01))
	     {
	        ret= mcu_bit_0();
	     	 if(1==ret)
	     	 {
	     	   bit=bit-1;
	     	 }	
	     
	     }
			 
			if(-1==bit)
			{
			  step=3;
				bit=7;
			
			}	
	
	}
	     
	     
	else if(3==step)//����
	{
	     	//step=0;
	      mcu_bit_end();//�����ź�
	       //��һ����־λ������
		//printf("xiansu end");
		
		#if 1
		cnt++;
		if(cnt==250)
		{
		  	step=0;
		    cnt=0;
			   return 1;
		}
		#endif
		  // return 1;
  }
	
	return 0;
}



vd_uint8_t mcu_speed_limit_info_t(vd_uint8_t data)
{
 	vd_uint8_t ret=0;
  static   vd_int16_t bit=7;	
	static vd_uint8_t step=0;

  static vd_uint8_t cnt=0;
	
	if(step==0)
	{
//	  ret=mcu_bit_begin();
//	  if(1==ret)
//		{
		  step=1;
//		
//		}
	}
	
	else if(1==step)
	{
	     if(1==((data>>bit)&0x01))
	     {
	        ret= mcu_bit_t_1();
	     	 if(1==ret)
	     	 {
	     	   bit=bit-1;
	     	 }
	     }
	     else if(0==((data>>bit)&0x01))
	     {
	        ret= mcu_bit_t_0();
	     	 if(1==ret)
	     	 {
	     	   bit=bit-1;
	     	 }	
	     
	     }
			 //printf(" bit=%d\r\n",bit);
			if(-1==bit)
			{
			  step=2;
				bit=7;
			//printf("step=%d bit=%d\r\n",step,bit);
			}
  }
	
	else if(2==step)
	{
	     if(1==((data>>bit)&0x01))
	     {
	        ret= mcu_bit_t_1();
	     	 if(1==ret)
	     	 {
	     	   bit=bit-1;
	     	 }
	     }
	     else if(0==((data>>bit)&0x01))
	     {
	        ret= mcu_bit_t_0();
	     	 if(1==ret)
	     	 {
	     	   bit=bit-1;
	     	 }	
	     
	     }
			 
			if(-1==bit)
			{
			  step=3;
				bit=7;
			
			}	
	
	}
	     
	     
	else if(3==step)//����
	{
	     	//step=0;
	    ret=  mcu_bit_end_t();//�����ź�
	       //��һ����־λ������
		//printf("xiansu end");
		  if(ret==1)
			{
		  	step=0;
		    //cnt=0;
			   return 1;			
			}
		#if 0
		cnt++;
		if(cnt==250)
		{
		  	step=0;
		    cnt=0;
			   return 1;
		}
		#endif
		  // return 1;
  }
	
	return 0;
}
/****************************************************************************/

/**
 * @brief ��������
* @param  
 * @return vd_void_t
 */
/************************��������********************************/
vd_void_t mcu_limit_drive(void)
{

	#if 1//���� 2022-10-24
		if((drv_speed_limit_state==0x02)&&(drv_speed_limit_trigger_flag==1))
	  {
	   // mcu_speed_limit_flag=2;
			if(drv_speed_limit_sendcnt>0)
			{
	     drv_speed_limit_ret=mcu_speed_limit_info(drv_speed_limit_valus);
			}
			
			 if(drv_speed_limit_ret==1)
	     {
//			   mcu_speed_limit_flag=0;
//			   speed_limit_new_flag=0;
	       drv_speed_limit_ret=0;
				 drv_speed_limit_cnt++;
	     }
			 
			 if(drv_speed_limit_cnt==drv_speed_limit_sendcnt)
			 {
			   drv_speed_limit_cnt=0;
			  // mcu_speed_limit_flag=0;
			   drv_speed_limit_trigger_flag=0;
	       drv_speed_limit_ret=0;			 
			 }			 
			 
    }
	
		if((drv_speed_limit_state==0x01)&&(drv_speed_limit_trigger_flag==1))
	  {
	     // mcu_speed_limit_flag=2;
			if(drv_speed_limit_sendcnt>0)
			{
	     drv_speed_limit_ret=mcu_speed_limit_info(drv_speed_limit_valus);
			} 

			 
			 
			 if(drv_speed_limit_ret==1)
	     {
//			   mcu_speed_limit_flag=0;
//			   speed_limit_new_flag=0;
         drv_speed_limit_ret=0;
				 drv_speed_limit_cnt++;
	     }
			 
			 if(drv_speed_limit_cnt==drv_speed_limit_sendcnt)
			 {
			   drv_speed_limit_cnt=0;
			  // mcu_speed_limit_flag=0;
			   drv_speed_limit_trigger_flag=0;
	       drv_speed_limit_ret=0;			 
			 }			 
			 
    }	
	#endif


		
		
		
	
	#if 1

		if((drv_speed_limit_state==0x02)&&(drv_speed_limit_trigger_flag==2))
	  {
	   // mcu_speed_limit_flag=2;
			 if(drv_speed_limit_sendcnt>0)
			 {
	       drv_speed_limit_ret=mcu_speed_limit_info_t(drv_speed_limit_valus);
			 }
			
	     if(drv_speed_limit_ret==1)
	     {
//			   mcu_speed_limit_flag=0;
//			   speed_limit_new_flag=0;
	       drv_speed_limit_ret=0;
				 drv_speed_limit_cnt++;
	     }
			 
			 if(drv_speed_limit_cnt==drv_speed_limit_sendcnt)
			 {
			   drv_speed_limit_cnt=0;
			  // mcu_speed_limit_flag=0;
			   drv_speed_limit_trigger_flag=0;
	       drv_speed_limit_ret=0;			 
			 }
    }		
		
		
		
		
		
		if((drv_speed_limit_state==0x01)&&(drv_speed_limit_trigger_flag==2))
	  {
	     // mcu_speed_limit_flag=2;
			 if(drv_speed_limit_sendcnt>0)
			 {
	      drv_speed_limit_ret=mcu_speed_limit_info_t(drv_speed_limit_valus);
			 }
			 
	     if(drv_speed_limit_ret==1)
	     {
//			   mcu_speed_limit_flag=0;
//			   speed_limit_new_flag=0;
         drv_speed_limit_ret=0;
				 drv_speed_limit_cnt++;
	     }
			 
			 if(drv_speed_limit_cnt==drv_speed_limit_sendcnt)
			 {
			   drv_speed_limit_cnt=0;
			  // mcu_speed_limit_flag=0;
			   drv_speed_limit_trigger_flag=0;
	       drv_speed_limit_ret=0;			 
			 }
    }	

  #endif			
		
		
		
		
		
		
		
		
		
		
		

}



/*************************************************************/



/**
 * @brief ���ٵ��ýӿ�
* @param   customer:�ͻ���1����  2���   limit_mode��0x01������״̬  0x02����״̬  limit_valus:���͵���ֵ  send_cnt�����Ͳ��δ�������ǰĬ��10��
 * @return vd_void_t
 */
/*****************************���ٽӿ�*************************************/
vd_void_t drv_limit_mode(vd_uint8_t customer ,vd_uint8_t limit_mode,vd_uint8_t limit_valus,vd_uint8_t send_cnt)
{
	
   drv_speed_limit_state=limit_mode;
//	 if(drv_speed_limit_state!= config_table.user_chose_info_.user_chose_info[2]){
//			config_table.user_chose_info_.user_chose_info[2] =drv_speed_limit_state;
//			drv_flash_save_to_temp(NFC_AND_CONFIG_INFO_ADDR, sizeof(config_table_t), (vd_uint8_t*)&config_table);
//	 }
	 drv_speed_limit_valus=limit_valus;

   drv_speed_limit_sendcnt=send_cnt;
   drv_speed_limit_trigger_flag=customer;

}



/**********************************************************************/



/**
 * @brief ���ٵ��ýӿ�
* @param  limit_mode��0x01������״̬  0x02����״̬  limit_valus:���͵���ֵ  send_cnt�����Ͳ��δ�������ǰĬ��10��
 * @return vd_void_t
 */

vd_uint8_t drv_get_limit_state(vd_void_t)
{
	
  return  drv_speed_limit_state;

}

/**
 * @brief �������ýӿ�
 * @param  state��0x01������״̬  0x02����״̬ 
 * @return vd_void_t
 */

vd_void_t drv_set_limit_state(vd_uint8_t state)
{	
		config_table.user_chose_info_.user_chose_info[2] = state;
//		drv_flash_save_to_temp(NFC_AND_CONFIG_INFO_ADDR, sizeof(config_table_t), (vd_uint8_t*)&config_table);
		vd_bsp_flash_write(NFC_AND_CONFIG_INFO_ADDR, (vd_uint8_t*)&config_table, sizeof(config_table_t));
		if(state == 2){
			drv_speed_limit_state =state;
		}else{
			drv_speed_limit_state =1;
		}
	  
}


/**
 * @brief �������ýӿ�
 * @param  state��0x01������״̬  0x02����״̬ 
 * @return vd_void_t
 */

vd_uint8_t drv_get_limit_value_idx(vd_void_t)
{	
	return config_table.user_chose_info_.user_chose_info[2]; 
}
/**
 * @brief ���ٲ������ݽӿ�
 * @param  limit_mode��0x01������״̬  0x02����״̬  limit_valus:���͵���ֵ  send_cnt�����Ͳ��δ�������ǰĬ��10��
 * @return vd_void_t
 */

vd_uint8_t drv_get_limit_valus(vd_uint8_t state,vd_uint8_t factory)
{	
	vd_uint8_t value = 0;
  switch (factory){
		case FACTORY_XI_TE:{
			if(state == 0x01){
				value  = 0x55;
			}else if(state == 0x02){
				value  = 0x5A;
			}else if(state == 0x03){
				value = 0xA0;
			}
		}
		break;
		case FACTORY_FENG_HUANG:{
			if(state == 0x01){
				value  = 0x5A;
			}else if(state == 0x02){
				value  = 0xA5;
			}else if(state == 0x03){
				value = 0x5A;
			}	
		}
		break;		
		default:
		break;
	}
	return value;
}

/**
 * @brief ��������ʹ�ܽӿ�
 * @param  en_type 0x00,NFC��ˢ��0x01��BLE����
 * @return vd_void_t
 */
vd_void_t drv_set_limit_en_type(vd_uint8_t en_type)
{	
	config_table.user_chose_info_.user_chose_info[1] = en_type;
//	drv_flash_save_to_temp(NFC_AND_CONFIG_INFO_ADDR, sizeof(config_table_t), (vd_uint8_t*)&config_table);
	vd_bsp_flash_write(NFC_AND_CONFIG_INFO_ADDR, (vd_uint8_t*)&config_table, sizeof(config_table_t));
}

/**
 * @brief ���ٻ�ȡʹ�ܽӿ�
 * @param  en_type 0x00,NFC��ˢ��0x01��BLE����
 * @return vd_uint8_t
 */
vd_uint8_t drv_get_limit_en_type(vd_void_t)
{	
	return config_table.user_chose_info_.user_chose_info[1];
}

/**
 * @brief ���ٻ�ȡִ��һ�νӿ�
 * @param  
 * @return vd_uint8_t 0���ѷ��ͣ�1��δ����
 */
vd_uint8_t drv_get_limit_once_flag(vd_void_t)
{	
	return config_table.user_chose_info_.user_chose_info[3];
}

/**
 * @brief ��������ִ��һ�νӿ�
 * @param  en_type 0x00,NFC��ˢ��0x01��BLE����
 * @return vd_uint8_t
 */
vd_void_t drv_set_limit_once_flag(vd_uint8_t flag)
{	
	config_table.user_chose_info_.user_chose_info[3] = flag;
//	drv_flash_save_to_temp(NFC_AND_CONFIG_INFO_ADDR, sizeof(config_table_t), (vd_uint8_t*)&config_table);
	vd_bsp_flash_write(NFC_AND_CONFIG_INFO_ADDR, (vd_uint8_t*)&config_table, sizeof(config_table_t));
}

/**********************************************************************/


