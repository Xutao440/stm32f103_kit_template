/*
 * Project: mcu-kit
 * Module: lcd chip driver
 * File: LCD_ST7789V3_ORIC_0144_240_240_SPI.c
 * Created Date: 2022-04-20 19:27:54
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
#include "base.h"
#include "vd_fml_per_lcd/vd_fml_per_lcd.h"
#include "log.h"
#include "platform_config.h"

#if (LCD_ST7789V3_ORIC_0144_240_240_SPI == ON)

/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */
LCD_DEFINE(LCD_ST7789V3_ORIC_0144_240_240_SPI);

/**
 * @brief : lcd初始化参数表
 *
 */
LCD_INIT_PARAM(LCD_ST7789V3_ORIC_0144_240_240_SPI) = {
    {LCD_CMD, 1, 0xEF},

    {LCD_CMD, 1, 0xEB},
    {LCD_DATA, 1, 0x14},

    {LCD_CMD, 1, 0xFE},
    {LCD_CMD, 1, 0xEF},

    {LCD_CMD, 1, 0xEB},
    {LCD_DATA, 1, 0x14},

    {LCD_CMD, 1, 0x84},
    {LCD_DATA, 1, 0x40},

    {LCD_CMD, 1, 0x85},
    {LCD_DATA, 1, 0xFF},

    {LCD_CMD, 1, 0x86},
    {LCD_DATA, 1, 0xFF},

    {LCD_CMD, 1, 0x87},
    {LCD_DATA, 1, 0xFF},

    {LCD_CMD, 1, 0x88},
    {LCD_DATA, 1, 0x0A},

    {LCD_CMD, 1, 0x89},
    {LCD_DATA, 1, 0x21},

    {LCD_CMD, 1, 0x8A},
    {LCD_DATA, 1, 0x00},

    {LCD_CMD, 1, 0x8B},
    {LCD_DATA, 1, 0x80},

    {LCD_CMD, 1, 0x8C},
    {LCD_DATA, 1, 0x01},

    {LCD_CMD, 1, 0x8D},
    {LCD_DATA, 1, 0x01},

    {LCD_CMD, 1, 0x8E},
    {LCD_DATA, 1, 0xFF},

    {LCD_CMD, 1, 0x8F},
    {LCD_DATA, 1, 0xFF},

    {LCD_CMD, 1, 0xB6},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x00},

    //		for(i = 0; i < 1000000; i++) __NOP(},
    {LCD_CMD, 1, 0x36},
    // if (horizontal == 0)
    {LCD_DATA, 1, 0x00},
    // else if (horizontal == 1){LCD_DATA, 1, 0xC0},
    // else if (horizontal == 2){LCD_DATA, 1, 0x70},
    // else {LCD_DATA, 1, 0xB0},

    {LCD_CMD, 1, 0x3A},
    {LCD_DATA, 1, 0x55},

    {LCD_CMD, 1, 0x90},
    {LCD_DATA, 1, 0x08},
    {LCD_DATA, 1, 0x08},
    {LCD_DATA, 1, 0x08},
    {LCD_DATA, 1, 0x08},

    {LCD_CMD, 1, 0xBD},
    {LCD_DATA, 1, 0x06},

    {LCD_CMD, 1, 0xBC},
    {LCD_DATA, 1, 0x00},

    {LCD_CMD, 1, 0xFF},
    {LCD_DATA, 1, 0x60},
    {LCD_DATA, 1, 0x01},
    {LCD_DATA, 1, 0x04},

    {LCD_CMD, 1, 0xC3},
    {LCD_DATA, 1, 0x13},
    {LCD_CMD, 1, 0xC4},
    {LCD_DATA, 1, 0x13},

    {LCD_CMD, 1, 0xC9},
    {LCD_DATA, 1, 0x22},

    {LCD_CMD, 1, 0xBE},
    {LCD_DATA, 1, 0x11},

    {LCD_CMD, 1, 0xE1},
    {LCD_DATA, 1, 0x10},
    {LCD_DATA, 1, 0x0E},

    {LCD_CMD, 1, 0xDF},
    {LCD_DATA, 1, 0x21},
    {LCD_DATA, 1, 0x0c},
    {LCD_DATA, 1, 0x02},

    {LCD_CMD, 1, 0xF0},
    {LCD_DATA, 1, 0x45},
    {LCD_DATA, 1, 0x09},
    {LCD_DATA, 1, 0x08},
    {LCD_DATA, 1, 0x08},
    {LCD_DATA, 1, 0x26},
    {LCD_DATA, 1, 0x2A},

    {LCD_CMD, 1, 0xF1},
    {LCD_DATA, 1, 0x43},
    {LCD_DATA, 1, 0x70},
    {LCD_DATA, 1, 0x72},
    {LCD_DATA, 1, 0x36},
    {LCD_DATA, 1, 0x37},
    {LCD_DATA, 1, 0x6F},

    {LCD_CMD, 1, 0xF2},
    {LCD_DATA, 1, 0x45},
    {LCD_DATA, 1, 0x09},
    {LCD_DATA, 1, 0x08},
    {LCD_DATA, 1, 0x08},
    {LCD_DATA, 1, 0x26},
    {LCD_DATA, 1, 0x2A},

    {LCD_CMD, 1, 0xF3},
    {LCD_DATA, 1, 0x43},
    {LCD_DATA, 1, 0x70},
    {LCD_DATA, 1, 0x72},
    {LCD_DATA, 1, 0x36},
    {LCD_DATA, 1, 0x37},
    {LCD_DATA, 1, 0x6F},

    {LCD_CMD, 1, 0xED},
    {LCD_DATA, 1, 0x1B},
    {LCD_DATA, 1, 0x0B},

    {LCD_CMD, 1, 0xAE},
    {LCD_DATA, 1, 0x77},

    {LCD_CMD, 1, 0xCD},
    {LCD_DATA, 1, 0x63},

    {LCD_CMD, 1, 0x70},
    {LCD_DATA, 1, 0x07},
    {LCD_DATA, 1, 0x07},
    {LCD_DATA, 1, 0x04},
    {LCD_DATA, 1, 0x0E},
    {LCD_DATA, 1, 0x0F},
    {LCD_DATA, 1, 0x09},
    {LCD_DATA, 1, 0x07},
    {LCD_DATA, 1, 0x08},
    {LCD_DATA, 1, 0x03},

    {LCD_CMD, 1, 0xE8},
    {LCD_DATA, 1, 0x04},

    {LCD_CMD, 1, 0xE9},
    {LCD_DATA, 1, 0x0A},

    {LCD_CMD, 1, 0x62},
    {LCD_DATA, 1, 0x18},
    {LCD_DATA, 1, 0x0D},
    {LCD_DATA, 1, 0x71},
    {LCD_DATA, 1, 0xED},
    {LCD_DATA, 1, 0x70},
    {LCD_DATA, 1, 0x70},
    {LCD_DATA, 1, 0x18},
    {LCD_DATA, 1, 0x0F},
    {LCD_DATA, 1, 0x71},
    {LCD_DATA, 1, 0xEF},
    {LCD_DATA, 1, 0x70},
    {LCD_DATA, 1, 0x70},

    {LCD_CMD, 1, 0x63},
    {LCD_DATA, 1, 0x18},
    {LCD_DATA, 1, 0x11},
    {LCD_DATA, 1, 0x71},
    {LCD_DATA, 1, 0xF1},
    {LCD_DATA, 1, 0x70},
    {LCD_DATA, 1, 0x70},
    {LCD_DATA, 1, 0x18},
    {LCD_DATA, 1, 0x13},
    {LCD_DATA, 1, 0x71},
    {LCD_DATA, 1, 0xF3},
    {LCD_DATA, 1, 0x70},
    {LCD_DATA, 1, 0x70},

    {LCD_CMD, 1, 0x64},
    {LCD_DATA, 1, 0x28},
    {LCD_DATA, 1, 0x29},
    {LCD_DATA, 1, 0xF1},
    {LCD_DATA, 1, 0x01},
    {LCD_DATA, 1, 0xF1},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x07},

    {LCD_CMD, 1, 0x66},
    {LCD_DATA, 1, 0x3C},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0xCD},
    {LCD_DATA, 1, 0x67},
    {LCD_DATA, 1, 0x45},
    {LCD_DATA, 1, 0x45},
    {LCD_DATA, 1, 0x10},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x00},

    {LCD_CMD, 1, 0x67},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x3C},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x01},
    {LCD_DATA, 1, 0x54},
    {LCD_DATA, 1, 0x10},
    {LCD_DATA, 1, 0x32},
    {LCD_DATA, 1, 0x98},

    {LCD_CMD, 1, 0x74},
    {LCD_DATA, 1, 0x10},
    {LCD_DATA, 1, 0x85},
    {LCD_DATA, 1, 0x80},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x4E},
    {LCD_DATA, 1, 0x00},

    {LCD_CMD, 1, 0x98},
    {LCD_DATA, 1, 0x3e},
    {LCD_DATA, 1, 0x07},

    {LCD_CMD, 1, 0x35},
    {LCD_CMD, 1, 0x21},

    {LCD_CMD, 1, 0x11},
    {LCD_DELAY, 120},
    {LCD_CMD, 1, 0x29},
    {LCD_DELAY, 50}
    //////////////////////////////////////////////////
    // {LCD_CMD, 1, 0x36},
    // // if (horizontal == 0)
    // {LCD_DATA, 1, 0x00},
    // // else if (horizontal == 1)
    // //     {LCD_DATA, 1,0xC0},
    // // else if (horizontal == 2)
    // //     {LCD_DATA, 1,0x70},
    // // else
    // //     {LCD_DATA, 1,0xB0},

    // {LCD_CMD, 1, 0x3A},
    // {LCD_DATA, 1, 0x55},

    // {LCD_CMD, 1, 0xB2},
    // {LCD_DATA, 1, 0x0C},
    // {LCD_DATA, 1, 0x0C},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x33},
    // {LCD_DATA, 1, 0x33},

    // {LCD_CMD, 1, 0x35},
    // {LCD_DATA, 1, 0x00},

    // {LCD_CMD, 1, 0xB7},
    // {LCD_DATA, 1, 0x56},  // VGH=14.06 VGL=-11.38V

    // {LCD_CMD, 1, 0xBB},
    // {LCD_DATA, 1, 0x31},

    // {LCD_CMD, 1, 0xC0},
    // {LCD_DATA, 1, 0x2C},

    // {LCD_CMD, 1, 0xC2},
    // {LCD_DATA, 1, 0x01},

    // {LCD_CMD, 1, 0xC3},
    // {LCD_DATA, 1, 0x1D},

    // {LCD_CMD, 1, 0xC4},
    // {LCD_DATA, 1, 0x20},  // VDV, 0x20:0v

    // {LCD_CMD, 1, 0xC6},
    // {LCD_DATA, 1, 0x0F},

    // {LCD_CMD, 1, 0xD0},
    // {LCD_DATA, 1, 0xA4},
    // {LCD_DATA, 1, 0xA1},

    // {LCD_CMD, 1, 0xD6},
    // {LCD_DATA, 1, 0xA1},

    // {LCD_CMD, 1, 0xE0},
    // {LCD_DATA, 1, 0xF0},
    // {LCD_DATA, 1, 0x05},
    // {LCD_DATA, 1, 0x0B},
    // {LCD_DATA, 1, 0x09},
    // {LCD_DATA, 1, 0x09},
    // {LCD_DATA, 1, 0x06},
    // {LCD_DATA, 1, 0x31},
    // {LCD_DATA, 1, 0x44},
    // {LCD_DATA, 1, 0x47},
    // {LCD_DATA, 1, 0x36},
    // {LCD_DATA, 1, 0x13},
    // {LCD_DATA, 1, 0x13},
    // {LCD_DATA, 1, 0x28},
    // {LCD_DATA, 1, 0x2D},

    // {LCD_CMD, 1, 0xE1},
    // {LCD_DATA, 1, 0xF0},
    // {LCD_DATA, 1, 0x07},
    // {LCD_DATA, 1, 0x0C},
    // {LCD_DATA, 1, 0x0B},
    // {LCD_DATA, 1, 0x0B},
    // {LCD_DATA, 1, 0x17},
    // {LCD_DATA, 1, 0x31},
    // {LCD_DATA, 1, 0x33},
    // {LCD_DATA, 1, 0x46},
    // {LCD_DATA, 1, 0x38},
    // {LCD_DATA, 1, 0x14},
    // {LCD_DATA, 1, 0x15},
    // {LCD_DATA, 1, 0x2A},
    // {LCD_DATA, 1, 0x2E},

    // {LCD_CMD, 1, 0x21},

    // {LCD_CMD, 1, 0x11},
    // {LCD_DELAY, 120},

    // {LCD_CMD, 1, 0x29},
    // {LCD_DELAY, 120},

    // {LCD_CMD, 1, 0x2A},  // Column Address Set
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x00},  // 0
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0xEF},  // 239

    // {LCD_CMD, 1, 0x2B},  // Row Address Set
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x00},  // 0
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0xEF},  // 239
    // {LCD_CMD, 1, 0x2C},
    ///////////////////////////////////
    // {LCD_CMD, 1, 0xB2},
    // {LCD_DATA, 1, 0x1F},
    // {LCD_DATA, 1, 0x1F},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x33},
    // {LCD_DATA, 1, 0x33},

    // {LCD_CMD, 1, 0xB7},
    // {LCD_DATA, 1, 0x10},
    // {LCD_CMD, 1, 0xBB},
    // {LCD_DATA, 1, 0x38},
    // {LCD_CMD, 1, 0xC0},
    // {LCD_DATA, 1, 0x2C},
    // {LCD_CMD, 1, 0xC2},
    // {LCD_DATA, 1, 0x01},
    // {LCD_CMD, 1, 0xC3},
    // {LCD_DATA, 1, 0x0F},
    // {LCD_CMD, 1, 0xC4},
    // {LCD_DATA, 1, 0x20},
    // {LCD_CMD, 1, 0xC6},
    // {LCD_DATA, 1, 0x33},
    // {LCD_CMD, 1, 0xD0},
    // {LCD_DATA, 1, 0xA4},
    // {LCD_DATA, 1, 0xA1},

    // {LCD_CMD, 1, 0xD6},
    // {LCD_DATA, 1, 0xA1},

    // {LCD_CMD, 1, 0xE4},
    // {LCD_DATA, 1, 0x1D},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x00},

    // {LCD_CMD, 1, 0xE0},
    // {LCD_DATA, 1, 0xF0},
    // {LCD_DATA, 1, 0x01},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x09},
    // {LCD_DATA, 1, 0x0C},
    // {LCD_DATA, 1, 0x07},
    // {LCD_DATA, 1, 0x1C},
    // {LCD_DATA, 1, 0x44},
    // {LCD_DATA, 1, 0x32},
    // {LCD_DATA, 1, 0x08},
    // {LCD_DATA, 1, 0x14},
    // {LCD_DATA, 1, 0x15},
    // {LCD_DATA, 1, 0x18},
    // {LCD_DATA, 1, 0x1E},
    // {LCD_CMD, 1, 0xE1},
    // {LCD_DATA, 1, 0xF0},
    // {LCD_DATA, 1, 0x11},
    // {LCD_DATA, 1, 0x1A},
    // {LCD_DATA, 1, 0x0C},
    // {LCD_DATA, 1, 0x0C},
    // {LCD_DATA, 1, 0x08},
    // {LCD_DATA, 1, 0x38},
    // {LCD_DATA, 1, 0x44},
    // {LCD_DATA, 1, 0x4F},
    // {LCD_DATA, 1, 0x0A},
    // {LCD_DATA, 1, 0x18},
    // {LCD_DATA, 1, 0x17},
    // {LCD_DATA, 1, 0x35},
    // {LCD_DATA, 1, 0x35},

    // {LCD_CMD, 1, 0x36},
    // // if (horizontal == 0){LCD_DATA, 1, 0x00},
    // // else if (horizontal == 1){LCD_DATA, 1, 0xC0},
    // // else if (horizontal == 2){LCD_DATA, 1, 0x70},
    // // else {LCD_DATA, 1, 0xB0},

    // {LCD_CMD, 1, 0x3a},
    // {LCD_DATA, 1, 0x05},

    // {LCD_CMD, 1, 0x21},

    // {LCD_CMD, 1, 0x29},
    // {LCD_CMD, 1, 0x2a},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0xef},
    // {LCD_CMD, 1, 0x2b},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0x00},
    // {LCD_DATA, 1, 0xef},

    // {LCD_CMD, 1, 0x2c},

    // {LCD_CMD, 1, 0x11},
    // {LCD_DELAY, 500},
    // {LCD_CMD, 1, 0x29},
    // {LCD_DELAY, 500},
};

