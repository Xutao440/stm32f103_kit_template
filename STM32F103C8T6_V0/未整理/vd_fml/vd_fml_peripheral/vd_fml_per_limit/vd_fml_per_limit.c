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
#define NFC_AND_CONFIG_INFO_ADDR 0xFE000  // nfc信息和一线通信息、电压电量表

/* ======================================================================================
 * macros
 */
// 宏定义位置

/* ======================================================================================
 * types
 */
// 类型定义位置

/* ======================================================================================
 * declaration
 */
// 函数定义位置

/* ======================================================================================
 * globals
 */
// 全局变量位置
static vd_uint8_t drv_speed_limit_state=0x02;//0x01解限速状态  0x02限速状态
static vd_uint32_t  drv_speed_limit_ret=0;
static vd_uint8_t drv_speed_limit_cnt=0;
static vd_uint8_t drv_speed_limit_trigger_flag=0;
static vd_uint8_t drv_speed_limit_valus=0x00;//
static vd_uint8_t drv_speed_limit_sendcnt=10;//默认发10次
static vd_uint8_t drv_speed_limit_once_flag = 0;

/* ======================================================================================
 * helper
 */
// 帮助函数位置



/**
 * @brief bit起始信号
 * @param 
 * @return vd_uint8_t
 */
/****************bit值：起始信号*******************/
vd_uint8_t mcu_bit_begin(void)//高：1ms   低：3ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<40)) //1ms
	 {
	   //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//低 2023-3-2 todo
	   gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//锁电机反向再反向     低
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
 * @brief bit值：1
 * @param 
 * @return vd_uint8_t
 */
/****************bit值：1*******************/
vd_uint8_t mcu_bit_1(void)//高：3ms   低：1ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<30)) //3ms
	 {
	   //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//高  todo
	    gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//锁电机反向再反向     高
	 }

   if((bit_cnt>=30)&&(bit_cnt<40)) //1ms
	 {
	 	 //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//低  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//锁电机反向再反向     低
	 }
	
   if(bit_cnt>=40)
	 {
	   bit_cnt=0;
		 return 1;
		 
	 }
	  return 0;
}
	


vd_uint8_t mcu_bit_t_1(void)//高：3ms   低：1ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<20)) //2ms
	 {
	   //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//高  todo
	    gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//锁电机反向再反向     高
	 }

   if((bit_cnt>=20)&&(bit_cnt<30)) //1ms
	 {
	 	 //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//低  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//锁电机反向再反向     低
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
 * @brief bit值：0
 * @param 
 * @return vd_uint8_t
 */
/****************bit值：0*******************/
vd_uint8_t mcu_bit_0(void)//高：1ms   低：3ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<10)) //1ms
	 {
	  // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//高  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//锁电机反向再反向     高
	 }

   if((bit_cnt>=10)&&(bit_cnt<40)) //3ms
	 { 
	 	// mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//低  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//锁电机反向再反向     低
	 }
	
   if(bit_cnt>=40)
	 {
	   bit_cnt=0;
		 return 1;
		 
	 }
	  return 0;
}





