/*
 * Project: mcu-kit
 * Module: lcd chip driver
 * File: LCD_ST7789V22_BOHU_0244_240_320_SPI.c
 * Created Date: 2022-04-20 19:27:54
 * Author: xiezhuang
 * Description: ������TP2.4����
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

#if (LCDHUNAM_WEITAI_099_128_115_SPI == ON)

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
LCD_DEFINE(LCDHUNAM_WEITAI_099_128_115_SPI);

/**
 * @brief : lcd��ʼ��������
 *
 */
LCD_INIT_PARAM(LCDHUNAM_WEITAI_099_128_115_SPI) = {
    //{LCD_DELAY, 120},
    {LCD_CMD, 1, 0xFE},
    {LCD_CMD, 1, 0xEF},

    {LCD_CMD, 1, 0xB0},
    {LCD_DATA, 1, 0xC0},

    {LCD_CMD, 1, 0xB2},
    {LCD_DATA, 1, 0x24},

    {LCD_CMD, 1, 0xB3},
    {LCD_DATA, 1, 0x03},

    {LCD_CMD, 1, 0xB7},
    {LCD_DATA, 1, 0x01},

    {LCD_CMD, 1, 0xB6},
    {LCD_DATA, 1, 0x19},

    {LCD_CMD, 1, 0xAC},
    {LCD_DATA, 1, 0xDB},
    {LCD_CMD, 1, 0xAB},
    {LCD_DATA, 1, 0x0F},
    {LCD_CMD, 1, 0x3A},
    {LCD_DATA, 1, 0x05},

    {LCD_CMD, 1, 0xB4},
    {LCD_DATA, 1, 0x04},

    {LCD_CMD, 1, 0xA8},
    {LCD_DATA, 1, 0x0C},

    {LCD_CMD, 1, 0xB8},
    {LCD_DATA, 1, 0x08},

    {LCD_CMD, 1, 0xED},
    {LCD_DATA, 1, 0x03},

    {LCD_CMD, 1, 0xEA},
    {LCD_DATA, 1, 0x9F},

    {LCD_CMD, 1, 0xC6},
    {LCD_DATA, 1, 0x2A},

    {LCD_CMD, 1, 0xC7},
    {LCD_DATA, 1, 0x10},

    {LCD_CMD, 1, 0xF0},
    {LCD_DATA, 1, 0x09},
    {LCD_DATA, 1, 0x35},
    {LCD_DATA, 1, 0x2A},
    {LCD_DATA, 1, 0x4A},
    {LCD_DATA, 1, 0xA9},
    {LCD_DATA, 1, 0x39},
    {LCD_DATA, 1, 0x35},
    {LCD_DATA, 1, 0x60},
    {LCD_DATA, 1, 0x00},
    {LCD_DATA, 1, 0x14},
    {LCD_DATA, 1, 0x0A},
    {LCD_DATA, 1, 0x16},
    {LCD_DATA, 1, 0x10},
    {LCD_DATA, 1, 0x1F},

    {LCD_DATA, 1, 0xF1},
    {LCD_DATA, 1, 0x13},
    {LCD_DATA, 1, 0x24},
    {LCD_DATA, 1, 0x55},
    {LCD_DATA, 1, 0x3C},
    {LCD_DATA, 1, 0xC6},
    {LCD_DATA, 1, 0x16},
    {LCD_DATA, 1, 0x3F},
    {LCD_DATA, 1, 0x60},
    {LCD_DATA, 1, 0x08},
    {LCD_DATA, 1, 0x06},
    {LCD_DATA, 1, 0x0D},
    {LCD_DATA, 1, 0x1F},
    {LCD_DATA, 1, 0x1C},
    {LCD_DATA, 1, 0x10},

    {LCD_CMD, 1, 0x11},
    {LCD_DELAY, 120},
    {LCD_CMD, 1, 0x29},
    {LCD_DELAY, 20},
};