/**
 * @brief : lcd休眠参数表
 *
 */
LCD_SLEEP_PARAM(LCD_ST7789V3_ORIC_0144_240_240_SPI) = {
    {LCD_CMD, 2, 0x07, 0x00},  // Set D1=0, D0=1
    {LCD_DATA, 2, 0x31, 0x01},
    {LCD_DELAY, 2},
    {LCD_CMD, 2, 0x07, 0x00},  // Set D1=0, D0=0
    {LCD_DATA, 2, 0x07, 0x00},
    {LCD_DELAY, 2},
    {LCD_CMD, 2, 0x07, 0x00},  // display OFF
    {LCD_DATA, 2, 0x00, 0x00},
    //************* Power OFF sequence **************//
    {LCD_CMD, 2, 0x10, 0x00},  // SAP, BT[3:0], APE, AP, DSTB, SLP
    {LCD_DATA, 2, 0x80, 0x00},
    {LCD_CMD, 2, 0x11, 0x00},  // DC1[2:0], DC0[2:0], VC[2:0]
    {LCD_DATA, 2, 0x00, 0x00},
    {LCD_CMD, 2, 0x12, 0x00},  // VREG1OUT voltage
    {LCD_DATA, 2, 0x00, 0x00},
    {LCD_CMD, 2, 0x13, 0x00},  // VDV[4:0] for VCOM amplitude
    {LCD_DATA, 2, 0x00, 0x00},
    {LCD_DELAY, 50},           // Dis-charge capacitor power voltage
    {LCD_CMD, 2, 0x10, 0x00},  // SAP, BT[3:0], APE, AP, DSTB, SLP
    {LCD_DATA, 2, 0x82, 0x00},
};

