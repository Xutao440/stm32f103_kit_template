/*
 * Project: fr8000
 * Module: nfc
 * File: drv_nfc.c
 * Created Date: 2022-10-18 20:17:06
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
#include "vd_fml_per_nfc.h"

#include "project_config.h"

#if 1  //(KIT_PLATFORM == KIT_FR8003D)
#include "Rfid_interface.h"

#include "emv_if.h"
#include "os_timer.h"
#include "sl2x23.h"
#include "string.h"
#include "system_config.h"
#include "types.h"

#include "vd_bsp_gpio/vd_bsp_gpio.h"
/* ======================================================================================
 * macros
 */
// 宏定义位置
#define NFC_RESET_PORT	30	//PD6

/* ======================================================================================
 * types
 */
// 类型定义位置
os_timer_t              nfc_timer, speed_limit_timer;

typedef struct {
    vd_uint8_t nfc_id_len;        // nfc?????
    vd_uint8_t nfc_id_buff[10];   // nfc ???buff
    vd_uint8_t nfc_id_len1;       // nfc?????
    vd_uint8_t nfc_id_buff1[10];  // nfc ???buff

    vd_uint8_t nfc_id_len2;       // nfc?????
    vd_uint8_t nfc_id_buff2[10];  // nfc ???buff
    vd_uint8_t nfc_id_len3;       // nfc?????
    vd_uint8_t nfc_id_buff3[10];  // nfc ???buff
    vd_uint8_t nfc_id_len4;       // nfc?????
    vd_uint8_t nfc_id_buff4[10];  // nfc ???buff

} local_nfc_data_t;

local_nfc_data_t local_nfc_data;  //本地保存的nfc信息


/* ======================================================================================
 * declaration
 */
// 函数定义位置

/* ======================================================================================
 * globals
 */
// 全局变量位置


vd_uint8_t recvdata  = 0;
vd_uint8_t recv_data = 0;

vd_uint8_t  nfc_0_cnt              = 0;  //接收不到nfcid后的计时
vd_uint8_t  nfc_0_flag             = 0;  //
vd_uint8_t  last_nfc_state         = 0;
vd_uint8_t  nfc_tim_state          = 0;  // nfc结束定时器开启状态
vd_uint64_t nfc_unlock_start_time  = 0;
vd_uint8_t  nfc_unlock_start_state = 0, speed_limit_state = 0, speed_limit_state_en = 0;
vd_uint8_t  speed_limit_cnt = 0, speed_limit_st = 0; 

vd_uint8_t   nfc_reset_flag=0;//nfc驱动复位标志位



volatile vd_uint8_t demo_step;
volatile vd_uint8_t g_ret;

static vd_uint8_t nfc_flag      = 0;  // 0:无效flag；1：解锁；2；上锁；3：
vd_uint8_t        beep_nfc_flag = 2;  // beep专用nfc_flag，默认上锁，1:解锁


extern vd_uint8_t  card_A;
extern vd_uint8_t  card_B;
extern vd_uint8_t  card_F;
extern vd_uint8_t  card_V;
extern vd_uint8_t  sak;
extern vd_uint16_t atqa;


vd_uint8_t lock_state;
extern nfc_data_t nfc_data;


/*============================================== ERROR ===========================================================*/
//extern config_table_t config_table;
/*================================================================================================================*/	
config_table_t config_table;


void vTaskDelay(uint32_t t)
{
	0;
}


/* ======================================================================================
 * helper
 */
// 帮助函数位置
extern void iouart1_delayUs(volatile vd_uint32_t nTime);



/**
 * @brief :nfc驱动
 *
 * @param
 *
 * @return vd_uint8_t
 */
