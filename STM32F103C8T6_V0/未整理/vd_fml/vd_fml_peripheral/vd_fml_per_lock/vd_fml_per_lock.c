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
//采用项目中的状态机
//#include "machine_pr015.h"

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
//querystatus_cmd_t       lock_cmd;
extern vd_uint8_t       lock_state;
vd_uint8_t              mcu_fire_up_state         = 0;  //点火状态
static vd_uint8_t       frist_flag                = 0;
vd_uint8_t              guard_against_theft_state = 0;  //测试//0;//防盗状态
vd_uint8_t              beep_sec                  = 0;  //防盗报警声
vd_uint8_t              electric_door_lock_state  = 0;  //电门锁状态
vd_uint8_t              anti_beep_flag            = 0;
vd_uint8_t              ele_lock_state            = 0;  //电门锁状态
// vd_uint8_t  lock_state=0;      // 防盗状态  0：解锁  1：锁车
vd_uint8_t fire_up_state = 0;  //点火状态

vd_uint8_t ele_lock_state_flag = 0;  //监控电门锁有没有打开标志位

vd_uint8_t        lock_on_sta_flag  = 0;  //监控锁车打开后电门锁有没有打开标志位
vd_uint8_t        lock_off_sta_flag = 0;  //解锁时监控电门锁状态 1：打开  0：关闭
static vd_uint8_t antitheft_state   = 0;

os_timer_t      test_timer;
static uint32_t flag_timer = 0;

vd_uint8_t loc_ele_control_state = 0;  //电门锁状态

static unsigned char  sensor_on_flag = 0;  // 0：震动传感器无震动      1：震动传感器震动了
static unsigned short sensor_cnt     = 0;
static unsigned short horn_cnt       = 0;  //喇叭标志位

static unsigned char level_inversion_cnt = 0;  //监控电平翻转次数，为防盗状态下锁电机做准备  2022-9-17
static unsigned char low_level_cnt = 0;  //监控低电平持续次数，为防盗状态下锁电机做准备   2022-9-17
static unsigned short times_cnt    = 0;

static unsigned char turn_wheel_flag = 0;  //轮子转动标志位

static unsigned char state_flag = 0;

static vd_uint8_t mechanical_lock = 0;  // 0:机械锁没插上   1:机械锁插上

/* ======================================================================================
 * helper
 */
// 帮助函数位置

/**
 * @brief :测试计数
 *
 * @param  arg
 * @return vd_void_t
 */
vd_void_t test_timer_func(vd_void_t* arg) {
    flag_timer++;
    // printf("flag_timer = %d\r",flag_timer);
}

/**
 * @brief :得到电门锁状态
 *
 * @param  arg
 * @return vd_void_t
 */
vd_uint8_t lock_ele_get_state(vd_uint8_t data) {
    ele_lock_state = data;
    // return ele_lock_state;
}

/**
 * @brief :得到防盗状态
 *
 * @param
 * @return vd_uint8_t
 */
vd_uint8_t get_antitheft_state(vd_void_t) {
    return antitheft_state;
}

/**
 * @brief 控制电门锁电平
 * @param data:控制电平
 * @return vd_uint8_t
 */
vd_uint8_t lock_ele_ctr_state(vd_uint8_t data) {
    loc_ele_control_state = data;
}

/**
 * @brief 返回控制电门锁电平
 * @param
 * @return vd_uint8_t
 */
vd_uint8_t lock_ele_control(vd_void_t) {
    return loc_ele_control_state;
}

/* ======================================================================================
 * private implementation
 */
// 私有函数实现位置

/* ======================================================================================
 * implementation
 */
// 函数实现位置

/**
 * @brief 锁车开关
 * @param
 * @return vd_uint8_t
 */
vd_uint8_t drv_lock_switch_on(vd_void_t) {
//    printf("ele_lock_state=%02x\r\n", ele_lock_state);
    if (1 == ele_lock_state)  //当前为点火状态 操作无效
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
            lock_off_sta_flag         = 0;  // 51电门锁低电平状态
            guard_against_theft_state = 1;  //??

            return lock_on_ok;
        }
    }
}

/**
 * @brief : lock handle 锁车处理 防盗后处理
 *

 * @return vd_void_t
// */

