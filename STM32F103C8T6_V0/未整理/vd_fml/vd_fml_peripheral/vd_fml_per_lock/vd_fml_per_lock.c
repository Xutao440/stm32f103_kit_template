/*
 * Project: fr8000
 * Module: lock
 * File: drv_lock.c
 * Created Date: 2022-10-18 21:23:34
 * Author: liaozewei
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
#include "vd_fml_per_lock.h"

#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "os_timer.h"
#include "system_config.h"
#include "platform_config.h"
//������Ŀ�е�״̬��
//#include "machine_pr015.h"

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
//querystatus_cmd_t       lock_cmd;
extern vd_uint8_t       lock_state;
vd_uint8_t              mcu_fire_up_state         = 0;  //���״̬
static vd_uint8_t       frist_flag                = 0;
vd_uint8_t              guard_against_theft_state = 0;  //����//0;//����״̬
vd_uint8_t              beep_sec                  = 0;  //����������
vd_uint8_t              electric_door_lock_state  = 0;  //������״̬
vd_uint8_t              anti_beep_flag            = 0;
vd_uint8_t              ele_lock_state            = 0;  //������״̬
// vd_uint8_t  lock_state=0;      // ����״̬  0������  1������
vd_uint8_t fire_up_state = 0;  //���״̬

vd_uint8_t ele_lock_state_flag = 0;  //��ص�������û�д򿪱�־λ

vd_uint8_t        lock_on_sta_flag  = 0;  //��������򿪺��������û�д򿪱�־λ
vd_uint8_t        lock_off_sta_flag = 0;  //����ʱ��ص�����״̬ 1����  0���ر�
static vd_uint8_t antitheft_state   = 0;

os_timer_t      test_timer;
static uint32_t flag_timer = 0;

vd_uint8_t loc_ele_control_state = 0;  //������״̬

static unsigned char  sensor_on_flag = 0;  // 0���𶯴���������      1���𶯴���������
static unsigned short sensor_cnt     = 0;
static unsigned short horn_cnt       = 0;  //���ȱ�־λ

static unsigned char level_inversion_cnt = 0;  //��ص�ƽ��ת������Ϊ����״̬���������׼��  2022-9-17
static unsigned char low_level_cnt = 0;  //��ص͵�ƽ����������Ϊ����״̬���������׼��   2022-9-17
static unsigned short times_cnt    = 0;

static unsigned char turn_wheel_flag = 0;  //����ת����־λ

static unsigned char state_flag = 0;

static vd_uint8_t mechanical_lock = 0;  // 0:��е��û����   1:��е������

/* ======================================================================================
 * helper
 */
// ��������λ��

/**
 * @brief :���Լ���
 *
 * @param  arg
 * @return vd_void_t
 */
vd_void_t test_timer_func(vd_void_t* arg) {
    flag_timer++;
    // printf("flag_timer = %d\r",flag_timer);
}

/**
 * @brief :�õ�������״̬
 *
 * @param  arg
 * @return vd_void_t
 */
vd_uint8_t lock_ele_get_state(vd_uint8_t data) {
    ele_lock_state = data;
    // return ele_lock_state;
}

/**
 * @brief :�õ�����״̬
 *
 * @param
 * @return vd_uint8_t
 */
vd_uint8_t get_antitheft_state(vd_void_t) {
    return antitheft_state;
}

/**
 * @brief ���Ƶ�������ƽ
 * @param data:���Ƶ�ƽ
 * @return vd_uint8_t
 */
vd_uint8_t lock_ele_ctr_state(vd_uint8_t data) {
    loc_ele_control_state = data;
}

/**
 * @brief ���ؿ��Ƶ�������ƽ
 * @param
 * @return vd_uint8_t
 */
vd_uint8_t lock_ele_control(vd_void_t) {
    return loc_ele_control_state;
}

/* ======================================================================================
 * private implementation
 */
// ˽�к���ʵ��λ��

/* ======================================================================================
 * implementation
 */
// ����ʵ��λ��

/**
 * @brief ��������
 * @param
 * @return vd_uint8_t
 */
