/*
 * Project: ble_freertos_demo_pr008_9_cJson
 * Module: isdn
 * File: drv_isdn.c
 * Created Date: 2022-07-23 11:20:58
 * Author: baoye
 * Description: һ��ͨͨѶ
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
#include "vd_fml_per_isdn.h"

#include "math.h"
#include "protocol_format.h"
#include "vd_bsp_systick.h"
#include "iservice.h"

#if (KIT_PLATFORM == KIT_VDCHIP4005)
#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "driver_pmu.h"
#include "os_timer.h"
os_timer_t drv_isdn_state_check_timer;
/* ======================================================================================
 * macros
 */
#if (PROJECT_ISDN_EN == ON)
/* ======================================================================================
 * types
 */
static vd_uint8_t  start_signal = 0;  //��ʼ�ź�
static vd_uint64_t  t1_time      = 0;  //�����ߵ�ƽʱ��
static vd_uint64_t  t2_time      = 0;  //�����͵�ƽʱ��
static vd_uint64_t low_time     = 0;  //�͵�ƽ����ʱ��
static vd_uint64_t high_time    = 0;  //�ߵ�ƽ����ʱ��
static vd_uint8_t  high_or_low  = 2;  //�ߵ͵�ƽ
static vd_uint8_t  byte         = 0;  //�����ֽڱ�ʾ
static vd_uint8_t  bit_num      = 0;  //�������ݵ�λ��
// static vd_uint8_t  byte_num      = 0;    //�ֽڼ���
// static vd_uint8_t  isdn_sum[12] = {0};  //�������ݵ�����
vd_uint8_t byte_num     = 0;    //�ֽڼ���
vd_uint8_t isdn_sum[12] = {0};  //�������ݵ�����
/* ======================================================================================
 * declaration
 */
#if (PROJECT_ISDN_TEST == ON)
vd_uint64_t  test_num[1024] = {0};
vd_uint16_t test_i         = 0;
#endif
/* ======================================================================================
 * globals
 */
static drv_isdn_t g_drv_isdn[] = {
#if ((PROJECT_ISDN0_EN == ON) || (PROJECT_ISDN1_EN == ON))
#if (PROJECT_ISDN0_EN == ON)
    {PLATFORM_ISDN0, PROJECT_ISDN_PIN, VD_GPIO_MODE_INPUT},
#endif
#if (PROJECT_ISDN1_EN == ON)
    {PLATFORM_ISDN1, 16, VD_GPIO_MODE_INPUT},
#endif
#else
    {NULL, NULL, NULL, NULL},
#endif
};
vd_uint8_t drv_isdn_statt_cnt = 0;

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */
/**
 * @brief : һ��ͨ�жϴ�����
 *
 * @param pin:�ж�����
 * @param value:���ŵ�ƽ
 * @return vd_void_t
 */