vd_uint8_t nfc_demo(void) {
    static vd_uint8_t u_temp = 0, card_det = 0;
    vd_uint8_t        rate = 0;
   // demo_step              = 0;//2022-12-12
    //       while (1) {
    switch (demo_step) {
        case 0:  //询卡
                 // printf("nfc1\r\n");
            card_A = 0;
            card_B = 0;
            card_F = 1;
            card_V = 1;

            g_ask_block_flag = 0;

            pcd_antenna_off();

#if 0  // 2022-9-14									 
      pcd_sleep();       // Soft power off

      vTaskDelay(60);  // 2022-7-25
      pcd_sleep_exit();  // Soft power on
#endif
            pcd_set_rate(1);
            g_ret = pcd_polling(g_tag_info.serial_num);
            // printf("nfc5\r\n");
            if (g_ret == 0) {
                demo_step = 1;
                //    rate = rate%4;
                //    rate++;
                rate = 1;
#ifdef _ENABLE_LPCD_
                u_temp = 1;
#endif
							//printf("demo_step1=%d\r\n",demo_step);
                break;
            } else {
                demo_step = 0;
                card_det  = 0;
							//	printf("demo_step0=%d\r\n",demo_step);
                return card_det;
            }
        case 1:

#if 1  // 2022-9-14		
                if (card_A) {

                    if ((sak == 0x08) || (sak == 0x18)) {  // MIFARE

  											NFC_A_Mifare_Test(g_tag_info.serial_num);

                        demo_step = 0;
                        break;
                    }
		
//                    if (atqa == 0x000c) {
//                        NFC_A_Tag1_Test(g_tag_info.serial_num);
//                        demo_step = 0;
//                        break;
//                    }
//                    if (atqa == 0x4400) {
//                        NFC_A_Tag2_Test(g_tag_info.serial_num);
//                        demo_step = 0;
//                        break;
//                    }
                }

#endif
            g_ret = pcd_active(rate);
            if (g_ret == 0) {
                demo_step = 2;
            } else {
                demo_step = 0;
            }
            break;
        case 2:
            if (card_A) {
                Iblock_getrand(rate);
            }
            if (card_B) {
                g_ask_block_flag = 1;
                g_ret            = get_idcard_num(g_tag_info.serial_num);
                if (g_ret == 0) {
                    g_tag_info.uid_length = 10;
                    PRINT_BUF("ID_NUM: ", g_tag_info.serial_num, g_tag_info.uid_length);
                    printf("## test end##\r\n\r\n");
                } else {
                    Iblock_getrand(rate);
                }
            }

#if 0
                if(card_F) {
                    NFC_F_Tag3_Test(g_tag_info.serial_num);
                }
                if(card_V) {
                    NFC_V_Tag5_Test(g_tag_info.serial_num);
                }
#endif

            demo_step = 0;
            //  return 1;
            break;
        default:
#ifdef FPGA
            if (rate == 1) {
                P1OUT &= ~BIT7;
            } else {
                P1OUT |= BIT7;
            }
#endif
            pcd_set_rate(1);
            pcd_antenna_off();
            // delay_1ms(3);//2022-7-25
            vTaskDelay(1);  // 2022-7-25		
            pcd_antenna_on();
            demo_step = 0;
            break;
    }
    if (demo_step == 0) {
        // delay_1ms(600);
    }
    //        if (1 == u_temp) {
    //            card_det = 1;
    //            break;
    //        }
    //    }
    return card_det;
}

/* ======================================================================================
 * private implementation
 */
// 私有函数实现位置

/* ======================================================================================
 * implementation
 */
// 程序入口

/**
 * @brief :nfc复位信号
 *
 * @param nfc nfc数据上报
 * @return vd_void_t
 */
vd_void_t nfc_reset_state(vd_uint8_t state)
{

  nfc_reset_flag=state;

}

/**
 * @brief : 获取nfc复位信号
 *
 * @param nfc nfc数据上报
 * @return vd_uint8_t
 */
vd_uint8_t get_nfc_reset_state(vd_void_t)
{

  return  nfc_reset_flag;

}



/**
 * @brief : nfc复位处理
 *
 * @param nfc nfc数据上报
 * @return vd_uint8_t
 */
vd_uint8_t nfc_reset_handle(vd_void_t)
{

  if(1==get_nfc_reset_state())
	{
		    printf("nfc  reset begin\r\n");
		    nfc_reset_state(0);
		
	      vd_bsp_gpio_write(NFC_RESET_PORT, 0);
        vTaskDelay(10);                         // 100ms

        
        vd_bsp_gpio_write(NFC_RESET_PORT, 1);  //复位
        vTaskDelay(1);                          // 10ms
        

        mcu_drv_init();

        vTaskDelay(2);  // 20ms
        

        Rfid_Init();

        pcd_default_info();
	
	}
}


/**
 * @brief : nfc事件上报
 *
 * @param nfc nfc数据上报
 * @return vd_void_t
 */