vd_uint8_t drv_lock_switch_on(vd_void_t) {
//    printf("ele_lock_state=%02x\r\n", ele_lock_state);
    if (1 == ele_lock_state)  //��ǰΪ���״̬ ������Ч
    {
//        printf("fire on now,invalid");
        return lock_on_off;

    } else if (0 == ele_lock_state)  //??????? ???
    {
        if (1 == guard_against_theft_state)  //???????  ????
        {
            //??????
//            printf("fire off now,but invalid");
            return lock_on_on_operate;

        } else if (0 == guard_against_theft_state)  //???????
        {
//            printf("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\r");
//            printf("fire off now,ok\r\n");
            lock_off_sta_flag         = 0;  // 51�������͵�ƽ״̬
            guard_against_theft_state = 1;  //??

            return lock_on_ok;
        }
    }
}

/**
 * @brief : lock handle �������� ��������
 *

 * @return vd_void_t
// */

vd_void_t drv_lock_handle(vd_void_t) {
    //	unsigned char sendbuff[4]={0};
    //	unsigned char bufflen=0;

    if (1 == guard_against_theft_state)  //���״̬
    {
#if 1                                                //�𶯴�����  ֻ����
        if (1 ==  vd_bsp_gpio_read(31))  //�𶯴����� �ߵ�ƽ  ���������Ǹߵ͵�ƽ������һֱ��
        {
            // printf("open zhendong\r\n");
            //			 times_cnt=0;
            //			 mcu_gpio_set_value(CAR_LOCKING_SIGNAL_PORT,CAR_LOCKING_SIGNAL_PIN,MCU_IO_HIGH);//�򿪵��ӵ�����

            sensor_on_flag = 1;  // 1���𶯴���������
            sensor_cnt     = 0;
        } else if (0 == vd_bsp_gpio_read(31))  //�𶯴����� �͵�ƽ    �����Ǹߵ͵�ƽ
        {
            sensor_cnt++;
            if (20 == sensor_cnt)  // 5s
            {
                //printf("close zhendong\r\n");
                sensor_cnt     = 0;
                sensor_on_flag = 0;
                // horn_cnt=0;
            }
        }

        if ((1 == sensor_on_flag) || (1 == turn_wheel_flag))  //�𶯴��������� ��ָ����cpu������
        {
            
            
           // extern vd_uint64_t pre_warning_time;  ���ÿ���״̬��ʱ��Ҫ��
             vd_uint64_t pre_warning_time;
            
            // printf("times=%d\r", common_time_get_current_time_ms() - pre_warning_time);
            if (common_time_get_current_time_ms() - pre_warning_time >= 7000) {
                // printf("horn_cnt = %d\r", horn_cnt);
                if (0 == horn_cnt) {
                    beep_sec = 1;
                } else if (800 == horn_cnt) {
                    horn_cnt = 0;
                    beep_sec = 1;
                }
                horn_cnt++;
            }
        } else if ((0 == sensor_on_flag) && (0 == turn_wheel_flag)) {
            horn_cnt = 0;
        }

#endif

#if 1                                                //�ֶ����  �򿪵��ӵ����� ���������
        if (0 ==  vd_bsp_gpio_read(22))  //�ֶ����  ���ֶ�
        {
#if 1  //�͵�ƽ����ʱ��
            low_level_cnt++;

#endif

#if 1  // 9-17 ��ص�ƽ��ת
            if ((level_inversion_cnt == 0) || (level_inversion_cnt == 2) || (level_inversion_cnt == 4)) {
                level_inversion_cnt++;
            }

#endif

            if ((level_inversion_cnt == 5) || (low_level_cnt > 8))  // 9-17  ��ת����4�� ���ߵ͵�ƽ����500ms
            {
                printf("level_inversion_cnt=%02x  low_level_cnt=%02x\r\n ", level_inversion_cnt, low_level_cnt);
                level_inversion_cnt = 0;  //�����ת����  9-17
                low_level_cnt       = 0;  //����͵�ƽ����ʱ��  9-17

                printf("close suo1\r\n");
                // mcu_gpio_set_value(CAR_LOCKING_SIGNAL_PORT,CAR_LOCKING_SIGNAL_PIN,MCU_IO_HIGH);//�򿪵��ӵ�����
                // ���˲��������
                // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//������ź�
                if (state_flag == 0) {
                    state_flag                    = 1;
//ʵ����Ŀ�д�    lock_cmd.device_electric_lock = 1;
                    lock_ele_ctr_state(1);
                    // printf("loc_ele_control_state=%02x\r\n", loc_ele_control_state);
                    ele_lock_state_flag = 1;
                    lock_on_sta_flag    = 1;
                    antitheft_state     = 1;
                }
                vd_bsp_gpio_write(23,VD_GPIO_LOW);  //
                // no_turn_flag=1;//���ֶ���
                turn_wheel_flag = 1;  //���ֶ���
                times_cnt       = 0;
            }

        } else if (1 == vd_bsp_gpio_read(22))  //�ֶ����  ���ֶ�
        {
#if 1  // 9-17 ��ص�ƽ��ת
            if ((level_inversion_cnt == 1) || (level_inversion_cnt == 3)) {
                level_inversion_cnt++;
            }
            low_level_cnt = 0;  //����͵�ƽ����ʱ��
#endif

            times_cnt++;
            if (150 == times_cnt)  // 10��
            {
                if (state_flag == 1) {
                    state_flag                    = 0;
//ʵ����Ŀ��Ҫ��  lock_cmd.device_electric_lock = 0;
                    lock_ele_ctr_state(0);
                    // printf("loc_ele_control_state=%02x\r\n", loc_ele_control_state);
                    ele_lock_state_flag = 0;
                    lock_on_sta_flag    = 0;
                    antitheft_state     = 0;
                }
                turn_wheel_flag = 0;
                sensor_on_flag  = 0;
                printf("10s come,  close \r\n");
                // no_turn_flag=0;
                times_cnt = 0;
                vd_bsp_gpio_write(23, VD_GPIO_HIGH);

                level_inversion_cnt = 0;  //�����ת����
                // mcu_gpio_set_value(CAR_LOCKING_SIGNAL_PORT,CAR_LOCKING_SIGNAL_PIN,MCU_IO_LOW);//�رյ��ӵ�����
                //  mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//��������ź�
            }
        }

#endif
    }
}