static vd_void_t isdn_pin_isr(vd_uint8_t pin, vd_uint8_t value) {
#if (PROJECT_ISDN_TEST == ON)
    /*---------------------------- ��������---------------------------*/
    if (value == 1) {
        t1_time = drv_systick_get();                                          //�����ߵ�ƽʱ��
        drv_gpio_irq_mode(PROJECT_ISDN_PIN, DRV_GPIO_MODE_EXTI_IT_FALLING);  //�ߵ�ƽ�����½��ش���
        low_time = t1_time - t2_time;                                         //�͵�ƽ����ʱ��
        if (test_i < 1024) {
            test_num[test_i++] = t1_time;
            test_num[test_i++] = 1;
        }
    } else {
        t2_time = drv_systick_get();                                         //�����͵�ƽʱ��
        drv_gpio_irq_mode(PROJECT_ISDN_PIN, DRV_GPIO_MODE_EXTI_IT_RISING);  //�͵�ƽ���������ش���
        high_time = t2_time - t1_time;                                       //�ߵ�ƽ����ʱ��
        if (test_i < 1024) {
            test_num[test_i++] = t2_time;
            test_num[test_i++] = 0;
        }
    }
    if (high_time != 0 && low_time != 0) {
        high_or_low = (low_time > high_time) ? LOW : HIGH;
        if (test_i < 1024) {
            // test_num[test_i] = high_or_low;
            // test_i++;
            high_time = low_time = 0;  //�ߵ͵�ƽ����ʱ�����
        }
    }
    return;

/*---------------------------- ��������---------------------------*/
#endif
    // printf("start_signal=%d\r",start_signal);
    if (value == 1) {
        t1_time = drv_systick_get();                                                                                                                  
        vd_bsp_gpio_requst_irq(PROJECT_ISDN_PIN,VD_GPIO_IRQ_MODE_FALLING,isdn_pin_isr,vd_null); //�����ߵ�ƽʱ��
        vd_bsp_gpio_irq_enable(PROJECT_ISDN_PIN,VD_GPIO_IRQ_ENABLE);
        if (t1_time != 0 && start_signal == 1) {
            high_time = t2_time - t1_time;  //�ߵ�ƽ����ʱ��
        } else if (t1_time != 0 && start_signal == 2) {
            start_signal = 1;          //��ʼ��������
            high_time = low_time = 0;  //�ߵ͵�ƽ����ʱ�����
        }
#if 0
        if ((t2_time - t1_time >= 10000) && start_signal == 0) {
            start_signal = 2;  //��ʼ�ź�
        } else if (t2_time != 0 && start_signal == 1) {
            low_time = t1_time - t2_time;  //�͵�ƽ����ʱ��					
        }
#endif
    } else {
        t2_time = drv_systick_get();                                         //�����͵�ƽʱ��                                                                           
        vd_bsp_gpio_requst_irq(PROJECT_ISDN_PIN,VD_GPIO_IRQ_MODE_RISING,isdn_pin_isr,vd_null); //�����ߵ�ƽʱ��
        vd_bsp_gpio_irq_enable(PROJECT_ISDN_PIN,VD_GPIO_IRQ_ENABLE);

        if ((t2_time - t1_time >= 10000) && start_signal == 0) {
            start_signal = 2;  //��ʼ�ź�
        } else if (t2_time != 0 && start_signal == 1) {
            low_time = t1_time - t2_time;  //�͵�ƽ����ʱ��
        }
    }
    if (high_time != NULL && low_time != NULL && start_signal == 1) {
        high_or_low = (low_time > high_time) ? HIGH : LOW;  //�ߵ͵�ƽʱ�����
        drv_isdn_interaction(high_or_low);                  //�ֽ���Ϻ���
        high_time = low_time = 0;                           //ʱ�����}
    }
}

/**
 * @brief : �ֽ���Ϻ���
 *
 * @param bit_value: �����λ��
 * @return vd_bool_t
 */
static vd_bool_t drv_isdn_interaction(vd_uint8_t bit_value) {
    vd_uint8_t byte_bit;
    byte_bit = bit_value;
    // isdn_sum[bit_num] = bit_value; //����bit
    byte |= byte_bit << (7 - bit_num);
    bit_num++;
    if (bit_num == 8) {
        bit_num = 0;                //λ������
                                     // printf("byte=%x\r",byte);
                                    // printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r");
        isdn_sum[byte_num] = byte;  //�����ֽ�
        byte               = 0;     //�ֽ�����
        byte_num++;                 //�ֽ���+1
        if (byte_num / 12 == 1) {
            start_signal = 0;                                   //��ʼ�ź����
            iservice_push_queue(PLATFORM_ISDN0, isdn_sum, 12,vd_null);  //���������涪����
            memset(&isdn_sum, 0, 12);
            byte_num = 0;
        }
    }
    return vd_true;
}

/**
 * @brief : ͨ��isdn_id������������
 *
 * @param isdn_id:һ��ͨ����id
 * @return drv_isdn_t*
 */
static drv_isdn_t* drv_isdn_find(vd_uint8_t isdn_id) {
    for (int i = 0; i < vd_array_size(g_drv_isdn); i++) {
        if (g_drv_isdn[i].isdn_id == isdn_id) {
            return &g_drv_isdn[i];
        }
    }
    return vd_null;
}

/**
 * @brief : һ��ͨ��ʼ���ڲ�����
 *
 * @return vd_bool_t
 */
vd_bool_t isdn_inside_init(void)
 {
  
    vd_bsp_gpio_requst_irq(PROJECT_ISDN_PIN,VD_GPIO_IRQ_MODE_FALLING,isdn_pin_isr,vd_null);
    vd_bsp_gpio_irq_enable(PROJECT_ISDN_PIN,VD_GPIO_IRQ_ENABLE);
    printf("isdn_init_successful\r");
    return vd_true;
}

/* ======================================================================================
 * implementation
 */

