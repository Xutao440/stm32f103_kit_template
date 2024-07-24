#include "vd_fml_per_beep_module.h"
#include "vd_bsp_systick.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"
                                   
vd_fml_per_beep_t  beeps_obj_arry[BEEP_OBJ_MAX] = {0};    //beep ��������
vd_uint16_t        beeps_obj_arry_mask = 0;

 
/**************************************************************************
*�������ƣ�vd_beep_on
*�������ܣ��ڲ�����beep��,����pwmģʽ
*���������beep_id��beep��id��
*���ز�������
***************************************************************************/
static void vd_beep_on(vd_uint8_t beep_id)
{
	vd_fml_per_beep_t* beeps = vd_null;
	
	beeps = &beeps_obj_arry[beep_id]; 

	if(beeps->pwm_obj_id != -1){   //pwmģʽ
		 vd_bsp_pwm_start(beeps->pwm_obj_id);
	}else{
		vd_bsp_gpio_write(beeps->beep_init.gpio_port, beeps->beep_init.beep_gpio_init_state ? VD_GPIO_LOW : VD_GPIO_HIGH);
	}
}

/**************************************************************************
*�������ƣ�vd_beep_off
*�������ܣ��ڲ�����beep��,����pwmģʽ
*���������beep_id��beep��id��
*���ز�������
***************************************************************************/
static void vd_beep_off(vd_uint8_t beep_id)
{
	vd_fml_per_beep_t* beeps = vd_null;
	
	beeps = &beeps_obj_arry[beep_id]; 

	if(beeps->pwm_obj_id != -1){   //pwmģʽ
		vd_bsp_pwm_stop(beeps->pwm_obj_id);
	}else{
		vd_bsp_gpio_write(beeps->beep_init.gpio_port, beeps->beep_init.beep_gpio_init_state ? VD_GPIO_HIGH : VD_GPIO_LOW);
	}
}


/**************************************************************************
*�������ƣ�vd_fml_per_beep_init
*�������ܣ�led�Ƴ�ʼ��
*���������beep: beep�ƶ���, beep_pwm: ��Դ��������Ҫ���ã���Դ��NULL
*���ز������ɹ�����beep����id��ʧ�ܷ���<0
*ע��2.4k   416us ���� 
***************************************************************************/
vd_int32_t vd_fml_per_beep_init(vd_fml_per_beep_init_t* beep, vd_pwm_t* beep_pwm)
{
	vd_int32_t ret_value = VD_BSP_NOERROR;
	vd_int32_t   pwm_obj_id = -1, index = 0;           
	vd_fml_per_beep_t* beeps = vd_null;
	
	vd_check_return_val_error(beep == NULL, -VD_BSP_ERR_PARAMETER, "[ %s ]: parameter error \r\n",__FUNCTION__ );
	beeps = beeps_obj_arry; 
	
	/*GPIO��ʼ��*/
	log_info("vd_fml_per_led_init: led->gpio_port=%d\r\n",beep->gpio_port);
	ret_value = vd_bsp_gpio_init(beep->gpio_port, VD_GPIO_MODE_OUT_PP);  //Ĭ���������
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_SDK, "[ %s ]: bsp gpio init fail! \r\n",__FUNCTION__ );
	
	/*��Դ����������*/
	if(beep_pwm != NULL){
		ret_value = vd_bsp_pwm_init(beep_pwm);
		vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: bsp pwm init fail! \r\n",__FUNCTION__ );
		pwm_obj_id = ret_value; //����pwm����id
		log_info("beep pwm init: pwm_obj_id=%d\r\n",pwm_obj_id);
	}
	
	/*ע�ᵽ����*/
	for(index = 0; index < BEEP_OBJ_MAX; index++){	
		if(((beeps_obj_arry_mask >> index) & 0x01) == 0){
			beeps_obj_arry_mask |= 0x0001 << index;
			log_info("beeps_obj_arry_mask=%d, index=%d\r\n",beeps_obj_arry_mask, index);
			/*�Զ������*/
			beeps[index].beep_init.gpio_port = beep->gpio_port;                         //�Զ������ź�
			beeps[index].beep_init.beep_gpio_init_state = beep->beep_gpio_init_state;   //beep���ų�ʼ״̬ 
			beeps[index].beep_init.vd_beep_long_on_ms = beep->vd_beep_long_on_ms;       //��������������ʱ��
			beeps[index].beep_init.vd_beep_short_on_ms = beep->vd_beep_short_on_ms;     //��������������ʱ��
			beeps[index].beep_init.vd_beep_off_ms = beep->vd_beep_off_ms;               //�������ر�ʱ��
					
			/*Ĭ�ϲ���*/
			beeps[index].pwm_obj_id = pwm_obj_id;                                       //pwm����id
			beeps[index].vd_beep_mode = VD_BEEP_NONE;		                            //������
			beeps[index].curr_sys_time_ms = 0;                                          //beepʱ��Ƭ
			beeps[index].curr_work_time_ms = 0;                                         //beep�ѹ���ʱ��
			beeps[index].curr_state = VD_BEEP_OFF;                                      //��ǰbeep״̬
			ret_value = index;		
			break;
		}		
	}
	
	vd_check_return_val_error(index >= BEEP_OBJ_MAX, -VD_BSP_ERR_SDK, "[ %s ]: beeps obj arry is full\r\n",__FUNCTION__ );	
}



