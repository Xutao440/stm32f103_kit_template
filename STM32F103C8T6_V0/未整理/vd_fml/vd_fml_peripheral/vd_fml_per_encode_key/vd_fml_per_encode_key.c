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

// ��ʱ������ֵ����
#define DEV_ENCODEKEY_DETECT_TIMER_MS    2
#define DEV_ENCODEKEY_GPIO_TRIGGER_TIMER   14 / DEV_ENCODEKEY_DETECT_TIMER_MS

// ����ʱ���趨
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
 * @brief : ���aio�|�l�Дຯ��
 *
 * @param void
 * @return
 */
static vd_bool_t encodekey_gpio_irq_callback(vd_void_t) {
    // log_debug("irq\r\n");

    if (0 == drv_key_code_state_get())  // ���IጷŠ�B
    {
        drv_key_code_start_set(1);      // �|�l�O����ʼ��̖
        // log_debug("key_code trigger\r\n");
    }
}

/**
 * @brief : ���aio�|�l�Дຯ��
 *
 * @param void
 * @return
 */

static vd_void_t drv_timer_ms_irq(vd_void_t) {
    if (1 == drv_key_code_start_get())  // �O����ʼ��̖�����^���ƽ������
    {
        key_timer_cnt++;
        if (key_timer_cnt >= DEV_ENCODEKEY_GPIO_TRIGGER_TIMER) {
            key_timer_cnt = DEV_ENCODEKEY_GPIO_TRIGGER_TIMER;
            drv_key_code_start_set(0);  // ����|�l��̖
            drv_key_code_state_set(1);  // ���Խ��Ք���

            // key_wait_state=1;//��ʼ��̖���ˣ��ٵȴ�50ms
            // log_debug("key code begin wait\r\n");
            // log_debug("begin rec key code\r\n");
        }

    } else {
        key_timer_cnt = 0;          // ����
        drv_key_code_start_set(0);  // ����|�l��̖
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
 * @brief : key�¼��ϱ�
 *
 * @param key
 */
static vd_void_t key_code_report(key_event_t* key_event) {
    key_event->ev_type = EV_SYS_KEY;
    event_push(key_event);
}

/**
 * @brief : ���밴����ʱ����ʼ��
 *
 * @param key
 */
static vd_void_t drv_encodekey_timer_init(vd_void_t)
{
    vd_fml_timer_register(TIME_CHANNEL_1,drv_encodekey_timer_callback,vd_null,DEV_ENCODEKEY_DETECT_TIMER_MS);
}

/**
 * @brief : ���밴��gpio��ʼ��
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
 * @brief : ���밴��gpio��ʼ��
 *
 * @param
 */
static vd_void_t drv_encodekey_irq_init(vd_void_t)
{
    vd_bsp_gpio_requst_irq(PROJECT_ENCODEKEY_IRQ_PIN,VD_GPIO_IRQ_MODE_RISING,encodekey_gpio_irq_callback,vd_null);
    vd_bsp_gpio_irq_enable(PROJECT_ENCODEKEY_IRQ_PIN,1);
}

/**
 * @brief : ���밴�����ߴ�����
 *
 * @param
 */
static vd_void_t drv_encodekey_suspend(vd_void_t)
{
 
    vd_bsp_gpio_unrequst_irq(PROJECT_ENCODEKEY_IRQ_PIN);
    vd_fml_timer_Enable(TIME_CHANNEL_1,drv_encodekey_timer_callback,0);

}

/**
 * @brief : ���밴�����Ѵ�����
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
 * @brief : ���a���I��ʼ������
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
 * @brief : ���a������a̎��
 *
 * @param void
 * @return
 */

vd_void_t vd_fml_per_encodekey_loop(vd_void_t) {
    key_event_t        key;
    vd_uint8_t         code          = 0;  // ԭʼ�M�����a
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
                    if (key_map[i].key_code != last_key.key_code)  // �Iֵ��һ�ӣ��ψ�
                    {
                        // log_debug("KEY_PRESS\r\n");
                        key.key_code = key_map[i].key_code;
                        key.key_flag = KEY_PRESS;  // ���I����
                        key_code_report(&key);

                        last_key.key_code = key.key_code;
                        last_key.key_flag = key.key_flag;
                        pre_timestamp     = common_time_get_current_time_ms();
                    } else  // �Iֵһ�ӣ��Д��Ƿ��L��
                    {
                        if (common_time_get_current_time_ms() - pre_timestamp > DEV_ENCODEKEY_KEY_LONG_PRESS_TIME)  // �����Дࠑ�L���¼�
                        {
                            // log_debug("KEY_LONG_PRESS\r\n");
                            key.key_code = key_map[i].key_code;
                            key.key_flag = KEY_LONG_3s_PRESS;  // �L���|�l
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
            key.key_flag = KEY_RELEASE;  // ���Iጷ�
            key.key_code = last_key.key_code;

            key_code_report(&key);

            last_key.key_code = 0;
            last_key.key_flag = key.key_flag;
            drv_key_code_state_set(0);  // ጷ���̖���o���I�|�l
        }
    }
}
#endif
