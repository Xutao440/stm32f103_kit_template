/*
 * Project: ble_freertos_demo_pr008_9_cJson
 * Module: drv_buzzer
 * File: drv_buzzer.c
 * Created Date: 2022-08-30 09:38:08
 * Author: baoye
 * Description: 蜂鸣器驱动实现和旋音
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (0)

/* ======================================================================================
 * includes
 */
#include "vd_fml_per_beep.h"
#include "project_config.h"
#include "vd_bsp_pwm/vd_bsp_pwm.h"
#include "driver_system.h"
#include "sys_utils.h"
/* ======================================================================================
 * macros
 */
#if(PROJECT_BEEP == ON)
/* ======================================================================================
 * types
 */

vd_uint8_t          bz_flag;      //蜂鸣器标志位
drv_bz_t            bz_one;       //一个蜂鸣器
const drv_bz_t*     bz_disp;      //列表指针
static uint16_t     duty = 0;     //占空比
DRV_BZ_STATE_E      bz_peri;      //提示音选择
vd_uint16_t beep_flag = 1;
/* ======================================================================================
 * declaration
 */

 beep_voice_map_t  beep_voice_map[]=BEEP_VOICE_MAP;
/* ======================================================================================
 * globals
 */
static drv_beep_t g_drv_beep[] = PROJECT_BEEP_MAP;

/* ======================================================================================
 * helper
 */
/**
 * @brief : 通过beep_id索引查找驱动
 *
 * @param beep_id:蜂鸣器索引id
 * @return drv_beep_t*
 */
static drv_beep_t* drv_beep_find(vd_uint8_t beep_id) {
    for (int i = 0; i < vd_array_size(g_drv_beep); i++) {
        if (g_drv_beep[i].vd_fml_per_beep_id == beep_id) {
            return &g_drv_beep[i];
        }
    }
    return vd_null;
}

/**
 * @brief : 蜂鸣器PWM控制
 * @param period : pwm周期
 * @param clock: pwm时钟频率
 * @return vd_void_t
 *
 */
 vd_void_t vd_fml_per_beep_control(plat_beep_id_e beep_id,uint16_t clock, uint16_t period) 
{
    drv_beep_t *beep_t;
    beep_t = drv_beep_find(beep_id);
    vd_bsp_pwm_init(beep_t->vd_fml_per_beep_port, clock,period);  //将是时钟放在pwm频率，周期放在占空比
}

/**
 * @brief open beep
 * 
 * @param beep_id 
 * @return ** vd_void_t 
 */
vd_void_t vd_fml_per_beep_on(plat_beep_id_e beep_id)
{
    drv_beep_t *beep_t;
    beep_t = drv_beep_find(beep_id);
    vd_bsp_pwm_start(beep_t->vd_fml_per_beep_port);
}

/**
 * @brief close beep
 * 
 * @param beep_id 
 * @return ** vd_void_t 
 */
vd_void_t vd_fml_per_beep_off(plat_beep_id_e beep_id)
{
    drv_beep_t *beep_t;
    beep_t = drv_beep_find(beep_id);
    vd_bsp_pwm_stop(beep_t->vd_fml_per_beep_port);
}

/* ======================================================================================
 * private implementation
 */
/**
 * @brief : 蜂鸣器一次参数输入,指针指向对应的表取对应的时间
 * @param mode:蜂鸣器声音选择模式
 * @return vd_void_t
 *
 */
static vd_uint8_t buzzer_input(DRV_BZ_SELCET_E mode) {
  //  bz_peri = BUZSTART;
    for(uint16_t i =0; i< vd_array_size(beep_voice_map);i++){
        if(mode == beep_voice_map[i].beep_voice_type){
            bz_disp =  beep_voice_map[i].beep_voice_table;
            return BUZSTART;
        }
    }
    return BUZEND;

}

/**
 * @brief : 蜂鸣器和旋音驱动
 * @param bz_mode:蜂鸣器声音选择
 * @return vd_void_t
 *
 */
vd_void_t vd_fml_per_beep_play(plat_beep_id_e beep_id,DRV_BZ_SELCET_E bz_mode) 
{
    drv_beep_t *beep_t;
    beep_t = drv_beep_find(beep_id);

    DRV_BZ_SELCET_E beep_mode;
   beep_mode = buzzer_input(bz_mode);
   while(1){
    switch (beep_mode) {
        case BUZEND:                     //蜂鸣器响铃结束
            __SYSTEM_PWM_CLK_DISABLE();  //关闭PWM时钟使能
            vd_bsp_pwm_stop(beep_t->vd_fml_per_beep_port);
            return;
        case BUZSTART:                       //蜂鸣器响铃开始
            __SYSTEM_PWM_CLK_ENABLE();       //打开PWM时钟使能
            bz_one = *bz_disp;               //取音调数据
            if (bz_one.frq_select != VPOFF)  //非结束符
            {
                vd_fml_per_beep_control(beep_id,bz_one.frq_select, bz_one.clock_time);  //开始播音
                #if LOG_ENABLE_DEBUG
                co_printf("fre=%d time =%d\r\n",bz_one.frq_select, bz_one.clock_time);
                #endif
                vd_fml_per_beep_on(beep_id);
                co_delay_100us(1000);
                bz_disp++;
            } else
                beep_mode = BUZEND;
            break;
        default:
            beep_mode = BUZEND;
            break;
    }
   }
}


/**
 * @brief beep init
 * 
 * @param frequency 
 * @param duty 
 * @return ** vd_void_t 
 */
vd_void_t vd_fml_per_beep_init()
{
    for(uint8_t i =0; i< vd_array_size(g_drv_beep);i++){
        vd_bsp_pwm_init(g_drv_beep[i].vd_fml_per_beep_port,0 ,0);  //将是时钟放在pwm频率，周期放在占空比
    }
}
/* ======================================================================================
 * implementation
 */

#endif