// vd_uint8_t nfc_tt=126;
static vd_void_t nfc_report(nfc_event_t* nfc) {
    // log_debug("knob_status %s\r\n", (knob_event->rotate == ROTATE_LEFT) ? "Left" : "Right");
    // log_debug("k_v = %d\r\n", (knob_event->rotate == ROTATE_LEFT) ? --knob_tt : ++knob_tt);
    // knob_event->ev_type = EV_SYS_KNOB;
    // printf("k_v = %d\r\n", (nfc->nfc_id_len == 11) ? --nfc_tt : ++nfc_tt);
    // printf("nfc->nfc_id_len=%d\r\n",nfc->nfc_id_len);
    // nfc->nfc_id_len=6;
    nfc->nfc_type = EV_SYS_NFC;
    event_push(nfc);
}

//nfc刷卡延时判断定时器使能相关变量及函数//
static vd_uint8_t nfc_timer_en = 0;
vd_void_t set_nfc_timer_en(vd_uint8_t en){
	nfc_timer_en = en;
}

vd_uint8_t get_nfc_timer_en(vd_void_t){
	return nfc_timer_en;
}

/**
 * @brief : NFC刷卡延时判断
 *
 * @param arg
 * @return vd_void_t
 */
vd_void_t nfc_tim_fn(vd_void_t* arg) {
    nfc_0_cnt++;
    if (nfc_0_cnt >= 130) {
				nfc_0_cnt= 0;
        last_nfc_state       = 0;
        nfc_tim_state        = 0;
        speed_limit_state_en = 0;
				set_nfc_timer_en(0);
				
    }
}

//nfc刷卡延时判断定时器使能相关变量及函数//
static vd_uint8_t speed_limit_timer_en = 0;
vd_void_t set_speed_limit_timer_en(vd_uint8_t en){
	speed_limit_timer_en = en;
}

vd_uint8_t get_speed_limit_timer_en(vd_void_t){
	return speed_limit_timer_en;
}


/**
 * @brief :速度限制
 *
 * @param arg
 * @return vd_void_t
 */
vd_void_t speed_limit_tim_fn(vd_void_t* arg) {
    speed_limit_cnt++;
    if (speed_limit_cnt > 2) {
//        gpio_write_pin(GPIO_C, GPIO_PIN_7, (speed_limit_cnt+1) & 1);
			vd_bsp_gpio_write(23, (speed_limit_cnt+1) & 1);
    }
    if (speed_limit_st == 0) {
        if (speed_limit_cnt > 17) {//8个波形，speed_limit_st=0，关闭超速提示，两声；即解除限速
						set_speed_limit_timer_en(0);
//            os_timer_stop(&speed_limit_timer);
//            os_timer_destroy(&speed_limit_timer);
          //  close_speed_warn();
        }
    } else {
        if (speed_limit_cnt > 11) {//5个波形，speed_limit_st=1，开启超速提示，一声；即恢复限速
						set_speed_limit_timer_en(0);
//            os_timer_stop(&speed_limit_timer);
//            os_timer_destroy(&speed_limit_timer);
          //  open_speed_warn();
        }
    }
}

/**
 * @brief : 得到nfc标志位
 *
 * @param
 * @return vd_uint8_t
 */
vd_uint8_t get_nfc_flag(void) {
    vd_uint8_t ref = nfc_flag;
    if (nfc_flag > 0) {
        nfc_flag = 0;
        return ref;
    } else {
        return 0;
    }
}

/**
 * @brief : nfc业务逻辑处理
 *
 * @param
 * @return vd_uint8_t
 */
 vd_uint8_t factory_nfc1[4] = {0x03, 0x66, 0x58, 0xe1};
 vd_uint8_t factory_nfc2[4] = {0xe1, 0x58, 0x66, 0x03};
 vd_uint8_t factory_nfc_flag = 0;
