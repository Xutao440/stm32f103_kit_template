#ifndef _VD_FML_BEEP_H_
#define _VD_FML_BEEP_H_

#include "base_type.h"
#include "vd_bsp_type.h"
#include "vd_bsp_gpio.h"
#include "vd_bsp_pwm.h"

#define BEEP_OBJ_MAX     1     //BEEP�������

/*������ģʽ*/
typedef enum{
	VD_BEEP_NONE = 0,  //������
	VD_BEEP_SHORT,     //����
	VD_BEEP_LONG,      //����
	
	VD_BEEP_MAX,
}VD_BEEP_MODE;

/*������ģʽ*/
typedef enum{
	VD_BEEP_OFF = 0,
	VD_BEEP_ON,
	
	VD_BEEP_STATE_MAX,
}VD_BEEP_STATE;

/*��������ʼ������*/
typedef struct{
	GPIO_PORT_E  gpio_port;             //�Զ������ź�
	GPIO_STATE_E beep_gpio_init_state;  //beep���ų�ʼ״̬
	vd_uint16_t  vd_beep_long_on_ms;    //����ʱ��
	vd_uint16_t  vd_beep_short_on_ms;   //����ʱ��
	vd_uint16_t  vd_beep_off_ms;        //�ر�ʱ��
	
}vd_fml_per_beep_init_t;

/*����������*/
typedef struct {
	vd_fml_per_beep_init_t  beep_init;             //beep��ʼ���ṹ��
	vd_int32_t              pwm_obj_id;            //pwm����Ĭ��-1
	vd_uint32_t             curr_sys_time_ms;      //beep��¼ϵͳʱ��
	vd_uint32_t             curr_work_time_ms;     //beep��ǰ����ʣ��ʱ��
    VD_BEEP_STATE           curr_state;            //beep��ǰ״̬
	VD_BEEP_MODE            vd_beep_mode;          //��������
	vd_uint8_t              vd_beep_count;         //�������������
} vd_fml_per_beep_t;


vd_int32_t vd_fml_per_beep_init(vd_fml_per_beep_init_t* beep, vd_pwm_t* beep_pwm);                 //��������ʼ������
vd_int32_t vd_fml_pre_beep_control(vd_uint8_t beep_id, VD_BEEP_MODE beep_mode, uint8_t beep_count);//���������ƺ�������
void vd_fml_per_beep_loop(void);                                                                   //������ѭ��������


#endif 
