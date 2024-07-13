/*
 * Project: ble_freertos_demo_pr008_9_cJson
 * Module: isdn
 * File: drv_isdn.c
 * Created Date: 2022-07-23 11:20:58
 * Author: baoye
 * Description: 一线通通讯
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
static vd_uint8_t  start_signal = 0;  //起始信号
static vd_uint64_t  t1_time      = 0;  //触发高电平时刻
static vd_uint64_t  t2_time      = 0;  //触发低电平时刻
static vd_uint64_t low_time     = 0;  //低电平持续时间
static vd_uint64_t high_time    = 0;  //高电平持续时间
static vd_uint8_t  high_or_low  = 2;  //高低电平
static vd_uint8_t  byte         = 0;  //单个字节表示
static vd_uint8_t  bit_num      = 0;  //传输数据的位数
// static vd_uint8_t  byte_num      = 0;    //字节计数
// static vd_uint8_t  isdn_sum[12] = {0};  //保存数据的数组
vd_uint8_t byte_num     = 0;    //字节计数
vd_uint8_t isdn_sum[12] = {0};  //保存数据的数组
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
 * @brief : 一线通中断处理函数
 *
 * @param pin:中断引脚
 * @param value:引脚电平
 * @return vd_void_t
 */
static vd_void_t isdn_pin_isr(vd_uint8_t pin, vd_uint8_t value) {
#if (PROJECT_ISDN_TEST == ON)
    /*---------------------------- 测试用例---------------------------*/
    if (value == 1) {
        t1_time = drv_systick_get();                                          //触发高电平时刻
        drv_gpio_irq_mode(PROJECT_ISDN_PIN, DRV_GPIO_MODE_EXTI_IT_FALLING);  //高电平配置下降沿触发
        low_time = t1_time - t2_time;                                         //低电平持续时间
        if (test_i < 1024) {
            test_num[test_i++] = t1_time;
            test_num[test_i++] = 1;
        }
    } else {
        t2_time = drv_systick_get();                                         //触发低电平时刻
        drv_gpio_irq_mode(PROJECT_ISDN_PIN, DRV_GPIO_MODE_EXTI_IT_RISING);  //低电平配置上升沿触发
        high_time = t2_time - t1_time;                                       //高电平持续时间
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
            high_time = low_time = 0;  //高低电平持续时间清空
        }
    }
    return;

/*---------------------------- 测试用例---------------------------*/
#endif
    // printf("start_signal=%d\r",start_signal);
    if (value == 1) {
        t1_time = drv_systick_get();                                                                                                                  
        vd_bsp_gpio_requst_irq(PROJECT_ISDN_PIN,VD_GPIO_IRQ_MODE_FALLING,isdn_pin_isr,vd_null); //触发高电平时刻
        vd_bsp_gpio_irq_enable(PROJECT_ISDN_PIN,VD_GPIO_IRQ_ENABLE);
        if (t1_time != 0 && start_signal == 1) {
            high_time = t2_time - t1_time;  //高电平持续时间
        } else if (t1_time != 0 && start_signal == 2) {
            start_signal = 1;          //开始接收数据
            high_time = low_time = 0;  //高低电平持续时间清空
        }
#if 0
        if ((t2_time - t1_time >= 10000) && start_signal == 0) {
            start_signal = 2;  //起始信号
        } else if (t2_time != 0 && start_signal == 1) {
            low_time = t1_time - t2_time;  //低电平持续时间					
        }
#endif
    } else {
        t2_time = drv_systick_get();                                         //触发低电平时刻                                                                           
        vd_bsp_gpio_requst_irq(PROJECT_ISDN_PIN,VD_GPIO_IRQ_MODE_RISING,isdn_pin_isr,vd_null); //触发高电平时刻
        vd_bsp_gpio_irq_enable(PROJECT_ISDN_PIN,VD_GPIO_IRQ_ENABLE);

        if ((t2_time - t1_time >= 10000) && start_signal == 0) {
            start_signal = 2;  //起始信号
        } else if (t2_time != 0 && start_signal == 1) {
            low_time = t1_time - t2_time;  //低电平持续时间
        }
    }
    if (high_time != NULL && low_time != NULL && start_signal == 1) {
        high_or_low = (low_time > high_time) ? HIGH : LOW;  //高低电平时间比例
        drv_isdn_interaction(high_or_low);                  //字节组合函数
        high_time = low_time = 0;                           //时间清空}
    }
}

/**
 * @brief : 字节组合函数
 *
 * @param bit_value: 传入的位数
 * @return vd_bool_t
 */
static vd_bool_t drv_isdn_interaction(vd_uint8_t bit_value) {
    vd_uint8_t byte_bit;
    byte_bit = bit_value;
    // isdn_sum[bit_num] = bit_value; //存入bit
    byte |= byte_bit << (7 - bit_num);
    bit_num++;
    if (bit_num == 8) {
        bit_num = 0;                //位数清零
                                     // printf("byte=%x\r",byte);
                                    // printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r");
        isdn_sum[byte_num] = byte;  //存入字节
        byte               = 0;     //字节清零
        byte_num++;                 //字节数+1
        if (byte_num / 12 == 1) {
            start_signal = 0;                                   //起始信号清除
            iservice_push_queue(PLATFORM_ISDN0, isdn_sum, 12,vd_null);  //往队列里面丢数据
            memset(&isdn_sum, 0, 12);
            byte_num = 0;
        }
    }
    return vd_true;
}