/**
 * @brief : һ��ͨ�����к���
 *
 * @param isdn_id: һ��ͨ����id
 * @param data : ��ȡ��������е�����
 * @param data_len : ��Ҫ��ȡ�����ݳ���
 * @return vd_uint16_t : ʵ�ʶ�ȡ�����ݳ���
 */
vd_uint16_t drv_isdn_read(vd_uint8_t isdn_id, vd_uint8_t* data, vd_uint16_t data_len) 
{
    drv_isdn_t* drv_isdn;
    drv_isdn = drv_isdn_find(isdn_id);
    vd_check_return_val_error(drv_isdn == vd_null, vd_false, "PLATFORM_ISDN %d OFF\r\n", isdn_id);
    return vd_queue_pull(&drv_isdn->rx_queue, data, data_len);
}

/**
 * @brief : ����һ��ͨ��ǰ״̬
 *
 * @param isdn_state ÿ�յ�һ�Σ�������Ϊ0��
 */
vd_void_t drv_set_isdn_state(vd_uint8_t isdn_state) 
{
	drv_isdn_statt_cnt = isdn_state;
}

/**
 * @brief : ����һ��ͨ��ǰ״̬
 *
 * @param isdn_state ÿ�յ�һ�Σ�������Ϊ0��
 */
vd_void_t drv_isdn_state_check(void *arg) 
{
	drv_isdn_statt_cnt++;
	if(drv_isdn_statt_cnt==20){
//		machine_pr015_isdn_clear_speed();
	}
	if(drv_isdn_statt_cnt>200){
		drv_isdn_statt_cnt =30;
	}
}


/**
 * @brief : һ��ͨ��ʼ���ӿں���
 *
 * @param isdn_id:һ��ͨ����id
 * @return vd_bool_t
 */
vd_bool_t drv_isdn_init(vd_uint8_t isdn_id) 
{
    drv_isdn_t* drv_isdn = drv_isdn_find(isdn_id);   
    vd_bsp_gpio_requst_irq(drv_isdn->pin_num,VD_GPIO_IRQ_MODE_FALLING,isdn_pin_isr,vd_null);
    vd_bsp_gpio_irq_enable(PROJECT_ISDN_PIN,VD_GPIO_IRQ_ENABLE);

    return vd_true;
}

/**
 * @brief : һ��ͨ��ʼ������
 *
 * @param void
 * @return
 */

vd_bool_t drv_isdn_init_t(vd_void_t)
 {
    vd_bsp_gpio_init(4,VD_GPIO_MODE_INPUT);
	os_timer_init(&drv_isdn_state_check_timer, drv_isdn_state_check, vd_null);
	os_timer_start(&drv_isdn_state_check_timer, 100, 1);
    printf("isnd ok\r\n");
}

/**
 * @brief : һ��ͨ����������
 *
 * @param void
 * @return
 */

#define start_low_level   450  //��ʼ�ĵ͵�ƽ ��100us��ʱ��
#define start_high_level  17   //��ʼ�ĸߵ�ƽ��100us��ʱ��


 


uint8_t isnd_level = 0;  //���ŵ�ƽ
#if 1

//��time��ȥʹ�����·�ʽ
//extern uint16_t test_L_buff[100];
//extern uint16_t test_H_buff[100];
//extern uint16_t test_L_len;
//extern uint16_t test_H_len;

uint16_t test_L_buff[100];
uint16_t test_H_buff[100];
uint16_t test_L_len;
uint16_t test_H_len;



uint16_t data_L_buff[100] = {0};
uint16_t data_H_buff[100] = {0};
uint16_t data_L_len       = 0;
uint16_t data_H_len       = 0;

uint8_t dabuff[20] = {0};
uint8_t dalen      = 0;

uint8_t isndbuff[20] = {0};  //һ��ͨ����   12�ֽ�
uint8_t isndbufflen  = 0;    //һ��ͨ���ݳ���

uint8_t rebit = 0;
uint8_t XOR   = 0;  //���ֵ

#else
extern unsigned short test1_L_buff[300];
extern unsigned short test1_H_buff[300];
extern unsigned short test1_L_len;
extern unsigned short test1_H_len;

