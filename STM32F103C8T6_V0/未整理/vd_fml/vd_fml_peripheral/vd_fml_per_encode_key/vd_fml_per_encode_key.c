/**
 * @file vd_fml_per_code_key.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "vd_fml_per_encode_key.h"
#include "project_config.h"
#include "vd_fml_time.h"
#include "key_code.h"
#include "vd_fml_per_key.h"
#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "system_config.h"
/* ======================================================================================
 * macros
 */
#if (PROJECT_ENCODEKEY_EN == ON)

// 定时器读键值设置
#define DEV_ENCODEKEY_DETECT_TIMER_MS    2
#define DEV_ENCODEKEY_GPIO_TRIGGER_TIMER   14 / DEV_ENCODEKEY_DETECT_TIMER_MS

// 长按时间设定
#define DEV_ENCODEKEY_KEY_LONG_PRESS_TIME  1000

/* ======================================================================================
 * types
 */
vd_uint8_t pin_sequence[PROJECT_ENCODEKEY_PIN_NUMBER] = PROJECT_ENCODEKEY_PIN_SEQUENCE;

/* ======================================================================================
 * declaration
 */
static vd_bool_t encodekey_gpio_irq_callback(vd_void_t);
static vd_void_t drv_encodekey_timer_callback(vd_void_t);
/* ======================================================================================
 * globals
 */

static vd_uint8_t key_code_start_signal = 0;

static vd_uint8_t key_code_state = 0;

static vd_uint8_t key_timer_cnt = 0;

static vd_uint8_t key_wait_timer_cnt = 0;

static vd_uint8_t key_wait_state = 0;

static encode_key_define_t key_map[] = {
    {0x01, KEY_1, vd_null},
    {0x02, KEY_2, vd_null},
    {0x03, KEY_3, vd_null},
    {0x04, KEY_4, vd_null},
    {0x05, KEY_5, vd_null},
    {0x06, KEY_6, vd_null},
    {0x07, KEY_7, vd_null},
    {0x08, KEY_8, vd_null},
};

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/**
 * @brief :
 *
 * @param void
 * @return
 */
static vd_bool_t drv_key_code_start_set(vd_uint8_t start) {
    key_code_start_signal = start;
}

/**
 * @brief :
 *
 * @param void
 * @return
 */
static vd_uint8_t drv_key_code_start_get(vd_void_t) {
    return key_code_start_signal;
}

/**
 * @brief :
 *
 * @param void
 * @return
 */

static vd_bool_t drv_key_code_state_set(vd_uint8_t state) {
    key_code_state = state;
}

/**
 * @brief :
 *
 * @param void
 * @return
 */
static vd_uint8_t drv_key_code_state_get(vd_void_t) {
    return key_code_state;
}

/**
 * @brief : 編碼io觸發中斷函數
 *
 * @param void
 * @return
 */
static vd_bool_t encodekey_gpio_irq_callback(vd_void_t) {
    // log_debug("irq\r\n");

    if (0 == drv_key_code_state_get())  // 按鍵釋放狀態
    {
        drv_key_code_start_set(1);      // 觸發監控起始信號
        // log_debug("key_code trigger\r\n");
    }
}

/**
 * @brief : 編碼io觸發中斷函數
 *
 * @param void
 * @return
 */

static vd_void_t drv_timer_ms_irq(vd_void_t) {
    if (1 == drv_key_code_start_get())  // 監控起始信號，此過程電平不爲低
    {
        key_timer_cnt++;
        if (key_timer_cnt >= DEV_ENCODEKEY_GPIO_TRIGGER_TIMER) {
            key_timer_cnt = DEV_ENCODEKEY_GPIO_TRIGGER_TIMER;
            drv_key_code_start_set(0);  // 清除觸發信號
            drv_key_code_state_set(1);  // 可以接收數據

            // key_wait_state=1;//起始信號來了，再等待50ms
            // log_debug("key code begin wait\r\n");
            // log_debug("begin rec key code\r\n");
        }

    } else {
        key_timer_cnt = 0;          // 消抖
        drv_key_code_start_set(0);  // 清除觸發信號
    }

}

/**
 * @brief : timer1_irq_callback
 *
 * @param vd_void_t
 * @return
 */
static vd_void_t drv_encodekey_timer_callback(vd_void_t) {
   drv_timer_ms_irq();
   vd_fml_per_encodekey_loop();
}

/**
 * @brief : key事件上报
 *
 * @param key
 */
static vd_void_t key_code_report(key_event_t* key_event) {
    key_event->ev_type = EV_SYS_KEY;
    event_push(key_event);
}

/**
 * @brief : 编码按键定时器初始化
 *
 * @param key
 */