vd_void_t drv_lock_handle(vd_void_t) {
    //	unsigned char sendbuff[4]={0};
    //	unsigned char bufflen=0;

    if (1 == guard_against_theft_state)  //设防状态
    {
#if 1                                                //震动传感器  只报警
        if (1 ==  vd_bsp_gpio_read(31))  //震动传感器 高电平  有震动来的是高低电平，不是一直高
        {
            // printf("open zhendong\r\n");
            //			 times_cnt=0;
            //			 mcu_gpio_set_value(CAR_LOCKING_SIGNAL_PORT,CAR_LOCKING_SIGNAL_PIN,MCU_IO_HIGH);//打开电子电门锁

            sensor_on_flag = 1;  // 1：震动传感器震动了
            sensor_cnt     = 0;
        } else if (0 == vd_bsp_gpio_read(31))  //震动传感器 低电平    来的是高低电平
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

        if ((1 == sensor_on_flag) || (1 == turn_wheel_flag))  //震动传感器震动了 发指令让cpu发警报
        {
            
            
           // extern vd_uint64_t pre_warning_time;  引用空闲状态机时需要打开
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

#if 1                                                //轮动检测  打开电子电门锁 才能锁电机
        if (0 ==  vd_bsp_gpio_read(22))  //轮动检测  有轮动
        {
#if 1  //低电平持续时间
            low_level_cnt++;

#endif

#if 1  // 9-17 监控电平反转
            if ((level_inversion_cnt == 0) || (level_inversion_cnt == 2) || (level_inversion_cnt == 4)) {
                level_inversion_cnt++;
            }

#endif

            if ((level_inversion_cnt == 5) || (low_level_cnt > 8))  // 9-17  翻转次数4次 或者低电平持续500ms
            {
                printf("level_inversion_cnt=%02x  low_level_cnt=%02x\r\n ", level_inversion_cnt, low_level_cnt);
                level_inversion_cnt = 0;  //清除翻转次数  9-17
                low_level_cnt       = 0;  //清除低电平持续时间  9-17

                printf("close suo1\r\n");
                // mcu_gpio_set_value(CAR_LOCKING_SIGNAL_PORT,CAR_LOCKING_SIGNAL_PIN,MCU_IO_HIGH);//打开电子电门锁
                // 打开了才能锁电机
                // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_HIGH);//锁电机信号
                if (state_flag == 0) {
                    state_flag                    = 1;
//实际项目中打开    lock_cmd.device_electric_lock = 1;
                    lock_ele_ctr_state(1);
                    // printf("loc_ele_control_state=%02x\r\n", loc_ele_control_state);
                    ele_lock_state_flag = 1;
                    lock_on_sta_flag    = 1;
                    antitheft_state     = 1;
                }
                vd_bsp_gpio_write(23,VD_GPIO_LOW);  //
                // no_turn_flag=1;//有轮动过
                turn_wheel_flag = 1;  //有轮动过
                times_cnt       = 0;
            }

        } else if (1 == vd_bsp_gpio_read(22))  //轮动检测  无轮动
        {
#if 1  // 9-17 监控电平反转
            if ((level_inversion_cnt == 1) || (level_inversion_cnt == 3)) {
                level_inversion_cnt++;
            }
            low_level_cnt = 0;  //清除低电平持续时间
#endif

            times_cnt++;
            if (150 == times_cnt)  // 10秒
            {
                if (state_flag == 1) {
                    state_flag                    = 0;
//实际项目需要打开  lock_cmd.device_electric_lock = 0;
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

                level_inversion_cnt = 0;  //清除翻转次数
                // mcu_gpio_set_value(CAR_LOCKING_SIGNAL_PORT,CAR_LOCKING_SIGNAL_PIN,MCU_IO_LOW);//关闭电子电门锁
                //  mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//解锁电机信号
            }
        }

#endif
    }
}

/**
 * @brief : lock switch off  解锁开关
 *

 * @return vd_uint8_t
// */

vd_uint8_t drv_lock_switch_off(void) {
    if (1 == guard_against_theft_state)  //当前为设防状态
    {
        if (1 == fire_up_state)  //当前既是点火状态，又是设防状态
        {
            // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//解锁电机信号
           vd_bsp_gpio_write(23, VD_GPIO_HIGH);
            lock_ele_ctr_state(1);
            // printf("loc_ele_control_state=%02x\r\n",loc_ele_control_state);
            //			   mcu_gpio_set_value(CAR_BUZZER_B_PORT,CAR_BUZZER_B_PIN,MCU_IO_LOW);//蜂鸣器B
            //			   mcu_gpio_set_value(CAR_BUZZER_A_PORT,CAR_BUZZER_A_PIN,MCU_IO_LOW);//蜂鸣器A

        } else if (0 == fire_up_state)  //当前是熄火状态，又是设防状态
        {
            // mcu_gpio_set_value(CAR_LOCK_MOTOR_SIGNAL_PORT,CAR_LOCK_MOTOR_SIGNAL_PIN,MCU_IO_LOW);//解锁电机信号
            // mcu_gpio_set_value(CAR_LOCKING_SIGNAL_PORT,CAR_LOCKING_SIGNAL_PIN,MCU_IO_LOW);//关闭电子电门锁
          vd_bsp_gpio_write(23, VD_GPIO_HIGH);
            lock_ele_ctr_state(1);
            // printf("loc_ele_control_state=%02x\r\n",loc_ele_control_state);
            //			  mcu_gpio_set_value(CAR_BUZZER_B_PORT,CAR_BUZZER_B_PIN,MCU_IO_LOW);//蜂鸣器B
            //			  mcu_gpio_set_value(CAR_BUZZER_A_PORT,CAR_BUZZER_A_PIN,MCU_IO_LOW);//蜂鸣器A
        }
        lock_off_sta_flag = 1;
        printf("now mcu_remove_guard_against_theft\r\n");
        guard_against_theft_state = 0;  //解防，对震动传感器，轮动状态不做处理

#if 1
        sensor_on_flag = 0;  // 0：震动传感器无震动      1：震动传感器震动了
        sensor_cnt     = 0;
        horn_cnt       = 0;  //喇叭标志位

        level_inversion_cnt = 0;  //监控电平翻转次数，为防盗状态下锁电机做准备  2022-9-17
        low_level_cnt       = 0;  //监控低电平持续次数，为防盗状态下锁电机做准备   2022-9-17
        times_cnt           = 0;

        turn_wheel_flag = 0;  //轮子转动标志位

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

    } else if (0 == guard_against_theft_state)  //当前为解防状态 无需操作
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
 * @brief : 机械锁解锁
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
        sensor_on_flag = 0;  // 0：震动传感器无震动      1：震动传感器震动了
        sensor_cnt     = 0;
        horn_cnt       = 0;  //喇叭标志位

        level_inversion_cnt = 0;  //监控电平翻转次数，为防盗状态下锁电机做准备  2022-9-17
        low_level_cnt       = 0;  //监控低电平持续次数，为防盗状态下锁电机做准备   2022-9-17
        times_cnt           = 0;

        turn_wheel_flag = 0;  //轮子转动标志位

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
 * @brief : 检测机械锁 解防盗
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
    if ((1 == ele_lock_state) && (1 == lock_off_sta_flag))  //解锁后io保持高电平，机械锁锁上无需操作
    {
        return 0;
    }

    if ((1 == ele_lock_state) && (1 == guard_against_theft_state) &&
        (0 == lock_on_sta_flag))  //防盗打开，没触发锁车轮，io没拉高。但是电门锁有电，机械锁打开了
    {
        // time_cnt++;
        // if (time_cnt == 30) {
        time_cnt = 0;
        // drv_lock_switch_off();//解锁
        drv_lock_switch_off_t();
        mechanical_lock = 1;
        printf("jixiesuo jiesuo\r\n");
        return 1;
        // }
    }

    if ((1 == ele_lock_state) && (1 == guard_against_theft_state) &&
        (1 == lock_on_sta_flag))  //防盗打开，触发锁车轮，io拉高。电门锁有电，机械锁也打开了，这里怎么判断
    {
        time_cnt++;
        if (time_cnt == 30) {
            time_cnt       = 0;
            anti_beep_flag = 1;
            printf("todo\r\n");
            // tido 这里要怎么做
            //		   drv_lock_switch_off();
            //		   return 1;
        }
    }

#endif

#if 1  //在监控到机械锁插上后，机械锁拔开进锁车

    if ((0 == ele_lock_state) && (1 == mechanical_lock)) {
        mechanical_lock = 0;
        drv_lock_switch_on();  //进锁车
    }
#endif
}

/**
 * @brief :得到机械锁状态
 *
 * @param
 */
vd_uint8_t get_mechanical_lock_state(vd_void_t) {
    return mechanical_lock;
}

/**
 * @brief : lock事件上报
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
 * @brief : 防盗loop
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
 * @brief : 防盗初始化
 *
 * @return vd_bool_t
// */
vd_bool_t drv_lock_init(void) {
   
    vd_bsp_gpio_init(23,VD_GPIO_MODE_INPUT_PULLUP);
    
    //轮子检测  输入

    vd_bsp_gpio_init(22,VD_GPIO_MODE_INPUT);
    
    vd_bsp_gpio_init(31,VD_GPIO_MODE_INPUT);
	
    vd_bsp_gpio_write(23,VD_GPIO_HIGH);
}

/**
 * @brief : 防盗轮子抱死
 * @param st,抱死状态，0，松开；1，抱死
 * @return vd_bool_t
 */
vd_void_t drv_lock_wheel_out(vd_uint8_t st){
    vd_bsp_gpio_write(23,!st);
}

/**
 * @brief : 防盗轮子检测
 * @param 
 * @return 轮动转态，0，轮动；1，无轮动
 */
vd_uint8_t drv_lock_wheel_input(vd_void_t){
	 return vd_bsp_gpio_read(22);
}