/**
 * @brief : lock switch off  ��������
 *

 * @return vd_uint8_t
// */

vd_uint8_t drv_lock_switch_off(void) {
    if (1 == guard_against_theft_state)  //��ǰΪ���״̬
    {
        if (1 == fire_up_state)  //��ǰ���ǵ��״̬���������״̬
        {
            // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//��������ź�
           vd_bsp_gpio_write(23, VD_GPIO_HIGH);
            lock_ele_ctr_state(1);
            // printf("loc_ele_control_state=%02x\r\n",loc_ele_control_state);
            //			   mcu_gpio_set_value(CAR_BUZZER_B_PORT,CAR_BUZZER_B_PIN,MCU_IO_LOW);//������B
            //			   mcu_gpio_set_value(CAR_BUZZER_A_PORT,CAR_BUZZER_A_PIN,MCU_IO_LOW);//������A

        } else if (0 == fire_up_state)  //��ǰ��Ϩ��״̬���������״̬
        {
            // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//��������ź�
            // mcu_gpio_set_value(CAR_LOCKING_SIGNAL_PORT,CAR_LOCKING_SIGNAL_PIN,MCU_IO_LOW);//�رյ��ӵ�����
          vd_bsp_gpio_write(23, VD_GPIO_HIGH);
            lock_ele_ctr_state(1);
            // printf("loc_ele_control_state=%02x\r\n",loc_ele_control_state);
            //			  mcu_gpio_set_value(CAR_BUZZER_B_PORT,CAR_BUZZER_B_PIN,MCU_IO_LOW);//������B
            //			  mcu_gpio_set_value(CAR_BUZZER_A_PORT,CAR_BUZZER_A_PIN,MCU_IO_LOW);//������A
        }
        lock_off_sta_flag = 1;
        printf("now mcu_remove_guard_against_theft\r\n");
        guard_against_theft_state = 0;  //��������𶯴��������ֶ�״̬��������

#if 1
        sensor_on_flag = 0;  // 0���𶯴���������      1���𶯴���������
        sensor_cnt     = 0;
        horn_cnt       = 0;  //���ȱ�־λ

        level_inversion_cnt = 0;  //��ص�ƽ��ת������Ϊ����״̬���������׼��  2022-9-17
        low_level_cnt       = 0;  //��ص͵�ƽ����������Ϊ����״̬���������׼��   2022-9-17
        times_cnt           = 0;

        turn_wheel_flag = 0;  //����ת����־λ

        state_flag = 0;
        lock_on_sta_flag=0;
        antitheft_state = 0;
#endif

        return lock_off_ok;
        //			 sendbuff[0]=0x09;
        //		   sendbuff[1]=0x80;
        //		   sendbuff[2]=0x01;
        //		   bufflen=3;
        //	     app_Combination(sendbuff,bufflen);

    } else if (0 == guard_against_theft_state)  //��ǰΪ���״̬ �������
    {
        lock_off_sta_flag = 1;
        lock_ele_ctr_state(1);
        // printf("loc_ele_control_state=%02x\r\n",loc_ele_control_state);
        printf("now mcu_remove_guard_against_theft\r\n");
        return lock_off_on_operate;
        //       sendbuff[0]=0x09;
        //		   sendbuff[1]=0x80;
        //		   sendbuff[2]=0x02;
        //		   bufflen=3;
        //	     app_Combination(sendbuff,bufflen);
    }
}