/**
 * @brief : lcd唤醒参数表
 *
 */
LCD_WAKEUP_PARAM(LCD_ST7789V3_ORIC_0144_240_240_SPI) = {{-1, -1, -1}};

/**
 * @brief : lcd清屏参数表
 *
 */
LCD_CLEAN_PARAM(LCD_ST7789V3_ORIC_0144_240_240_SPI) = {{-1, -1, -1}};

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
 * @brief 屏关闭显示功能函数
 *
 * @return vd_bool_t 返回处理结果
 */
vd_bool_t lcd_ssd1306_limito_64_48_spi_closeDisp(vd_void_t) {
    vd_uint32_t i;
    return vd_true;
}
/**
 * @brief 打开显示功能函数
 *
 * @return vd_bool_t 返回处理结果
 */
vd_bool_t lcd_ssd1306_limito_64_48_spi_openDisp(vd_void_t) {
    return vd_true;
}

/**
 * @brief 清除显示内容函数
 *
 * @return vd_bool_t 返回处理结果
 */
vd_bool_t lcd_st7789v3_oric_154_240_240_spi_clean(vd_void_t) {
    vd_uint16_t x, y;

    // vd_uint32_t i;
    // drv_lcd_write_cmd8(0x01);  // Software reset
    // for (i = 0; i < 1000; i++) __nop();
    // drv_lcd_write_cmd8(0x29);
    // for (i = 0; i < 120; i++) __nop();

    return vd_true;
}

