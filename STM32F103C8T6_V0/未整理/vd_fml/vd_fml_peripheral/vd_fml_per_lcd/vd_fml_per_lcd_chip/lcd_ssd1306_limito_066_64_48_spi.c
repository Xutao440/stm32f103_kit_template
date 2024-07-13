/*
 * Project: mcu-kit
 * Module: lcd chip driver
 * File: lcd_ssd1317_allvision_0138_96_96_spi.c
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
//#include "drv_lcd_pic.h"

#if (LCD_SSD1317_ALLVISION_0138_96_96_SPI == ON)

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
LCD_DEFINE(LCD_SSD1317_ALLVISION_0138_96_96_SPI);

/**
 * @brief : lcd初始化参数表
 *
 */
LCD_INIT_PARAM(LCD_SSD1317_ALLVISION_0138_96_96_SPI) = {
    {LCD_CMD, 1, 0xAE},  /*display off*/
    {LCD_CMD, 1, 0xD5},  /*set osc division*/
    {LCD_CMD, 1, 0xE1},  //
    {LCD_CMD, 1, 0xA8},  /*multiplex ratio*/
    {LCD_CMD, 1, 0x5F},  /*duty = 1/64*/
    {LCD_CMD, 1, 0xD3},  /*set display offset*/
    {LCD_CMD, 1, 0x00},  //
    {LCD_CMD, 1, 0x40},  /*set display start line*/
    {LCD_CMD, 1, 0xA1},  // set segment remap, 0xA0:正常，0xA1:左右镜像
    {LCD_CMD, 1, 0xC0},  // Com scan direction, 0xC0:正常，0xC8:上下镜像
    {LCD_CMD, 1, 0xDA},  /*set COM pins*/
    {LCD_CMD, 1, 0x12},  // LCM  control
    {LCD_CMD, 1, 0x81},  /*contract control*/
    {LCD_CMD, 1, 0x67},  /*128*/
    {LCD_CMD, 1, 0xD9},  /*set pre-charge period*/
    {LCD_CMD, 1, 0x22},  //
    {LCD_CMD, 1, 0xDB},  /*set vcomh*/
    {LCD_CMD, 1, 0x20},  //
    {LCD_CMD, 1, 0xA4},  //
    {LCD_CMD, 1, 0xA6},  /*normal / reverse*/
    {LCD_CMD, 1, 0xB0},  /*set page address*/
    {LCD_CMD, 1, 0xAF},  /*display ON*/
    {LCD_DELAY, 100},    //延迟100ms
    //
    // {LCD_CMD, 1, 0xAE},                      //--turn off oled panel
    // {LCD_CMD, 1, 0x00},                      //---set low column address
    // {LCD_CMD, 1, 0x10},                      //---set high column address
    // {LCD_CMD, 1, 0x40},                      //--set start line address  Set Mapping RAM Display Start Line
    // (0x00~0x3F) {LCD_CMD, 1, 0x81},                      //--set contrast control register {LCD_CMD, 1, 0xCF}, // Set
    // SEG Output Current Brightness {LCD_CMD, 1, 0xA1},                      //--Set SEG/Column Mapping 0xa0左右反置
    // 0xa1正常 {LCD_CMD, 1, 0xC8},                      // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常 {LCD_CMD,
    // 1, 0xA6},                      //--set normal display {LCD_CMD, 1, 0xA8},                      //--set multiplex
    // ratio(1 to 64) {LCD_CMD, 1, 0x3f},                      //--1/64 duty {LCD_CMD, 1, 0xD3}, //-set display offset
    // Shift Mapping RAM Counter (0x00~0x3F) {LCD_CMD, 1, 0x00},                      //-not offset {LCD_CMD, 1, 0xd5},
    // //--set display clock divide ratio/oscillator frequency {LCD_CMD, 1, 0x80},                      //--set divide
    // ratio, Set Clock as 100 Frames/Sec {LCD_CMD, 1, 0xD9},                      //--set pre-charge period {LCD_CMD,
    // 1, 0xF1},                      // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock {LCD_CMD, 1, 0xDA}, //--set
    // com pins hardware configuration {LCD_CMD, 1, 0x12}, {LCD_CMD, 1, 0xDB},  //--set vcomh {LCD_CMD, 1, 0x40}, // Set
    // VCOM Deselect Level {LCD_CMD, 1, 0x20},                      //-Set Page Addressing Mode (0x00/0x01/0x02)
    // {LCD_CMD, 1, 0x02},                      //
    // {LCD_CMD, 1, 0x8D},                      //--set Charge Pump enable/disable
    // {LCD_CMD, 1, 0x14},                      //--set(0x10) disable
    // {LCD_CMD, 1, 0xA4},                      // Disable Entire Display On (0xa4/0xa5)
    // {LCD_CMD, 1, 0xA6},                      // Disable Inverse Display On (0xa6/a7)
    // {LCD_CMD, 1, 0xAF},                      //--turn on oled panel

    // {LCD_CMD, 1, 0xAF}, /*display ON*/
};

/**
 * @brief : lcd休眠参数表
 *
 */