vd_void_t drv_nfc_deal(vd_void_t) {
    //     co_printf("nfc_data.nfc_state22 %d \r\n",nfc_data.nfc_state);
    if (nfc_data.nfc_state == 1) {
        //			co_printf("nfc_data.nfc_state11 %d \r\n",nfc_data.nfc_state);
				if(nfc_data.nfc_factory_flag == 1){   //工厂模式
					nfc_data.nfc_factory_flag = 0;
					factory_nfc_flag = 1;
				}else{
					if (last_nfc_state != nfc_data.nfc_state) {  //响应接收nfc
							vd_uint8_t nfc_st = 0;
							if (config_table.nfc.nfc_info_state & 1 << 0) {
									nfc_st |= !memcmp(config_table.nfc.nfc_id_buff0, nfc_data.nfc_id_buff, nfc_data.nfc_id_len);
							}
							if (config_table.nfc.nfc_info_state & 1 << 1) {
									nfc_st |= !memcmp(config_table.nfc.nfc_id_buff1, nfc_data.nfc_id_buff, nfc_data.nfc_id_len);
							}
							if (config_table.nfc.nfc_info_state & 1 << 2) {
									nfc_st |= !memcmp(config_table.nfc.nfc_id_buff2, nfc_data.nfc_id_buff, nfc_data.nfc_id_len);
							}
							if (config_table.nfc.nfc_info_state & 1 << 3) {
									nfc_st |= !memcmp(config_table.nfc.nfc_id_buff3, nfc_data.nfc_id_buff, nfc_data.nfc_id_len);
							}
							if (config_table.nfc.nfc_info_state & 1 << 4) {
									nfc_st |= !memcmp(config_table.nfc.nfc_id_buff4, nfc_data.nfc_id_buff, nfc_data.nfc_id_len);
							}

							if (nfc_st) {
//									co_printf("1.nfc_data.nfc_statessssssssssssss %d \r\n", nfc_data.nfc_state);
//									co_printf("2.lock_statezzzzzzzzzzzzzzzz %d \r\n", lock_state);
									if (lock_state) {
											nfc_flag      = 2;
											beep_nfc_flag = 2;
											//                      ble_lock();
											speed_limit_state_en = 0;
											//                    co_printf("lock_state %d, nfc_lock_flag %d \r\n", lock_state, nfc_lock_flag);
									} else {
											nfc_flag      = 1;
											beep_nfc_flag = 1;
											//                      ble_unlock();
											nfc_unlock_start_time = common_time_get_current_time_ms();
											speed_limit_state_en  = 1;
											//                    co_printf("lock_state %d, nfc_unlock_flag %d \r\n", lock_state,
											//                    nfc_unlock_flag);
									}
									last_nfc_state = nfc_data.nfc_state;
							}
					} else {
							nfc_data.nfc_state = 0;
							set_nfc_timer_en(0);
							nfc_tim_state = 0;

							if (common_time_get_current_time_ms() - nfc_unlock_start_time > SPEED_LIMIT_TIME && speed_limit_state_en ) {
									speed_limit_state_en = 0;
									if(drv_get_limit_en_type() == 1 )
									{
										drv_set_limit_en_type(0);
									}
									//							co_printf("lock_state
									//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \r\n");
									//! co_printf("speed_limit_state
									//%d!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \r\n",speed_limit_state);
									if (speed_limit_state) {
											speed_limit_st  = 0;             
											//									co_printf("lock_state
											// 2222222222!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \r\n");
									} else {  
											speed_limit_st  = 1;					
									}
									speed_limit_cnt = 0;                    
									set_speed_limit_timer_en(1);
									speed_limit_state ^= 1;
							}
					}
				}
    } else {
		
				factory_nfc_flag = 0; 														//工厂模式
        if (last_nfc_state == 1 && get_nfc_timer_en() == 0) {
            nfc_0_cnt     = 0;
            nfc_tim_state = 1;
						set_nfc_timer_en(1);
        }
    }
}

/**
 * @brief : 获取工厂模式刷特殊nfc状态
 *
 * @param
 * @return vd_uint8_t
 */
vd_uint8_t get_factory_test_nfc(vd_void_t){

	return factory_nfc_flag;
}
/* ======================================================================================
 * globals
 */
/**
 * @brief : nfc处理函数
 *

 * @return vd_void_t
// */
vd_uint16_t ret_cnt = 0;
vd_void_t   drv_nfc_loop(void) {
      nfc_demo();

#if 0  //重启
    ret_cnt++;
    if (ret_cnt == 600) {  //大概100s重启一次
        ret_cnt = 0;

        //          printf("rest nfc\r\n");
        gpio_write_pin(GPIO_D, GPIO_PIN_6, 0);
        vTaskDelay(10);                         // 100ms
        gpio_write_pin(GPIO_D, GPIO_PIN_6, 1);  //复位
        vTaskDelay(1);                          // 10ms

        mcu_drv_init();

        vTaskDelay(2);  // 20ms

        Rfid_Init();

        pcd_default_info();

        nfc_demo();
    }
#else
		
		nfc_reset_handle();
		
#endif
}
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/* ======================================================================================
 * implementation
 */