/**
 * @brief : 通过isdn_id索引查找驱动
 *
 * @param isdn_id:一线通索引id
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
 * @brief : 一线通初始化内部函数
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
 * @brief : 一线通读队列函数
 *
 * @param isdn_id: 一线通索引id
 * @param data : 读取缓存队列中的数据
 * @param data_len : 需要读取的数据长度
 * @return vd_uint16_t : 实际读取的数据长度
 */
vd_uint16_t drv_isdn_read(vd_uint8_t isdn_id, vd_uint8_t* data, vd_uint16_t data_len) 
{
    drv_isdn_t* drv_isdn;
    drv_isdn = drv_isdn_find(isdn_id);
    vd_check_return_val_error(drv_isdn == vd_null, vd_false, "PLATFORM_ISDN %d OFF\r\n", isdn_id);
    return vd_queue_pull(&drv_isdn->rx_queue, data, data_len);
}

/**
 * @brief : 设置一线通当前状态
 *
 * @param isdn_state 每收到一次，将其置为0；
 */
vd_void_t drv_set_isdn_state(vd_uint8_t isdn_state) 
{
	drv_isdn_statt_cnt = isdn_state;
}

/**
 * @brief : 设置一线通当前状态
 *
 * @param isdn_state 每收到一次，将其置为0；
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
 * @brief : 一线通初始化接口函数
 *
 * @param isdn_id:一线通索引id
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
 * @brief : 一线通初始化函数
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
 * @brief : 一线通解析处理函数
 *
 * @param void
 * @return
 */

#define start_low_level   450  //起始的低电平 ，100us定时器
#define start_high_level  17   //起始的高电平，100us定时器


 


uint8_t isnd_level = 0;  //引脚电平
#if 1

//在time中去使用以下方式
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

uint8_t isndbuff[20] = {0};  //一线通数据   12字节
uint8_t isndbufflen  = 0;    //一线通数据长度

uint8_t rebit = 0;
uint8_t XOR   = 0;  //异或值

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
    if (test_L_len == 96 && test_H_len == 96)  //接收到一帧数据。12字节*8bit
    {
        memcpy(data_L_buff, test_L_buff, test_L_len * 2);  //复制
        memcpy(data_H_buff, test_H_buff, test_H_len * 2);  //复制
        data_L_len = test_L_len;
        data_H_len = test_H_len;
        test_L_len = 0;
        test_H_len = 0;

        for (int i = 0; i < 96; i++) {
            // printf("L[%d]=%02d  H[%d]=%02d  \r\n",i,data_L_buff[i],i,data_H_buff[i]);

#if 1
            if (data_L_buff[i] > start_low_level &&data_H_buff[i] > start_high_level)  //防止同步信号未在定时器里过滤成功，一般不会进这里
            {
                rebit = 0;
                dalen = 0;
                printf("Sync error \r\n");  //同步信号
            } 
						else 
						{
                if (data_L_buff[i] > data_H_buff[i])  //对比获取一个bit
                {
                    // databuff[datalen] |=0;
                } 
								else if (data_L_buff[i] < data_H_buff[i])  //对比获取一个bit
                {
                    dabuff[dalen] |= 1;
                }
                rebit++;
                if (rebit == 8)  //接收到一个字节
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
                    if (dalen == 12)  //接收到12个字节
                    {
                        XOR = dabuff[0] ^ dabuff[1] ^ dabuff[2] ^ dabuff[3] ^ dabuff[4] ^ dabuff[5] ^ dabuff[6] ^
                              dabuff[7] ^ dabuff[8] ^ dabuff[9] ^ dabuff[10];
                        //printf("XOR=%02x\r\n",XOR);

                        // if(XOR==dabuff[11] && dabuff[0]==0x08 && dabuff[1]==0x61)//异或校验
                        if (XOR == dabuff[11])  //异或校验
                        {
                            // printf("%02x   %02x   success\r\n",dabuff[0],dabuff[1]  );
													
													
                            memcpy(isndbuff, dabuff, dalen);                    //复制		得到一线通数据
                            isndbufflen = dalen;                                //一线通数据长度
													
													//printf("isnd shache=%02x\r\n",isndbuff[4]);
													
                            iservice_push_queue(PLATFORM_ISDN0, isndbuff, 12,vd_null);  //往队列里面丢数据

                            //       									 memcpy(isnd_data_rec.isnd_buff,dabuff,dalen);//组包数据准备上传
                            //       									 isnd_data_rec.isnd_len=dalen;
                            // printf("a\r\n");
                            // mcu_isnd_parse(isndbuff,isndbufflen);//一线通数据解析
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
                    //       		   		  	if(relen==128)//异常
                    //       		   		  	{
                    //
                    //       		   		  	  relen=0;
                    //       		   					printf("error");
                    //       		   		  	}
                } else {
                    dabuff[dalen] = dabuff[dalen] << 1;  //移位
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