#endif
vd_void_t drv_isdn_loop(vd_void_t) {
    //  			if(1==gpio_read_pin(GPIO_A,GPIO_PIN_4))
    //				{
    //
    //				  printf("gpioa4  hight\r\n");
    //
    //				}
#if 1
	
	
#if 1
    if (test_L_len == 96 && test_H_len == 96)  //���յ�һ֡���ݡ�12�ֽ�*8bit
    {
        memcpy(data_L_buff, test_L_buff, test_L_len * 2);  //����
        memcpy(data_H_buff, test_H_buff, test_H_len * 2);  //����
        data_L_len = test_L_len;
        data_H_len = test_H_len;
        test_L_len = 0;
        test_H_len = 0;

        for (int i = 0; i < 96; i++) {
            // printf("L[%d]=%02d  H[%d]=%02d  \r\n",i,data_L_buff[i],i,data_H_buff[i]);

#if 1
            if (data_L_buff[i] > start_low_level &&data_H_buff[i] > start_high_level)  //��ֹͬ���ź�δ�ڶ�ʱ������˳ɹ���һ�㲻�������
            {
                rebit = 0;
                dalen = 0;
                printf("Sync error \r\n");  //ͬ���ź�
            } 
						else 
						{
                if (data_L_buff[i] > data_H_buff[i])  //�ԱȻ�ȡһ��bit
                {
                    // databuff[datalen] |=0;
                } 
								else if (data_L_buff[i] < data_H_buff[i])  //�ԱȻ�ȡһ��bit
                {
                    dabuff[dalen] |= 1;
                }
                rebit++;
                if (rebit == 8)  //���յ�һ���ֽ�
                {
                    // printf("testbuff[%d]=%02x  \r\n",testlen,testbuff[testlen]);
                   // printf("databuff[%d]=%02x\r\n",dalen,dabuff[dalen]);

                    //       					    if(dalen==4)
                    //       							{
                    //
                    //       								//printf("databuff[%d]=%02x\r\n",dalen,dabuff[0]);
                    //       								//printf("databuff[%d]=%02x\r\n",dalen,dabuff[1]);
                    //       							  //printf("databuff[%d]=%02x\r\n",dalen,dabuff[dalen]);
                    //
                    //       							}
                    // vTaskDelay(4);

                    rebit = 0;
                    dalen++;
                    if (dalen == 12)  //���յ�12���ֽ�
                    {
                        XOR = dabuff[0] ^ dabuff[1] ^ dabuff[2] ^ dabuff[3] ^ dabuff[4] ^ dabuff[5] ^ dabuff[6] ^
                              dabuff[7] ^ dabuff[8] ^ dabuff[9] ^ dabuff[10];
                        //printf("XOR=%02x\r\n",XOR);

                        // if(XOR==dabuff[11] && dabuff[0]==0x08 && dabuff[1]==0x61)//���У��
                        if (XOR == dabuff[11])  //���У��
                        {
                            // printf("%02x   %02x   success\r\n",dabuff[0],dabuff[1]  );
													
													
                            memcpy(isndbuff, dabuff, dalen);                    //����		�õ�һ��ͨ����
                            isndbufflen = dalen;                                //һ��ͨ���ݳ���
													
													//printf("isnd shache=%02x\r\n",isndbuff[4]);
													
                            iservice_push_queue(PLATFORM_ISDN0, isndbuff, 12,vd_null);  //���������涪����

                            //       									 memcpy(isnd_data_rec.isnd_buff,dabuff,dalen);//�������׼���ϴ�
                            //       									 isnd_data_rec.isnd_len=dalen;
                            // printf("a\r\n");
                            // mcu_isnd_parse(isndbuff,isndbufflen);//һ��ͨ���ݽ���
                            // factory_isnd_flag=1;
                        } 
												else
												{
                            printf("%02x   %02x   fail\r\n", dabuff[0], dabuff[1]);
                        }

                        memset(dabuff, 0, 12);
                        dalen = 0;
                    }
                    //
                    //       		   		  	if(relen==128)//�쳣
                    //       		   		  	{
                    //
                    //       		   		  	  relen=0;
                    //       		   					printf("error");
                    //       		   		  	}
                } else {
                    dabuff[dalen] = dabuff[dalen] << 1;  //��λ
                }
            }
#endif
        }

    } else {
        // printf("data over 12 byte or no data   error\r\n");
    }
#endif
	
#else
 //printf("test1_L_len=%d test1_H_len=%d",test1_L_len,test1_H_len);
 if((test1_L_len==198) && (test1_H_len==198))
 {
   for(unsigned short i=0;i<198;i++)
   {
	   printf("L[%d]=%d  H[%d]=%d\r\n",i,test1_L_buff[i],i,test1_H_buff[i]);
	   
	 }
   test1_L_len=0;
	 test1_H_len=0;
 }

#endif		
		
}

#endif
#endif