/**
 * @brief : nfc初始化
 *
 * @return vd_bool_t
// */

vd_bool_t drv_nfc_init(void) {
#if 0
	__SYSTEM_GPIO_CLK_ENABLE();
		//__SYSTEM_I2C0_CLK_ENABLE();
	  __SYSTEM_I2C1_CLK_ENABLE();
#if 1
	  /* I2C IO Init */
    //GPIO_Handle.Pin       = GPIO_PIN_0 | GPIO_PIN_1;
	  GPIO_Handle.Pin       =  GPIO_PIN_3;
    GPIO_Handle.Mode      = GPIO_MODE_AF_PP;
    GPIO_Handle.Pull      = GPIO_PULLUP;
    GPIO_Handle.Alternate = GPIO_FUNCTION_1;
	
	
	  GPIO_D6_Handle.Pin   =  GPIO_PIN_6;
  	GPIO_D6_Handle.Mode  =  GPIO_MODE_AF_PP;
	  GPIO_D6_Handle.Pull  =  GPIO_PULLUP;
	  GPIO_D6_Handle.Alternate = GPIO_FUNCTION_1;
    //gpio_init(GPIO_D, &GPIO_Handle);
		gpio_init(GPIO_C, &GPIO_Handle);
	  gpio_init(GPIO_D, &GPIO_D6_Handle);
	
    //I2C0_Handle.I2Cx = I2C0;
	  I2C0_Handle.I2Cx = I2C1;
#if MASTER
    I2C0_Handle.Init.I2C_Mode = I2C_MODE_MASTER_7BIT;
	  printf("MASTER Init\r\n");
#else
		printf("SLAVE Init\r\n");
	  I2C0_Handle.Init.I2C_Mode = I2C_MODE_SLAVE_7BIT;
	  I2C0_Handle.Init.Slave_Address = 0xA7;
#endif
    I2C0_Handle.Init.SCL_HCNT = 50;
    I2C0_Handle.Init.SCL_LCNT = 50;
    i2c_init(&I2C0_Handle);
			
		NVIC_EnableIRQ(I2C1_IRQn);	//2022-7-26
		NVIC_SetPriority(I2C1_IRQn, 0);	//2022-7-26
#if !MASTER
		i2c_int_enable(&I2C0_Handle, INT_RX_FULL|INT_RD_REQ);
    NVIC_EnableIRQ(I2C0_IRQn);
    vTaskDelay(1000);
    __I2C_ENABLE(I2C0_Handle.I2Cx);
#endif
#endif

#if 1  // nfc
	vTaskDelay(1000);//100ms
	mcu_drv_init();
	vTaskDelay(1000);//100ms
	write_reg(0x28, 0x3a);
  printf("read_reg=%x\n", read_reg(0x28));
	vTaskDelay(20);
	printf("VersionReg 37=%x\n", read_reg(0x37));
  Rfid_Init();
	printf("Rfid_Init ok\r\n");
  pcd_default_info();
	printf("pcd_default_info ok\r\n");
  irq_flag_io = 0;

#endif

#endif

#if 1


		
		vd_bsp_gpio_init(NFC_RESET_PORT, VD_GPIO_MODE_OUTPUT);
		vd_bsp_gpio_write(NFC_RESET_PORT, 1);

    vTaskDelay(10);  // 10ms

    vTaskDelay(10);  // 100ms
    mcu_drv_init();
    vTaskDelay(10);  // 100ms
    // while(1)
    //{
    write_reg(0x28, 0x3a);
    printf("read_reg28=%x\n", read_reg(0x28));
    vTaskDelay(20);
    printf("VersionReg 37=%x\n", read_reg(0x37));
    vTaskDelay(20);
    //}
    Rfid_Init();
    printf("Rfid_Init ok\r\n");
    pcd_default_info();
    printf("pcd_default_info1 ok\r\n");
    irq_flag_io = 0;

#endif
}
#endif