LCD_SLEEP_PARAM(LCD_SSD1317_ALLVISION_0138_96_96_SPI) = {
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
LCD_WAKEUP_PARAM(LCD_SSD1317_ALLVISION_0138_96_96_SPI) = {{-1, -1, -1}};

/**
 * @brief : lcd清屏参数表
 *
 */
LCD_CLEAN_PARAM(LCD_SSD1317_ALLVISION_0138_96_96_SPI) = {{-1, -1, -1}};

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
vd_bool_t lcd_ssd1317_allvision_0138_96_96_spi_closeDisp(vd_void_t) {
    vd_uint32_t i;
    // drv_lcd_write_cmd_16bit(0x0007);
    // drv_lcd_write_data_16bit(0x0131);  // Set D1=0, D0=1
    // for (i = 0; i < (10 * 255); i++) __nop();
    // drv_lcd_write_cmd_16bit(0x0007);
    // drv_lcd_write_data_16bit(0x0130);  // Set D1=0, D0=0
    // for (i = 0; i < (10 * 255); i++) __nop();
    // drv_lcd_write_cmd_16bit(0x0007);
    // drv_lcd_write_data_16bit(0x0000);  // display OFF
    /************* Power OFF sequence **************/
    //	drv_lcd_write_cmd_16bit(0x0010);drv_lcd_write_data_16bit(0x0080); // SAP, BT[3:0], APE, AP, DSTB, SLP
    //	drv_lcd_write_cmd_16bit(0x0011);drv_lcd_write_data_16bit(0x0000); // DC1[2:0], DC0[2:0], VC[2:0]
    //	drv_lcd_write_cmd_16bit(0x0012);drv_lcd_write_data_16bit(0x0000); // VREG1OUT voltage
    //	drv_lcd_write_cmd_16bit(0x0013);drv_lcd_write_data_16bit(0x0000); // VDV[4:0] for VCOM amplitude
    //	mcu_delay(200);                          // Dis-charge capacitor power voltage
    //	drv_lcd_write_cmd_16bit(0x0010);drv_lcd_write_data_16bit(0x0082); // SAP, BT[3:0], APE, AP, DSTB, SLP
    return vd_true;
}
/**
 * @brief 打开显示功能函数
 *
 * @return vd_bool_t 返回处理结果
 */
vd_bool_t lcd_ssd1317_allvision_0138_96_96_spi_openDisp(vd_void_t) {
    // Power On sequence
    //	drv_lcd_write_cmd_16bit(0x0010);drv_lcd_write_data_16bit(0x0080); // SAP, BT[3:0], AP, DSTB, SLP
    //	drv_lcd_write_cmd_16bit(0x0011);drv_lcd_write_data_16bit(0x0000); // DC1[2:0], DC0[2:0], VC[2:0]
    //	drv_lcd_write_cmd_16bit(0x0012);drv_lcd_write_data_16bit(0x0000); // VREG1OUT voltage
    //	drv_lcd_write_cmd_16bit(0x0013);drv_lcd_write_data_16bit(0x0000); // VDV[4:0] for VCOM amplitude
    //	mcu_delay(200);                          // Dis-charge capacitor power voltage
    //	drv_lcd_write_cmd_16bit(0x0010);drv_lcd_write_data_16bit(0x1490); // SAP, BT[3:0], AP, DSTB, SLP, STB
    //	drv_lcd_write_cmd_16bit(0x0011);drv_lcd_write_data_16bit(0x0227); // R11h=0x0221 at VCI=3.3V DC1[2:0], DC0[2:0],
    // VC[2:0] 	mcu_delay(50);                           // Delay 50ms
    //	drv_lcd_write_cmd_16bit(0x0012);drv_lcd_write_data_16bit(0x009B); // External reference voltage =Vci;
    //	delay(50);                           // Delay 50ms
    //	drv_lcd_write_cmd_16bit(0x0013);drv_lcd_write_data_16bit(0x1800); // R13h=0x1200 when R12=009D VDV[4:0] for VCOM
    // amplitude 	drv_lcd_write_cmd_16bit(0x0029);drv_lcd_write_data_16bit(0x0027); // R29h=0x000C when R12=009D
    // VCM[5:0] for VCOMH 	mcu_delay(50);                           // Delay 50ms
    // drv_lcd_write_cmd_16bit(0x0007);
    // drv_lcd_write_data_16bit(0x0133);  // 262K color and display ON
    return vd_true;
}

/**
 * @brief 清除显示内容函数
 *
 * @return vd_bool_t 返回处理结果
 */
vd_bool_t lcd_ssd1317_allvision_0138_96_96_spi_clean(vd_void_t) {
    vd_uint16_t x, y;

    vd_uint16_t height = g_lcd_plat_drv.lcd_height;
    vd_uint16_t width  = g_lcd_plat_drv.lcd_width;
		
    for (y = 0; y < 12; y++) {
        drv_lcd_write_cmd8(0xB0 + y);
        drv_lcd_write_cmd8(0x00);
        drv_lcd_write_cmd8(0x11);

        for (x = 16; x < width+16; x++) {
						drv_lcd_write_data8(0x00);
        }
    }

    return vd_true;
}

/**
 * @brief 设置lcd当前读写
 *
 * @return vd_bool_t 返回处理结果
 */
vd_bool_t lcd_ssd1317_allvision_0138_96_96_spi_set_pos(vd_uint16_t xs, vd_uint16_t ys, vd_uint16_t xe, vd_uint16_t ye) {
    vd_uint16_t x_start = xs + 16;
//    log_debug("xs=%d,ys=%d", xs, ys);
    drv_lcd_write_cmd8(0xB0 + ys);
    drv_lcd_write_cmd8(((x_start & 0x70) >> 4) | 0x10);
    drv_lcd_write_cmd8(x_start & 0x0f);

    return vd_true;
}

/**
 * @brief:实现lcd驱动,参数分别为: name, init(),sleep(),wakeup(),clean(),set_pos()
 * 如果为参数空，则使用默认的函数
 *
 */
LCD_IMPLETEMTE(
    LCD_SSD1317_ALLVISION_0138_96_96_SPI,
    vd_null,
    vd_null,
    vd_null,
    lcd_ssd1317_allvision_0138_96_96_spi_clean,
    lcd_ssd1317_allvision_0138_96_96_spi_set_pos)

#endif