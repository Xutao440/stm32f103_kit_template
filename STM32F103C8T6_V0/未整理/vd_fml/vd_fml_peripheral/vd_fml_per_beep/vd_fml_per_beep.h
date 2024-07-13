/*
 * Project: ble_freertos_demo_pr008_9_cJson
 * Module: drv_buzzer
 * File: drv_buzzer.c
 * Created Date: 2022-08-30 09:37:54
 * Author: baoye
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */
#ifndef drv_buzzer_DRV_BUZZER_H
#define drv_buzzer_DRV_BUZZER_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "vd_bsp_type.h"
/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */


/* ======================================================================================
 * types
 */
typedef enum {
     PLATFORM_BEEP0 = 0,
     PLATFORM_BEEP1,
     PLATFORM_BEEP2, 
     PLATFORM_BEEPMAX 
 } plat_beep_id_e;
typedef struct 
{
   plat_beep_id_e vd_fml_per_beep_id;
   vd_bsp_port_e vd_fml_per_beep_port;
}drv_beep_t;


typedef enum {
    BZ_NO = 0,  //关
    BZ_KEY,     //按键音
    BZ_ON,      //开机音
    BZ_OFF,     //关机音
    BZ_ERR,     //报警音
    BZ_PON,
} DRV_BZ_SELCET_E;

typedef enum {
    BUZEND   = 0,  //蜂鸣器响铃结束
    BUZSTART = 1,  //蜂鸣器响铃开始
} DRV_BZ_STATE_E;

typedef struct {
    vd_uint16_t frq_select;  //蜂鸣器频率选择
    vd_uint16_t clock_time;  //蜂鸣器周期
} drv_bz_t;

typedef struct 
{
   DRV_BZ_SELCET_E beep_voice_type;
   drv_bz_t beep_voice_table[20];
}beep_voice_map_t;

/* ======================================================================================
 * declaration
 */
//beep paly music
vd_void_t vd_fml_per_beep_play(plat_beep_id_e beep_id,DRV_BZ_SELCET_E bz_mode);

//beep init
vd_void_t vd_fml_per_beep_init();

// beep control (id ,frequency,duty )
vd_void_t vd_fml_per_beep_control(plat_beep_id_e beep_id,uint16_t clock, uint16_t period); 

//open beep
vd_void_t vd_fml_per_beep_on(plat_beep_id_e beep_id);

//close beep
vd_void_t vd_fml_per_beep_off(plat_beep_id_e beep_id);
/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // drv_buzzer_DRV_BUZZER_H