/**
 * @brief : ��е������
 *

 * @return vd_uint8_t
// */

vd_uint8_t drv_lock_switch_off_t(void) {
    if (1 == guard_against_theft_state)  //???????
    {
        if (1 == fire_up_state)  //????????,??????
        {
            // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//??????
           vd_bsp_gpio_write(23, VD_GPIO_HIGH);
            // lock_ele_ctr_state(1);
            // printf("loc_ele_control_state=%02x\r\n",loc_ele_control_state);
            //         mcu_gpio_set_value(CAR_BUZZER_B_PORT,CAR_BUZZER_B_PIN,MCU_IO_LOW);//???B
            //         mcu_gpio_set_value(CAR_BUZZER_A_PORT,CAR_BUZZER_A_PIN,MCU_IO_LOW);//???A

        } else if (0 == fire_up_state)  //???????,??????
        {
            // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//??????
            // mcu_gpio_set_value(CAR_LOCKING_SIGNAL_PORT,CAR_LOCKING_SIGNAL_PIN,MCU_IO_LOW);//???????
            vd_bsp_gpio_write(23, VD_GPIO_HIGH);
            // lock_ele_ctr_state(1);
            // printf("loc_ele_control_state=%02x\r\n",loc_ele_control_state);
            //        mcu_gpio_set_value(CAR_BUZZER_B_PORT,CAR_BUZZER_B_PIN,MCU_IO_LOW);//???B
            //        mcu_gpio_set_value(CAR_BUZZER_A_PORT,CAR_BUZZER_A_PIN,MCU_IO_LOW);//???A
        }
        lock_off_sta_flag = 1;
        printf("now mcu_remove_guard_against_theft\r\n");
        guard_against_theft_state = 0;  //??,??????,????????

#if 1
        sensor_on_flag = 0;  // 0���𶯴���������      1���𶯴���������
        sensor_cnt     = 0;
        horn_cnt       = 0;  //���ȱ�־λ

        level_inversion_cnt = 0;  //��ص�ƽ��ת������Ϊ����״̬���������׼��  2022-9-17
        low_level_cnt       = 0;  //��ص͵�ƽ����������Ϊ����״̬���������׼��   2022-9-17
        times_cnt           = 0;

        turn_wheel_flag = 0;  //����ת����־λ

        state_flag = 0;

#endif

        return lock_off_ok;
        //       sendbuff[0]=0x09;
        //       sendbuff[1]=0x80;
        //       sendbuff[2]=0x01;
        //       bufflen=3;
        //       app_Combination(sendbuff,bufflen);

    } else if (0 == guard_against_theft_state)  //??????? ????
    {
        lock_off_sta_flag = 1;
        // lock_ele_ctr_state(1);
        // printf("loc_ele_control_state=%02x\r\n",loc_ele_control_state);
        printf("now mcu_remove_guard_against_theft\r\n");
        return lock_off_on_operate;
        //       sendbuff[0]=0x09;
        //       sendbuff[1]=0x80;
        //       sendbuff[2]=0x02;
        //       bufflen=3;
        //       app_Combination(sendbuff,bufflen);
    }
}

/**
 * @brief : ����е�� �����
 *
 * @param
 */