static vd_void_t drv_encodekey_timer_init(vd_void_t)
{
    vd_fml_timer_register(TIME_CHANNEL_1,drv_encodekey_timer_callback,vd_null,DEV_ENCODEKEY_DETECT_TIMER_MS);
}

/**
 * @brief : 编码按键gpio初始化
 *
 * @param key
 */
static vd_void_t drv_encodekey_gpio_init(vd_void_t)
{
 //   drv_gpio_t      drv_gpio;

    vd_uint8_t      i = 0;

    for (i = 0; i < PROJECT_ENCODEKEY_PIN_NUMBER; i++) {
        vd_bsp_gpio_init(pin_sequence[i],VD_GPIO_MODE_INPUT);
    }
}

/**
 * @brief : 编码按键gpio初始化
 *
 * @param
 */
static vd_void_t drv_encodekey_irq_init(vd_void_t)
{
    vd_bsp_gpio_requst_irq(PROJECT_ENCODEKEY_IRQ_PIN,VD_GPIO_IRQ_MODE_RISING,encodekey_gpio_irq_callback,vd_null);
    vd_bsp_gpio_irq_enable(PROJECT_ENCODEKEY_IRQ_PIN,1);
}

/**
 * @brief : 编码按键休眠处理函数
 *
 * @param
 */
static vd_void_t drv_encodekey_suspend(vd_void_t)
{
 
    vd_bsp_gpio_unrequst_irq(PROJECT_ENCODEKEY_IRQ_PIN);
    vd_fml_timer_Enable(TIME_CHANNEL_1,drv_encodekey_timer_callback,0);

}

/**
 * @brief : 编码按键唤醒处理函数
 *
 * @param key
 */
static vd_void_t drv_encodekey_resume(vd_void_t)
{
    drv_encodekey_gpio_init();

    drv_encodekey_irq_init();

    drv_encodekey_timer_init();
}
/* ======================================================================================
 * implementation
 */

/**
 * @brief : 編碼按鍵初始化函数
 *
 * @param void
 * @return
 */
vd_bool_t vd_fml_per_encodekey_init(vd_void_t) {

    drv_encodekey_gpio_init();

    drv_encodekey_irq_init();

    drv_encodekey_timer_init();

   // drv_pm_ops_register("drv_encodekey",drv_encodekey_suspend,drv_encodekey_resume,DRV_PM_PRIORITY_LEVEL_DEFAULT);
    return vd_true;
}

/**
 * @brief : 編碼案件解碼處理
 *
 * @param void
 * @return
 */

vd_void_t vd_fml_per_encodekey_loop(vd_void_t) {
    key_event_t        key;
    vd_uint8_t         code          = 0;  // 原始組包編碼
    static vd_ulong_t  pre_timestamp = 0;
    static key_event_t last_key;
    vd_uint8_t         i = 0;
    // printf(" drv_encodekey_loop !!\n");

    if (1 == drv_key_code_state_get()) {
        for (i = 0; i < PROJECT_ENCODEKEY_PIN_NUMBER; i++) {
            code = code | ((vd_bsp_gpio_read(pin_sequence[i]) & 0x01) << i);
        }

        if (code != 0) {
            for (i = 0; i < (sizeof(key_map) / (sizeof(encode_key_define_t))); i++) {
                if (code == key_map[i].code) {
                    if (key_map[i].key_code != last_key.key_code)  // 鍵值不一樣，上報
                    {
                        // log_debug("KEY_PRESS\r\n");
                        key.key_code = key_map[i].key_code;
                        key.key_flag = KEY_PRESS;  // 按鍵按下
                        key_code_report(&key);

                        last_key.key_code = key.key_code;
                        last_key.key_flag = key.key_flag;
                        pre_timestamp     = common_time_get_current_time_ms();
                    } else  // 鍵值一樣，判斷是否長按
                    {
                        if (common_time_get_current_time_ms() - pre_timestamp > DEV_ENCODEKEY_KEY_LONG_PRESS_TIME)  // 兩秒判斷爲長按事件
                        {
                            // log_debug("KEY_LONG_PRESS\r\n");
                            key.key_code = key_map[i].key_code;
                            key.key_flag = KEY_LONG_3s_PRESS;  // 長按觸發
                            key_code_report(&key);
                            pre_timestamp = common_time_get_current_time_ms();
                        }
                    }

                    // key.key_code=key_map[i].key_code;
                    // key.key_flag = KEY_PRESS;
                    // key_code_report(&key);
                }
            }
        } else {
            // key.key_code=key_map[i].key_code;
            // log_debug("key code end\r\n");
            key.key_flag = KEY_RELEASE;  // 按鍵釋放
            key.key_code = last_key.key_code;

            key_code_report(&key);

            last_key.key_code = 0;
            last_key.key_flag = key.key_flag;
            drv_key_code_state_set(0);  // 釋放信號，無按鍵觸發
        }
    }
}
#endif