/**
 * @brief : 设置lcd当前读写区域
 *
 * @param xs: 起始坐标x
 * @param ys: 起始坐标y
 * @param xe: 结束坐标x
 * @param ye: 结束坐标y
 * @return vd_bool_t
 */
vd_bool_t lcd_st7789v3_oric_154_240_240_spi_set_pos(vd_uint16_t xs, vd_uint16_t ys, vd_uint16_t xe, vd_uint16_t ye) {
    drv_lcd_write_cmd8(0x2a);  //列地址设置
    mcu_delay(50);
    drv_lcd_write_data16(xs);
    drv_lcd_write_data16(xe);
    drv_lcd_write_cmd8(0x2b);  //行地址设置
    mcu_delay(50);
    drv_lcd_write_data16(ys);
    drv_lcd_write_data16(ye);
    drv_lcd_write_cmd8(0x2c);  //储存器写
    mcu_delay(50);
    return vd_true;
}

/**
 * @brief:lcd驱动实现,参数分别为: name, init(),sleep(),wakeup(),clean(),set_pos()
 * 如果为参数空，则使用默认的函数
 *
 */
LCD_IMPLETEMTE(
    LCD_ST7789V3_ORIC_0144_240_240_SPI,
    vd_null,
    vd_null,
    vd_null,
    lcd_st7789v3_oric_154_240_240_spi_clean,
    lcd_st7789v3_oric_154_240_240_spi_set_pos)

#endif