/**
 * @brief : lcd���߲�����
 *
 */
LCD_SLEEP_PARAM(LCDHUNAM_WEITAI_099_128_115_SPI) = {
    //    {LCD_CMD, 1, 0x28},
    //    {LCD_DELAY, 120},
    //    {LCD_CMD, 1, 0x10},
    {LCD_DELAY, 150},
};

/**
 * @brief : lcd���Ѳ�����
 *
 */
// LCD_WAKEUP_PARAM(
//     LCDHUNAM_WEITAI_099_128_115_SPI) = {{LCD_CMD, 1, 0x11}, {LCD_DELAY, 120}, {LCD_CMD, 1, 0x29}, {LCD_DELAY, 100}};
LCD_WAKEUP_PARAM(
    LCDHUNAM_WEITAI_099_128_115_SPI) = {{LCD_DELAY, 100}};
/**
 * @brief : lcd����������
 *
 */
LCD_CLEAN_PARAM(LCDHUNAM_WEITAI_099_128_115_SPI) = {{-1, -1, -1}};

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
 * @brief ���ر���ʾ���ܺ���
 *
 * @return vd_bool_t ���ش�����
 */
vd_bool_t lcd_ssd1306_limito_64_48_spi_closeDisp(vd_void_t)
{
  vd_uint32_t i;
  return vd_true;
}
/**
 * @brief ����ʾ���ܺ���
 *
 * @return vd_bool_t ���ش�����
 */
vd_bool_t lcd_ssd1306_limito_64_48_spi_openDisp(vd_void_t)
{
  return vd_true;
}

/**
 * @brief �����ʾ���ݺ���
 *
 * @return vd_bool_t ���ش�����
 */
vd_bool_t lcd_huannan_weitai_099_128_115_spi_clean(vd_void_t)
{
  vd_uint16_t x, y;

  // vd_uint32_t i;
  // drv_lcd_write_cmd8(0x01);  // Software reset
  // for (i = 0; i < 1000; i++) __nop();
  // drv_lcd_write_cmd8(0x29);
  // for (i = 0; i < 120; i++) __nop();

  return vd_true;
}

/**
 * @brief : ����lcd��ǰ��д����
 *
 * @param xs: ��ʼ����x
 * @param ys: ��ʼ����y
 * @param xe: ��������x
 * @param ye: ��������y
 * @return vd_bool_t
 */
vd_bool_t lcd_huannan_weitai_099_128_115_spi_set_pos(vd_uint16_t xs, vd_uint16_t ys, vd_uint16_t xe, vd_uint16_t ye)
{

  //	 ys+=48;
  //	  ye+=96;
  ys += 45;
  ye += 45;

  drv_lcd_write_cmd8(0x2a); //�е�ַ����
  // mcu_delay(50);
  drv_lcd_write_data8((xs >> 8));
  drv_lcd_write_data8(xs);
  drv_lcd_write_data8((xe >> 8));
  drv_lcd_write_data8(xe);

  drv_lcd_write_cmd8(0x2b); //�е�ַ����
  // mcu_delay(50);
  drv_lcd_write_data8((ys >> 8));
  drv_lcd_write_data8(ys);
  drv_lcd_write_data8((ye >> 8));
  drv_lcd_write_data8(ye);
  drv_lcd_write_cmd8(0x2c); //������д
  // mcu_delay(50);
  return vd_true;
}

/**
 * @brief:lcd����ʵ��,�����ֱ�Ϊ: name, init(),sleep(),wakeup(),clean(),set_pos()
 * ���Ϊ�����գ���ʹ��Ĭ�ϵĺ���
 *
 */
LCD_IMPLETEMTE(
    LCDHUNAM_WEITAI_099_128_115_SPI,
    vd_null,
    vd_null,
    vd_null,
    lcd_huannan_weitai_099_128_115_spi_clean,
    lcd_huannan_weitai_099_128_115_spi_set_pos)

#endif