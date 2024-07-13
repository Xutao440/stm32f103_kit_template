### **mcu-kit LCD驱动框架**

| 版本 | 修改日期 | 修改人 | 备注 |
| ---- | -------- | ------ | ---- |
| V1.0 | 20220425 | 张美珍 |      |

[TOC]



#### 一，LCD驱动调试步骤

##### 1，LCD驱动宏定义及参数修改

在mcu-kit/conf/platfrom_config.h头文件中新增一个lcd驱动宏名称，如LCD_SSD1306_LIMITO_64_48_SPI，并按以下图修改LCD参数：供电脚，复位脚，背光脚，ID脚，分辨率，选择角度等参数。

**注意：驱动宏命名规则：lcd+芯片名+屏厂商名+尺寸+分辨率+通信接口 ，宏大写，文件名小写**

```c
// 2.7 LCD配置
#define PLATFORM_LCD_EN             ON
#define PLATFORM_LCD_POWER_PIN      3   // 屏供电脚
#define PLATFORM_LCD_RESET_PIN      4   // 屏复位脚
#define PLATFORM_LCD_BACKLIGHT_PIN  3   // 屏背光脚
#define PLATFORM_LCD_BACKLIGHT_TYPE 0   // 0:GPIO控制背光，1:PWM控制背光
#define PLATFORM_LCD_ID1_PIN0       -1  // 屏ID脚1
#define PLATFORM_LCD_ID2_PIN0       -1  // 屏ID脚2
#define PLATFORM_LCD_ROTATE         90  // 屏角度,[0,90,180,270]
#define PLATFORM_LCD_WIDTH          96  // 屏宽
#define PLATFORM_LCD_HEIGHT         12  // 屏高

/****LCD屏类型*****/
#define LCD_SSD1306_LIMITO_64_48_SPI ON
#define LCD_FEIXINDA_128_GC9A01      OFF
#define LCD_XINLI_128_SSD1327        OFF
#define LCD_OURUIKE_130_7789V2       OFF
#define LCD_XINYONGHUI_130_7789V2    OFF
#define LCD_OURUIKE_144_7735S        OFF
#define LCD_FEIXINDA_154_7789        OFF
#define LCD_LONGXI_154_7789          OFF
#define LCD_OURUIKE_177_7735S        OFF
#define LCD_FEIXINDA_24_7789V2       OFF
#define LCD_RGB_43_GT911             OFF
```



##### 2，LCD驱动注册

在mcu-lit/lcd/drv_lcd.c中注册的LCD驱动

```c

/**
 * @brief: 声明LCD驱动
 *
 */
#if (LCD_SSD1306_LIMITO_64_48_SPI == ON)
LCD_DECLAE(LCD_SSD1306_LIMITO_64_48_SPI)
#endif
/* ======================================================================================
 * globals
 */

/**
 * @brief :添加LCD驱动
 *
 */
static lcd_chip_diver_t g_lcd_chip_drivers[] = {
#if (LCD_SSD1306_LIMITO_64_48_SPI == ON)
    LCD_ADD(LCD_SSD1306_LIMITO_64_48_SPI),
#endif
};
```



##### 3，LCD驱动实现

在mcu-kit/lcd/lcd_chip目录中新建lcd驱动文件，如lcd_ssd1306_limito_64_48_spi.c(按照命名规则)

1,，LCD_DEFINE(LCD_SSD1306_LIMITO_64_48_SPI) //定义驱动

2，LCD_INIT_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) //定义初始化表

3，LCD_SLEEP_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) //定义睡眠参数表

4，LCD_CLEAN_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) //定义清屏参数表

5，LCD_IMPLETEMTE(                    // 驱动实现
    LCD_SSD1306_LIMITO_64_48_SPI,     // 驱动名称，一定要跟上面的宏同名
    vd_null,                          // 屏初始化函数，如果为空，则使用默认函数下发初始化参数表
    vd_null,                          // 屏进入休眠函数，如果为空，则使用默认函数下发休眠参数表进入休眠
    vd_null, 			      // 屏唤醒函数，如果为空，则使用默认函数下发唤醒参数表唤醒屏幕
    lcd_ssd1306_limito_64_48_spi_clean,   //屏幕清屏函数
    lcd_ssd1306_limito_64_48_spi_set_pos) // 设置屏幕读写坐标函数



```c
LCD_DEFINE(LCD_SSD1306_LIMITO_64_48_SPI);
/**
 * @brief : lcd初始化参数表
 *
 */
LCD_INIT_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) = {
    {LCD_CMD, 1, 0xAE},  //LCD_CMD：表示命令， LCD_DATA:表示数据，LCD_DELAY表示延迟
    {LCD_CMD, 1, 0xD5},  /*set osc division*/
    {LCD_CMD, 1, 0xE1},  //
    {LCD_CMD, 1, 0xA8},  /*multiplex ratio*/
    {LCD_CMD, 1, 0x5F},  /*duty = 1/64*/
    {LCD_CMD, 1, 0xD3},  /*set display offset*/
    {LCD_CMD, 1, 0x00},  //
    {LCD_CMD, 1, 0x40},  /*set display start line*/
    {LCD_CMD, 1, 0xA0},  // set segment remap, 0xA0:正常，0xA1:左右镜像
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
    {LCD_DATA, 1, 0xAF},
    {LCD_DELAY, 100},  //延迟100ms
};

/**
 * @brief : lcd休眠参数表
 *
 */
LCD_SLEEP_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) = {{-1, -1, -1}};

/**
 * @brief : lcd唤醒参数表
 *
 */
LCD_WAKEUP_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) = {{-1, -1, -1}};

/**
 * @brief : lcd清屏参数表
 *
 */
LCD_CLEAN_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) = {{-1, -1, -1}};

/**
 * @brief 清除显示内容函数
 *
 * @return vd_bool_t 返回处理结果
 */
vd_bool_t lcd_ssd1306_limito_64_48_spi_clean(vd_void_t) {
    
    return vd_true;
}

/**
 * @brief 设置lcd当前读写问题
 *
 * @return vd_bool_t 返回处理结果
 */
vd_bool_t lcd_ssd1306_limito_64_48_spi_set_pos(vd_uint16_t x, vd_uint16_t y) {

    return vd_true;
}

/**
 * @brief:实现lcd驱动,参数分别为: name, init(),sleep(),wakeup(),clean(),set_pos()
 * 如果为参数空，则使用默认的函数
 *
 */
LCD_IMPLETEMTE(
    LCD_SSD1306_LIMITO_64_48_SPI,
    vd_null,
    vd_null,
    lcd_ssd1306_limito_64_48_spi_clean,
    lcd_ssd1306_limito_64_48_spi_clean,
    lcd_ssd1306_limito_64_48_spi_set_pos)

#endif
```

#### 二，LCD驱动框架介绍