vd_uint8_t lock_check_ele(vd_void_t) {
    static vd_uint16_t time_cnt = 0;
#if 0
  if((1==ele_lock_state)&&(0==ele_lock_state_flag))
  {
	   time_cnt++;
		
		if(time_cnt==30)//3s
		{
		
		  drv_lock_switch_off();
		
		}
	
	
	}
	else
	{
	  time_cnt=0;
	
	}
#endif

#if 1
    if ((1 == ele_lock_state) && (1 == lock_off_sta_flag))  //������io���ָߵ�ƽ����е�������������
    {
        return 0;
    }

    if ((1 == ele_lock_state) && (1 == guard_against_theft_state) &&
        (0 == lock_on_sta_flag))  //�����򿪣�û���������֣�ioû���ߡ����ǵ������е磬��е������
    {
        // time_cnt++;
        // if (time_cnt == 30) {
        time_cnt = 0;
        // drv_lock_switch_off();//����
        drv_lock_switch_off_t();
        mechanical_lock = 1;
        printf("jixiesuo jiesuo\r\n");
        return 1;
        // }
    }

    if ((1 == ele_lock_state) && (1 == guard_against_theft_state) &&
        (1 == lock_on_sta_flag))  //�����򿪣����������֣�io���ߡ��������е磬��е��Ҳ���ˣ�������ô�ж�
    {
        time_cnt++;
        if (time_cnt == 30) {
            time_cnt       = 0;
            anti_beep_flag = 1;
            printf("todo\r\n");
            // tido ����Ҫ��ô��
            //		   drv_lock_switch_off();
            //		   return 1;
        }
    }

#endif

#if 1  //�ڼ�ص���е�����Ϻ󣬻�е���ο�������

    if ((0 == ele_lock_state) && (1 == mechanical_lock)) {
        mechanical_lock = 0;
        drv_lock_switch_on();  //������
    }
#endif
}

/**
 * @brief :�õ���е��״̬
 *
 * @param
 */
vd_uint8_t get_mechanical_lock_state(vd_void_t) {
    return mechanical_lock;
}

/**
 * @brief : lock�¼��ϱ�
 *
 * @param  vd_void_t
 */
// vd_uint8_t nfc_tt=126;
static vd_void_t lock_report(lock_event_t* lock) {
    // log_debug("knob_status %s\r\n", (knob_event->rotate == ROTATE_LEFT) ? "Left" : "Right");
    // log_debug("k_v = %d\r\n", (knob_event->rotate == ROTATE_LEFT) ? --knob_tt : ++knob_tt);
    // knob_event->ev_type = EV_SYS_KNOB;
    // printf("k_v = %d\r\n", (nfc->nfc_id_len == 11) ? --nfc_tt : ++nfc_tt);
    // printf("nfc->nfc_id_len=%d\r\n",nfc->nfc_id_len);
    // nfc->nfc_id_len=6;
    lock->lock_type = EV_SYS_LOCK;
    event_push(lock);
}

/**
 * @brief : ����loop
 *
 * @return vd_bool_t
// */

vd_void_t drv_lock_loop(void) {
    lock_event_t lock;

    // printf("nfc ok\r\n");

    // nfc_demo();
    // nfc.nfc_id_len=6;
    // if(nfc.nfc_id_len!=0)
    // {
    lock_report(&lock);

    // }
}

/**
 * @brief : ������ʼ��
 *
 * @return vd_bool_t
// */
vd_bool_t drv_lock_init(void) {
   
    vd_bsp_gpio_init(23,VD_GPIO_MODE_INPUT_PULLUP);
    
    //���Ӽ��  ����

    vd_bsp_gpio_init(22,VD_GPIO_MODE_INPUT);
    
    vd_bsp_gpio_init(31,VD_GPIO_MODE_INPUT);
	
    vd_bsp_gpio_write(23,VD_GPIO_HIGH);
}

/**
 * @brief : �������ӱ���
 * @param st,����״̬��0���ɿ���1������
 * @return vd_bool_t
 */
vd_void_t drv_lock_wheel_out(vd_uint8_t st){
    vd_bsp_gpio_write(23,!st);
}

/**
 * @brief : �������Ӽ��
 * @param 
 * @return �ֶ�ת̬��0���ֶ���1�����ֶ�
 */
vd_uint8_t drv_lock_wheel_input(vd_void_t){
	 return vd_bsp_gpio_read(22);
}