/*
************************************************************
*	�������ƣ�	vd_fml_set_buzz_control_info
*	�������ܣ�	����������
*	��ڲ�����	buzz_mode��������ģʽ�� count�����д���
*	���ز�����	����ʣ���������
*	˵����		
************************************************************
*/
vd_int32_t vd_fml_pre_beep_control(vd_uint8_t beep_id, VD_BEEP_MODE beep_mode, uint8_t beep_count)
{
	vd_int32_t ret_value = -1;
	vd_fml_per_beep_t* beeps = vd_null;
	
	vd_check_return_val_error(beep_id > BEEP_OBJ_MAX || beep_mode > VD_BEEP_MAX, vd_false, "[ %s ]: parameter error \r\n",__FUNCTION__ );
	
	beeps = beeps_obj_arry;
	
	if(beeps->curr_state != VD_BEEP_ON){   //����������������״̬
		beeps->curr_state = VD_BEEP_ON;
		beeps->curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
		beeps->curr_work_time_ms = beeps->curr_sys_time_ms;
		beeps->vd_beep_count = beep_count;
		beeps->vd_beep_mode = beep_mode;
		vd_beep_on(beep_id);
		log_info("beep_id=%d,state=%d,count=%d,mode=%d,\r\n",beep_id,beeps->curr_state,beeps->vd_beep_count,beeps->vd_beep_mode);
	}
	
	return vd_true;	
}

/*
************************************************************
*	�������ƣ�	vd_fml_per_beep_loop
*	�������ܣ�	fengm
*	��ڲ�����	��
*	���ز�����	��
*	˵����		
************************************************************
*/
void vd_fml_per_beep_loop(void) 
{
	vd_uint8_t index = 0;		
	vd_fml_per_beep_t* beeps = vd_null;
	
	while((beeps_obj_arry_mask >> index) & 0x01){		
        beeps = &beeps_obj_arry[index]; 		
        switch(beeps->vd_beep_mode){
			case VD_BEEP_NONE: //������
				vd_beep_off(index);
				break;
			
			case VD_BEEP_SHORT://����
				if((beeps->curr_state == VD_BEEP_ON) && (beeps->vd_beep_count != 0)){       //���첿�ִ���					
					if(vd_bsp_time_differ(TICK_TYEP_MS, beeps->curr_sys_time_ms) >= beeps->beep_init.vd_beep_long_on_ms){
						beeps->curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
						beeps->curr_state = VD_BEEP_OFF;
						vd_beep_off(index);
						beeps->vd_beep_count--;
						log_info("VD_BEEP_SHORT_ON, count=%d, state=%d\r\n",beeps->vd_beep_count,beeps->curr_state);
					}
				}else if((beeps->curr_state == VD_BEEP_OFF) && (beeps->vd_beep_count != 0)){//�����첿�ִ���
					if(vd_bsp_time_differ(TICK_TYEP_MS, beeps->curr_sys_time_ms) >= beeps->beep_init.vd_beep_off_ms){
						beeps->curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
						beeps->curr_state = VD_BEEP_ON;
						vd_beep_on(index);
						log_info("VD_BEEP_SHORT_OFF, count=%d, state=%d\r\n",beeps->vd_beep_count,beeps->curr_state);
					}					
				}else{
					vd_beep_off(index);
				}
				break;
			
			case VD_BEEP_LONG://����
				if((beeps->curr_state == VD_BEEP_ON) && (beeps->vd_beep_count != 0)){       //���첿�ִ���					
					if(vd_bsp_time_differ(TICK_TYEP_MS, beeps->curr_sys_time_ms) >= beeps->beep_init.vd_beep_short_on_ms){
						beeps->curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
						beeps->curr_state = VD_BEEP_OFF;
						vd_beep_off(index);
						beeps->vd_beep_count--;
						log_info("VD_BEEP_LONG_ON, count=%d, state=%d\r\n",beeps->vd_beep_count,beeps->curr_state);
					}
				}else if((beeps->curr_state == VD_BEEP_OFF) && (beeps->vd_beep_count != 0)){//�����첿�ִ���
					if(vd_bsp_time_differ(TICK_TYEP_MS, beeps->curr_sys_time_ms) >= beeps->beep_init.vd_beep_off_ms){
						beeps->curr_sys_time_ms = vd_bsp_get_now_time(TICK_TYEP_MS);
						beeps->curr_state = VD_BEEP_ON;
						vd_beep_on(index);
						log_info("VD_BEEP_LONG_OFF, count=%d, state=%d\r\n",beeps->vd_beep_count,beeps->curr_state);
					}					
				}else{
					vd_beep_off(index);
				}
				break;
			
			default:
				vd_beep_off(index);
				break;
		}
	}	
}	









