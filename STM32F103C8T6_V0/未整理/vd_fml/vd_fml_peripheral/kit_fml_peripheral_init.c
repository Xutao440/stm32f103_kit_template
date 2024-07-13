#include "kit_driver_fml_peripheral.h"
#if (DRIVER_FML_ADC_KEY_EN==ON)
#include "fsm_common.h"
#endif

//ģ���ʼ���ӿ�
vd_void_t kit_hrd_init( vd_void_t )
{
	#if (DRIVER_FML_ADC_KEY_EN==ON)
	mSysParam.mCheckChargeFlag=hrd_adc_key_module_power_on_check();
	#endif
	#if (DRIVER_FML_BATTERY_EN==ON)
	hrd_battery_module_init();
	#endif
	#if ((DRIVER_FML_BMS_SPI_EN==ON)&&(DRIVER_BSP_SPIM_EN==ON))
	hrd_bms_spi_module_init();
	#endif
	#if ((DRIVER_FML_ADC_KEY_EN==ON)&&(DRIVER_BSP_ADC_EN==ON))
	hrd_adc_key_module_init();
	#endif
	#if ((DRIVER_FML_MOTOR_TYPE_DC==ON)&&(DRIVER_BSP_GPIO_EN==ON)&&(DRIVER_BSP_PWM_EN==ON)&&(DRIVER_BSP_ADC_EN==ON))
	hrd_roller_module_init();
	#endif
	#if (DRIVER_FML_LED_DISPLAY_EN==ON)
	hrd_led_display_module_init();
	#endif
	
	#if (DRIVER_FML_UART_DISPLAY_EN == ON)//������ʾ����λ��
	hrd_uart_display_module_init();
	#endif
	#if (DRIVER_FML_GPIO_KEY_EN == ON)
	hrd_key_module_init();
	#endif
	#if (DRIVER_FML_LED_EN==ON)
	hrd_led_module_init();
	#endif
	#if (DRIVER_FML_TIME_EN == ON)//����δ��ͨ
	hrd_time_module_init();//��ע���ж�//�ٴ��ж�
	#endif
	
	#if (DRIVER_FML_FAN_EN==ON)
	hrd_fan_module_init();
	#endif
	#if (DRIVER_FML_HEAT_EN==ON)
	hrd_heat_module_init();
	#endif
	#if (DRIVER_FML_BEEP_EN==ON)
	hrd_beep_module_init();
	#endif
	#if (DRIVER_FML_POT_DETECTION_EN==ON)
	hrd_pot_detection_module_init();
	#endif
	#if (DRIVER_FML_ZERO_CROSSING_DETECTION_EN==ON)
	hrd_zero_crossing_detect_module_init();
	#endif
}

/*************************ģ��ʹ��IO��ʼ���ӿ�***********************************/
vd_void_t kit_hrd_drivers_io_init(vd_void_t){
	
#if (DRIVER_FML_BMS_SPI_EN == ON)
	hrd_bms_spi_gpio_init();
	#endif
	#if (DRIVER_FML_LED_DISPLAY_EN==ON)
	hrd_led_display_module_io_init();
	#endif
	#if (DRIVER_FML_MOTOR_TYPE_BLDC == ON)||(DRIVER_FML_MOTOR_TYPE_BSAC == ON)
	hrd_motor_module_init();
	#endif
	#if (DRIVER_FML_LED_EN == ON)
	hrd_led_module_gpio_init();
	#endif
}

/*************************ģ��������ӿ�***********************************/
vd_void_t hrd_drivers_check_module(vd_uint16_t check_20ms_once_num){
	
	switch(check_20ms_once_num){
		case 5:
			
		break;
		default:
			
		break;
	}
}



/****************************��������Ϊ����ʹ��*******************************/
//ģ�鹦�ܲ��Գ�ʼ���ӿ�
#if DRIVER_FML_TEST_EN==ON
extern vd_bool_t hrd_drv_test_module_init(vd_void_t);
#endif
vd_bool_t hrd_drivers_module_test_init(vd_void_t){
	#if DRIVER_FML_TEST_EN==ON
	hrd_drv_test_module_init();//drv�������Գ�ʼ��
	#endif
	return vd_true;
}
//ģ�鹦�ܲ��Ե��Ƚӿ�
extern vd_void_t hrd_drv_test_module_loop(vd_void_t);
vd_bool_t hrd_drivers_module_test(vd_void_t){
	#if DRIVER_FML_TEST_EN==ON
	hrd_drv_test_module_loop();//drv��������
	#endif
	//hrdģ�����
	/****�������****/
	#if (DRIVER_FML_ADC_KEY_TEST_EN==ON)
		hrd_adc_key_module_test();
	#endif
	#if (DRIVER_FML_BATTERY_TEST_EN == ON)
	battery_module_test(); //���ADC���ģ�����
	#endif
	#if (DRIVER_FML_INFRARED_TEST_EN == ON)
	infrared_module_test();
	#endif
	#if (DRIVER_FML_MOTOR_TYPE_DC_TEST_EN == ON)
	roller_module_test();
	#endif
	#if DRIVER_FML_LED_DISPLAY_TEST_EN ==ON
	led_display_module_test();
	#endif
	#if DRIVER_FML_MOTOR_TEST_EN ==ON
	hrd_motor_module_test();
	#endif
	#if DRIVER_FML_BMS_SPI_TEST_EN==ON
	bms_spi_module_test();//�����ز����ɼ�����
	#endif     
	#if DRIVER_FML_UART_DISPLAY_TEST_EN == ON
	uart_display_module_test();
	#endif
	#if DRIVER_FML_KEY_TEST_EN == ON
	key_module_test();
	#endif
	#if DRIVER_FML_LED_TEST_EN==ON
	led_module_test();
	#endif
	#if DRIVER_FML_FAN_TEST_EN==ON
	fan_module_test();
	#endif
	#if DRIVER_FML_HEAT_TEST_EN==ON
	heat_module_test();
	#endif
	#if DRIVER_FML_BEEP_TEST_EN==ON
	beep_module_test();
	#endif
	#if DRIVER_FML_ZERO_CROSSING_DETECTION_TEST_EN==ON
	zero_crossing_detection_module_test();
	#endif
	#if (DRIVER_FML_TEMP_TEST_EN == ON)
	temp_module_test();
	#endif
	#if DRIVER_FML_UART_WEIGHING_TEST_EN==ON
	weighing_uart_module_test();
	#endif
	return vd_true;
}

