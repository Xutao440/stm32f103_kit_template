/*
 * Project: mcu-kit
 * Module: drv_lcd 工具
 * File: drv_lcd_util.c
 * Created Date: 2022-04-22 19:37:17
 * Author: meizhen
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
#include "drv_lcd_util.h"

#include "drv_lcd.h"
//#include "drv_lcd_pic.h"
#include "drv_lcd_word.h"
#include "spi/drv_spi.h"

/* ======================================================================================
 * macros
 */

#define SIZE       16
#define XLevelL    0x02
#define XLevelH    0x10
#define Max_Column 128
#define Max_Row    64
#define Brightness 0xFF
#define X_WIDTH    128
#define Y_WIDTH    64
/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/* ======================================================================================
 * implementation
 */

//在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// mode:0,反白显示;1,正常显示
// size:选择字体 16/12
vd_void_t drv_lcd_show_char(vd_uint16_t x, vd_uint16_t y, vd_uint8_t chr) {
    vd_uint8_t c = 0, i = 0;
    c = chr - ' ';  //得到偏移后的值

    vd_uint16_t height = g_lcd_plat_drv.lcd_height;
    if (x > Max_Column - 1) {
        x = 0;
        y = y + 2;
    }

    if (SIZE == 16) {
        drv_lcd_set_pos(x, y, 0, 0);
        // drv_lcd_set_pos(x, height - y - 1, 0, 0);
        for (i = 0; i < 8; i++) {
            drv_lcd_write_data8(words_single_8x16[c * 16 + i]);
        }
        drv_lcd_set_pos(x, y + 1, 0, 0);
        // drv_lcd_set_pos(x, height - y - 1, 0, 0);
        for (i = 0; i < 8; i++) {
            drv_lcd_write_data8(words_single_8x16[c * 16 + i + 8]);
        }
    } else {
        drv_lcd_set_pos(x, y, 0, 0);
        // drv_lcd_set_pos(x, height - y - 1, 0, 0);
        for (i = 0; i < 6; i++) {
            drv_lcd_write_data8(words_single_6x8[c][i]);
        }
    }
}

// m^n函数
vd_uint32_t oled_pow(vd_uint8_t m, vd_uint8_t n) {
    vd_uint32_t result = 1;
    while (n--) result *= m;
    return result;
}

/**
 * @brief : 显示2个数字
 *
 * @param x
 * @param y
 * @param num:数值(0~4294967295);
 * @param len:数字的位数
 * @param size2:字体大小
 */
vd_void_t drv_lcd_show_number(vd_uint8_t x, vd_uint8_t y, vd_uint32_t num, vd_uint8_t len, vd_uint8_t size2) {
    vd_uint8_t t, temp;
    vd_uint8_t enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                drv_lcd_show_char(x + (size2 / 2) * t, y, ' ');
                continue;
            } else
                enshow = 1;
        }
        drv_lcd_show_char(x + (size2 / 2) * t, y, temp + '0');
    }
}

/**
 * @brief : 显示字符串
 *
 * @param x
 * @param y
 * @param chr
 */
vd_void_t drv_lcd_show_string(vd_uint16_t x, vd_uint16_t y, vd_uint8_t* str) {
    vd_uint8_t j = 0;
    while (str[j] != '\0') {
        drv_lcd_show_char(x, y, str[j]);
        x += 8;
        if (x > 120) {
            x = 0;
            y += 2;
        }
        j++;
    }
}

/**
 * @brief : 显示单个汉字
 *
 * @param x
 * @param y
 * @param no
 */
vd_void_t drv_lcd_show_chinese(vd_uint8_t x, vd_uint8_t y, vd_uint8_t no) {
    vd_uint8_t  t, adder = 0;
    vd_uint16_t height = g_lcd_plat_drv.lcd_height;

    // drv_lcd_set_pos(x, y, 0, 0);
    drv_lcd_set_pos(x, height - y - 1, 0, 0);

    for (t = 0; t < 16; t++) {
        drv_lcd_write_cmd8(chinese_single[2 * no][t]);
        adder += 1;
    }
    // drv_lcd_set_pos(x, y + 1, 0, 0);
    drv_lcd_set_pos(x, height - y - 1, 0, 0);
    for (t = 0; t < 16; t++) {
        drv_lcd_write_cmd8(chinese_single[2 * no + 1][t]);
        adder += 1;
    }
}

/**
 * @brief : 显示BMP图片
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param BMP
 */
vd_void_t drv_lcd_show_bmp(vd_uint16_t x0, vd_uint16_t y0, vd_uint16_t x1, vd_uint16_t y1, vd_uint8_t* data) {
    unsigned int j = 0;
    vd_uint8_t   x, y;

    vd_uint16_t height = g_lcd_plat_drv.lcd_height;
    vd_uint16_t width  = g_lcd_plat_drv.lcd_width;
    log_debug("show_bmp:width=%d,height=%d", width, height);

    // if (y1 % height == 0) {
    //     y = y1 / height;
    // } else {
    //     y = y1 / height + 1;
    // }

    for (y = y0; y < y1; y++) {
        drv_lcd_set_pos(x0, height - y - 1, 0, 0);
        // drv_lcd_write_data(&data[j], width);
        // j += width;
        for (x = x0; x < x1; x++) {
            drv_lcd_write_data8(data[j++]);
        }
    }
}

// extern unsigned char image_color;
//  extern unsigned char pic96x48_sigle;
/**
 * @brief : 调屏测试接口
 *
 * @return vd_void_t
 */
vd_void_t drv_lcd_test(vd_void_t) {
    mcu_delay(10000 * 1000);
    // drv_lcd_show_char(0, 0, 'A');
    // drv_lcd_show_char(0, 2, 'B');
    // drv_lcd_show_char(0, 4, 'C');
    // drv_lcd_show_char(0, 6, 'D');
    // drv_lcd_show_char(20, 0, 'a');
    // drv_lcd_show_char(20, 2, 'b');
    // drv_lcd_show_char(20, 4, 'c');
    // drv_lcd_show_char(20, 8, 'd');

    drv_lcd_show_string(2, 1, "mcu-kit");

    // mcu_delay(10000 * 1000);
    // drv_lcd_show_bmp(0, 0, 96, 12, (vd_uint8_t*)image_color);
    // drv_lcd_clean();
    //drv_lcd_show_bmp(0, 0, 96, 6, (vd_uint8_t*)pic96x48_sigle);
}