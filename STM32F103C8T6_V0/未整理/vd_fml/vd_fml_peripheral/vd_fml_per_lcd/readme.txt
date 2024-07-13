### **mcu-kit LCD�������**

| �汾 | �޸����� | �޸��� | ��ע |
| ---- | -------- | ------ | ---- |
| V1.0 | 20220425 | ������ |      |

[TOC]



#### һ��LCD�������Բ���

##### 1��LCD�����궨�弰�����޸�

��mcu-kit/conf/platfrom_config.hͷ�ļ�������һ��lcd���������ƣ���LCD_SSD1306_LIMITO_64_48_SPI����������ͼ�޸�LCD����������ţ���λ�ţ�����ţ�ID�ţ��ֱ��ʣ�ѡ��ǶȵȲ�����

**ע�⣺��������������lcd+оƬ��+��������+�ߴ�+�ֱ���+ͨ�Žӿ� �����д���ļ���Сд**

```c
// 2.7 LCD����
#define PLATFORM_LCD_EN             ON
#define PLATFORM_LCD_POWER_PIN      3   // �������
#define PLATFORM_LCD_RESET_PIN      4   // ����λ��
#define PLATFORM_LCD_BACKLIGHT_PIN  3   // �������
#define PLATFORM_LCD_BACKLIGHT_TYPE 0   // 0:GPIO���Ʊ��⣬1:PWM���Ʊ���
#define PLATFORM_LCD_ID1_PIN0       -1  // ��ID��1
#define PLATFORM_LCD_ID2_PIN0       -1  // ��ID��2
#define PLATFORM_LCD_ROTATE         90  // ���Ƕ�,[0,90,180,270]
#define PLATFORM_LCD_WIDTH          96  // ����
#define PLATFORM_LCD_HEIGHT         12  // ����

/****LCD������*****/
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



##### 2��LCD����ע��

��mcu-lit/lcd/drv_lcd.c��ע���LCD����

```c

/**
 * @brief: ����LCD����
 *
 */
#if (LCD_SSD1306_LIMITO_64_48_SPI == ON)
LCD_DECLAE(LCD_SSD1306_LIMITO_64_48_SPI)
#endif
/* ======================================================================================
 * globals
 */

/**
 * @brief :���LCD����
 *
 */
static lcd_chip_diver_t g_lcd_chip_drivers[] = {
#if (LCD_SSD1306_LIMITO_64_48_SPI == ON)
    LCD_ADD(LCD_SSD1306_LIMITO_64_48_SPI),
#endif
};
```



##### 3��LCD����ʵ��

��mcu-kit/lcd/lcd_chipĿ¼���½�lcd�����ļ�����lcd_ssd1306_limito_64_48_spi.c(������������)

1,��LCD_DEFINE(LCD_SSD1306_LIMITO_64_48_SPI) //��������

2��LCD_INIT_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) //�����ʼ����

3��LCD_SLEEP_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) //����˯�߲�����

4��LCD_CLEAN_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) //��������������

5��LCD_IMPLETEMTE(                    // ����ʵ��
    LCD_SSD1306_LIMITO_64_48_SPI,     // �������ƣ�һ��Ҫ������ĺ�ͬ��
    vd_null,                          // ����ʼ�����������Ϊ�գ���ʹ��Ĭ�Ϻ����·���ʼ��������
    vd_null,                          // ���������ߺ��������Ϊ�գ���ʹ��Ĭ�Ϻ����·����߲������������
    vd_null, 			      // �����Ѻ��������Ϊ�գ���ʹ��Ĭ�Ϻ����·����Ѳ���������Ļ
    lcd_ssd1306_limito_64_48_spi_clean,   //��Ļ��������
    lcd_ssd1306_limito_64_48_spi_set_pos) // ������Ļ��д���꺯��



```c
LCD_DEFINE(LCD_SSD1306_LIMITO_64_48_SPI);
/**
 * @brief : lcd��ʼ��������
 *
 */
LCD_INIT_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) = {
    {LCD_CMD, 1, 0xAE},  //LCD_CMD����ʾ��� LCD_DATA:��ʾ���ݣ�LCD_DELAY��ʾ�ӳ�
    {LCD_CMD, 1, 0xD5},  /*set osc division*/
    {LCD_CMD, 1, 0xE1},  //
    {LCD_CMD, 1, 0xA8},  /*multiplex ratio*/
    {LCD_CMD, 1, 0x5F},  /*duty = 1/64*/
    {LCD_CMD, 1, 0xD3},  /*set display offset*/
    {LCD_CMD, 1, 0x00},  //
    {LCD_CMD, 1, 0x40},  /*set display start line*/
    {LCD_CMD, 1, 0xA0},  // set segment remap, 0xA0:������0xA1:���Ҿ���
    {LCD_CMD, 1, 0xC0},  // Com scan direction, 0xC0:������0xC8:���¾���
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
    {LCD_DELAY, 100},  //�ӳ�100ms
};

/**
 * @brief : lcd���߲�����
 *
 */
LCD_SLEEP_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) = {{-1, -1, -1}};

/**
 * @brief : lcd���Ѳ�����
 *
 */
LCD_WAKEUP_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) = {{-1, -1, -1}};

/**
 * @brief : lcd����������
 *
 */
LCD_CLEAN_PARAM(LCD_SSD1306_LIMITO_64_48_SPI) = {{-1, -1, -1}};

/**
 * @brief �����ʾ���ݺ���
 *
 * @return vd_bool_t ���ش�����
 */
vd_bool_t lcd_ssd1306_limito_64_48_spi_clean(vd_void_t) {
    
    return vd_true;
}

/**
 * @brief ����lcd��ǰ��д����
 *
 * @return vd_bool_t ���ش�����
 */
vd_bool_t lcd_ssd1306_limito_64_48_spi_set_pos(vd_uint16_t x, vd_uint16_t y) {

    return vd_true;
}

/**
 * @brief:ʵ��lcd����,�����ֱ�Ϊ: name, init(),sleep(),wakeup(),clean(),set_pos()
 * ���Ϊ�����գ���ʹ��Ĭ�ϵĺ���
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

#### ����LCD������ܽ���