vd_uint8_t mcu_bit_t_0(void)//高：1ms   低：3ms
{
	static vd_uint8_t bit_cnt=0;
	bit_cnt++;
//	
	 if((bit_cnt>=0)&&(bit_cnt<10)) //1ms
	 {
	  // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//高  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//锁电机反向再反向     高
	 }

   if((bit_cnt>=10)&&(bit_cnt<30)) //2ms
	 { 
	 	// mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//低  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//锁电机反向再反向     低
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
 * @brief bit值：结束信号
 * @param 
 * @return vd_uint8_t
 */
/****************bit值：结束信号*******************/
vd_uint8_t mcu_bit_end(void)//高：1ms   低：3ms
{
	   //mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//高  todo
	    gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//锁电机反向再反向     高
		 return 1; 
}
	

vd_uint8_t mcu_bit_end_t(void)//高：1ms   低：3ms
{
		static vd_uint16_t bit_cnt=0;
	  bit_cnt++;
	
	 if((bit_cnt>=0)&&(bit_cnt<10)) //1ms
	 {
	  // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//高  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 0);//锁电机反向再反向     高
	 }
	 
   if((bit_cnt>=10)&&(bit_cnt<260)) //2ms
	 { 
	 	// mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//低  todo
	     gpio_write_pin(GPIO_C, GPIO_PIN_7, 1);//锁电机反向再反向     低
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
// 私有函数实现位置



/* ======================================================================================
 * implementation
 */
// 函数实现位置





/**
 * @brief 限速波形
* @param  data：发送的数据
 * @return vd_uint8_t
 */
/***********************************限速*****************************************/

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
	     
	     
	else if(3==step)//结束
	{
	     	//step=0;
	      mcu_bit_end();//结束信号
	       //少一个标志位？？？
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
	     
	     
	else if(3==step)//结束
	{
	     	//step=0;
	    ret=  mcu_bit_end_t();//结束信号
	       //少一个标志位？？？
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
 * @brief 限速驱动
* @param  
 * @return vd_void_t
 */
/************************限速驱动********************************/
vd_void_t mcu_limit_drive(void)
{

	#if 1//限速 2022-10-24
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
 * @brief 限速调用接口
* @param   customer:客户：1锡特  2凤凰   limit_mode：0x01解限速状态  0x02限速状态  limit_valus:发送的数值  send_cnt：发送波形次数，当前默认10次
 * @return vd_void_t
 */
/*****************************限速接口*************************************/
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
 * @brief 限速调用接口
* @param  limit_mode：0x01解限速状态  0x02限速状态  limit_valus:发送的数值  send_cnt：发送波形次数，当前默认10次
 * @return vd_void_t
 */

vd_uint8_t drv_get_limit_state(vd_void_t)
{
	
  return  drv_speed_limit_state;

}

/**
 * @brief 限速设置接口
 * @param  state：0x01解限速状态  0x02限速状态 
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
 * @brief 限速设置接口
 * @param  state：0x01解限速状态  0x02限速状态 
 * @return vd_void_t
 */

vd_uint8_t drv_get_limit_value_idx(vd_void_t)
{	
	return config_table.user_chose_info_.user_chose_info[2]; 
}
/**
 * @brief 限速波形数据接口
 * @param  limit_mode：0x01解限速状态  0x02限速状态  limit_valus:发送的数值  send_cnt：发送波形次数，当前默认10次
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
 * @brief 限速设置使能接口
 * @param  en_type 0x00,NFC长刷；0x01，BLE设置
 * @return vd_void_t
 */
vd_void_t drv_set_limit_en_type(vd_uint8_t en_type)
{	
	config_table.user_chose_info_.user_chose_info[1] = en_type;
//	drv_flash_save_to_temp(NFC_AND_CONFIG_INFO_ADDR, sizeof(config_table_t), (vd_uint8_t*)&config_table);
	vd_bsp_flash_write(NFC_AND_CONFIG_INFO_ADDR, (vd_uint8_t*)&config_table, sizeof(config_table_t));
}

/**
 * @brief 限速获取使能接口
 * @param  en_type 0x00,NFC长刷；0x01，BLE设置
 * @return vd_uint8_t
 */
vd_uint8_t drv_get_limit_en_type(vd_void_t)
{	
	return config_table.user_chose_info_.user_chose_info[1];
}

/**
 * @brief 限速获取执行一次接口
 * @param  
 * @return vd_uint8_t 0，已发送；1，未发送
 */
vd_uint8_t drv_get_limit_once_flag(vd_void_t)
{	
	return config_table.user_chose_info_.user_chose_info[3];
}

/**
 * @brief 限速设置执行一次接口
 * @param  en_type 0x00,NFC长刷；0x01，BLE设置
 * @return vd_uint8_t
 */
vd_void_t drv_set_limit_once_flag(vd_uint8_t flag)
{	
	config_table.user_chose_info_.user_chose_info[3] = flag;
//	drv_flash_save_to_temp(NFC_AND_CONFIG_INFO_ADDR, sizeof(config_table_t), (vd_uint8_t*)&config_table);
	vd_bsp_flash_write(NFC_AND_CONFIG_INFO_ADDR, (vd_uint8_t*)&config_table, sizeof(config_table_t));
}

/**********************************************************************/


