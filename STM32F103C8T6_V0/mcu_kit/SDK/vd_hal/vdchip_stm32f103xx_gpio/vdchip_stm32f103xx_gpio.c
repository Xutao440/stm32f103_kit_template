//#include "vdchip_stm32f103xx_gpio.h"
//#include "vd_bsp_type.h"
//#include "base_type.h"
//#include "base_check.h"
//#include "log.h"
///* ======================================================================================
// * macros
// */
//#define PLATFORM_GPIO_NUM_MAX 34	//GPIO引脚数
//#define ITEM_NUM(items) (sizeof(items) / sizeof(items[0]))
//#define __STM32F103_PIN(index, gpio, gpio_index) {(index), (GPIO_TypeDef*)(GPIO##gpio), (GPIO_Pin_##gpio_index)}
//#define __STM32F103_PIN_DEFAULT  {-1, 0, 0}

///* ====================================================================================== */
///*pin 定义*/    //高位表示gourp，低位表示pin
//static const stm32f103_gpio_inf_t pins[] =
//{
//	/*GPIOA*/
//    __STM32F103_PIN(0x00, A, 0),  
//    __STM32F103_PIN(0x01, A, 1),
//    __STM32F103_PIN(0x02, A, 2),
//    __STM32F103_PIN(0x03, A, 3),
//    __STM32F103_PIN(0x04, A, 4),
//    __STM32F103_PIN(0x05, A, 5),
//    __STM32F103_PIN(0x06, A, 6),
//    __STM32F103_PIN(0x07, A, 7),
//	__STM32F103_PIN(0x08, A, 8),
//    __STM32F103_PIN(0x09, A, 9),
//    __STM32F103_PIN(0x0A, A, 10),
//    __STM32F103_PIN(0x0B, A, 11),
//	__STM32F103_PIN(0x0C, A, 12),
//    __STM32F103_PIN(0x0D, A, 13),
//	__STM32F103_PIN(0x0E, A, 14),
//    __STM32F103_PIN(0x0F, A, 15),
//	/*GPIOB*/
//    __STM32F103_PIN(0x10, B, 0),  
//    __STM32F103_PIN(0x11, B, 1),
//    __STM32F103_PIN(0x12, B, 2),
//    __STM32F103_PIN(0x13, B, 3),
//    __STM32F103_PIN(0x14, B, 4),
//    __STM32F103_PIN(0x15, B, 5),
//    __STM32F103_PIN(0x16, B, 6),
//    __STM32F103_PIN(0x17, B, 7),
//	__STM32F103_PIN(0x18, B, 8),
//    __STM32F103_PIN(0x19, B, 9),
//    __STM32F103_PIN(0x1A, B, 10),
//    __STM32F103_PIN(0x1B, B, 11),
//	__STM32F103_PIN(0x1C, B, 12),
//    __STM32F103_PIN(0x1D, B, 13),
//	__STM32F103_PIN(0x11, B, 14),
//    __STM32F103_PIN(0x1F, B, 15),
//	/*GPIOC*/
//	__STM32F103_PIN(0x20, C, 0),  
//    __STM32F103_PIN(0x21, C, 1),
//    __STM32F103_PIN(0x22, C, 2),
//    __STM32F103_PIN(0x23, C, 3),
//    __STM32F103_PIN(0x24, C, 4),
//    __STM32F103_PIN(0x25, C, 5),
//    __STM32F103_PIN(0x26, C, 6),
//    __STM32F103_PIN(0x27, C, 7),
//	__STM32F103_PIN(0x28, C, 8),
//    __STM32F103_PIN(0x29, C, 9),
//    __STM32F103_PIN(0x2A, C, 10),
//    __STM32F103_PIN(0x2B, C, 11),
//	__STM32F103_PIN(0x2C, C, 12),
//    __STM32F103_PIN(0x2D, C, 13),
//	__STM32F103_PIN(0x2E, C, 14),
//    __STM32F103_PIN(0x2F, C, 15),

//};

///*中断函数表*/
//vd_gpio_irq_hdr_t pin_irq_hdr_tab[GPIO_EXTIn_MAX]=
//{
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL},
//    { -1, 0, 0, NULL, NULL}, 
//};

///* ======================================================================================
// * declaration
// */
//static vd_uint8_t stm32f103_gpio_get_irqn(vd_bsp_port_e pin);
//const stm32f103_gpio_inf_t *get_pin(vd_bsp_port_e pin);
//static uint32_t stm32f103_gpio_get_PeriphClock(GPIO_TypeDef* gpio);

//static vd_bool_t stm32f103_gpio_init(vd_bsp_port_e pin, vd_gpio_mode_e mode);
//static vd_bool_t stm32f103_gpio_write( vd_bsp_port_e pin, vd_gpio_pin_status_e value);
//static vd_int32_t stm32f103_gpio_read(vd_bsp_port_e pin);
//static vd_bool_t stm32f103_gpio_attach_irq(vd_bsp_port_e pin,vd_gpio_irq_mode_e mode, void (*hdr)(void *args), void *args);
//static vd_bool_t stm32f103_gpio_dettach_irq(vd_bsp_port_e pin);
//static vd_bool_t stm32f103_gpio_irq_enable(vd_bsp_port_e pin, vd_gpio_irq_enable_e enabled);

///* ======================================================================================
// * globals
// */

///* ======================================================================================
// * helper
// */


///**
// * @brief Get the pin object
// * 
// * @param pin 
// * @return ** const struct pin_index* 
// */
//const stm32f103_gpio_inf_t *get_pin(vd_bsp_port_e pin)
//{
//    const stm32f103_gpio_inf_t* index = NULL;
//	int16_t i = 0;
//	
//    if (pin < ITEM_NUM(pins)){
//		for(i = 0; i < ITEM_NUM(pins); i++){
//			if(pins[i].type_gpio_num == pin){
//				log_info("index = %d, get_pin = %x \r\n",i,pins[i].type_gpio_num);
//				index = &pins[i];
//				break;
//			}
//		}	       
//        if (index->type_gpio_num == -1){
//            index = NULL;
//		}
//    }else{
//        index = NULL;
//    }

//    return index;
//};


///* ======================================================================================
// * private implementation
// */
///**
// * @brief stm32f103 gpio init
// * 
// * @param pin 
// * @param mode 
// * @return ** void 
// */
//static vd_bool_t stm32f103_gpio_init(vd_bsp_port_e pin, vd_gpio_mode_e mode)
//{	
//    const stm32f103_gpio_inf_t *index = NULL;	
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//	vd_uint32_t PeriphClock = 0;
//	vd_bool_t ret_value = vd_true;
//	
//	vd_check_return_val(pin < 0 || pin >= VD_PORT_GPIO_MAX, vd_false);
//	
//	GPIO_StructInit(&GPIO_InitStruct);
//    /*1.gpio get pin*/
//    index = get_pin(pin);
//	vd_check_return_val_error(index == NULL, vd_false, "[ %s ]: get pin fail \r\n",__FUNCTION__ );
//    /* 2.GPIO Periph clock enable */
//	PeriphClock = stm32f103_gpio_get_PeriphClock(index->group);
//	RCC_APB2PeriphClockCmd(PeriphClock, ENABLE);     		
//	
//    /*3. Configure GPIO_InitStructure */
//    GPIO_InitStruct.GPIO_Pin = index->pin;
//    /* mode setting */
//	switch(mode){
//	  case VD_GPIO_MODE_AIN:            //模拟输入
//		  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
//		  break;
//	  case VD_GPIO_MODE_IN_FLOATING:    //浮空输入
//		  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
//		  break;
//	  case VD_GPIO_MODE_IPD:            //输入下拉
//		  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPD;
//		  break;
//	  case VD_GPIO_MODE_IPU:            //输入上拉
//		  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;
//		  break;
//	  case VD_GPIO_MODE_OUT_OD:         //开漏输出
//		  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_OD;
//		  break;
//	  case VD_GPIO_MODE_OUT_PP:         //推挽输出
//		  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
//		  break;
//	  case VD_GPIO_MODE_AF_OD:          //复用开漏输出
//		  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_OD;
//		  break;
//	  case VD_GPIO_MODE_AF_PP:          //复用推挽输出
//		  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
//		  break;
//	  default:
//		  ret_value = vd_false;
//		  break;  
//	}
//    /* spend setting */
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	log_info("GPIO_Init index->gpio=%X \r\n", index->group);
//    /*gpio sdk init*/
//    GPIO_Init(index->group, &GPIO_InitStruct);
//	ret_value = vd_true;
//	
//    return ret_value;
//}


///**
// * @brief stm32f103 gpio write
// * 
// * @param pin 
// * @param value 
// * @return ** void 
// */
//static vd_bool_t stm32f103_gpio_write( vd_bsp_port_e pin, vd_gpio_pin_status_e value)
//{   
//    const stm32f103_gpio_inf_t *index = NULL;
//	
//	vd_check_return_val(pin < 0 || pin >= VD_PORT_GPIO_MAX, vd_false);
//	
//    index = get_pin(pin);
//	vd_check_return_val_error(index == NULL, vd_false, "[ %s ]: get pin fail \r\n",__FUNCTION__ );
//	GPIO_WriteBit(index->group, index->pin, (BitAction)value);
//    return vd_true;
//}


///**
// * @brief gpio read 
// * 
// * @param pin 
// * @return ** int 
// */
//static vd_int32_t stm32f103_gpio_read(vd_bsp_port_e pin)
//{  
//    int value = 0;
//    const stm32f103_gpio_inf_t *index = NULL;
//	
//	vd_check_return_val(pin < 0 || pin >= VD_PORT_GPIO_MAX, -1);
//	
//    index = get_pin(pin);
//	vd_check_return_val_error(index == NULL, vd_false, "[ %s ]: get pin fail \r\n",__FUNCTION__ );
//	value = GPIO_ReadInputDataBit(index->group, index->pin);
//    return value;
//}

///**
// * @brief stm32f103 irq request
// * 
// * @param pin 
// * @param mode 
// * @param hdr 
// * @param args 
// * @return ** vd_bool_t 
// */
//static vd_bool_t stm32f103_gpio_attach_irq(vd_bsp_port_e pin, vd_gpio_irq_mode_e mode, void (*hdr)(void *args), void *args)
//{   
//    const stm32f103_gpio_inf_t *index = NULL;
//    GPIO_InitTypeDef GPIO_Handle;
//	uint32_t PeriphClock = 0;
//	vd_uint8_t irq_map_index = 0;
//	vd_bool_t ret_value = vd_true;
//	
//	vd_check_return_val(pin < 0 || pin >= VD_PORT_GPIO_MAX, vd_false);
//	GPIO_StructInit(&GPIO_Handle);
//    index = get_pin(pin);
//	vd_check_return_val_error(index == NULL, vd_false, "[ %s ]: get pin fail \r\n",__FUNCTION__ );
//    /*****1.open clk*****/
//	PeriphClock = stm32f103_gpio_get_PeriphClock(index->group);
//	RCC_APB2PeriphClockCmd(PeriphClock, ENABLE);
//		
//    /* 2.Configure GPIO_InitStructure */
//	irq_map_index = pin & 0x0f;
//	if(pin_irq_hdr_tab[irq_map_index].index == -1){
//		pin_irq_hdr_tab[irq_map_index].line = index->pin;
//		pin_irq_hdr_tab[irq_map_index].index = irq_map_index;  
//		pin_irq_hdr_tab[irq_map_index].mode = mode;
//		pin_irq_hdr_tab[irq_map_index].hdr = hdr;
//		pin_irq_hdr_tab[irq_map_index].args = args;
//		log_info("irq_map_index = %d, gpio attach irq succeed! \r\n", irq_map_index);		
//	}else{
//		log_info("gpio attach irq fail!");
//		ret_value = vd_false;
//	}

//    return ret_value;
//}


///**
// * @brief stm32f103 irq unrequest
// * 
// * @param pin 
// * @return ** vd_int_t 
// */
//static vd_bool_t stm32f103_gpio_dettach_irq(vd_bsp_port_e pin)
//{
//    const stm32f103_gpio_inf_t *index = NULL;
//    GPIO_InitTypeDef GPIO_Handle = {0};
//    vd_int_t irqindex = 0;
//	uint32_t PeriphClock = 0;
//	vd_uint8_t irq_map_index = 0;
//	
//	vd_check_return_val(pin < 0 || pin >= VD_PORT_GPIO_MAX, vd_false);
//	
//	GPIO_StructInit(&GPIO_Handle);
//    index = get_pin(pin);
//	vd_check_return_val_error(index == NULL, vd_false, "[ %s ]: get pin fail \r\n",__FUNCTION__ );
//    /*****1.open clk*****/
//	PeriphClock = stm32f103_gpio_get_PeriphClock(index->group);
//	RCC_APB2PeriphClockCmd(PeriphClock, ENABLE);
//		
//    /* 2.Configure GPIO_InitStructure */
//	irq_map_index = pin & 0x0f;
//	if(pin_irq_hdr_tab[irq_map_index].index != -1){
//		pin_irq_hdr_tab[irqindex].index = -1;
//		pin_irq_hdr_tab[irqindex].mode = 0;
//		pin_irq_hdr_tab[irqindex].hdr = NULL;
//		pin_irq_hdr_tab[irqindex].args = NULL;		
//	}
//   return vd_true;
//}

///**
// * @brief gpio irq enable
// * 
// * @param pin 
// * @param enabled 
// * @return ** vd_int_t 
// */
//static vd_uint8_t stm32f103_gpio_get_irqn(vd_bsp_port_e pin)
//{
//	vd_uint8_t irqn = GPIO_IRQn_ERRO;
//	const stm32f103_gpio_inf_t *index = NULL;
//	
//	vd_check_return_val(pin < 0 || pin >= VD_PORT_GPIO_MAX, GPIO_IRQn_ERRO);	
//    index = get_pin(pin);
//	vd_check_return_val_error(index == NULL, GPIO_IRQn_ERRO, "[ %s ] get pin fail! \r\n",__FUNCTION__);
//	
//	if(index->pin == GPIO_Pin_0){
//		irqn = EXTI0_IRQn;
//		
//	}else if(index->pin == GPIO_Pin_1){
//		irqn = EXTI1_IRQn;
//		
//	}else if(index->pin == GPIO_Pin_2){
//		irqn = EXTI2_IRQn;
//		
//	}else if(index->pin == GPIO_Pin_3){
//		irqn = EXTI3_IRQn;
//		
//	}else if(index->pin == GPIO_Pin_4){
//		irqn = EXTI4_IRQn;
//		
//	}else if(index->pin <= GPIO_Pin_9){
//		irqn = EXTI9_5_IRQn;
//		
//	}else if(index->pin <= GPIO_Pin_15){
//		irqn = EXTI15_10_IRQn;
//	}
//    
//	return irqn;
//}

///**
// * @brief gpio irq enable
// * 
// * @param pin 
// * @param enabled 
// * @return ** vd_int_t 
// */
//static vd_bool_t stm32f103_gpio_irq_enable(vd_bsp_port_e pin, vd_gpio_irq_enable_e enabled)
//{  
//    const stm32f103_gpio_inf_t *index = NULL;
//    GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;	
//	uint32_t PeriphClock = 0;
//	vd_uint8_t irq_map_index = 0;
//	vd_uint8_t irqn = GPIO_IRQn_ERRO;
//	
//	vd_check_return_val(pin < 0 || pin >= VD_PORT_GPIO_MAX, vd_false);
//	/*结构体默认值*/
//	GPIO_StructInit(&GPIO_InitStructure);
//	EXTI_StructInit(&EXTI_InitStructure);
//	
//    index = get_pin(pin);
//	vd_check_return_val_error(index == NULL, vd_false, "[ %s ]: get pin fail \r\n",__FUNCTION__ );

//	irqn = stm32f103_gpio_get_irqn(index->type_gpio_num);
//	vd_check_return_val_error(irqn == GPIO_IRQn_ERRO, vd_false, "[ %s ]: get irqn fail \r\n",__FUNCTION__);
//	log_info("irqn = %d, gpio get irqn succeed \r\n",irqn);
//	
//    if (enabled == VD_GPIO_IRQ_ENABLE){
//		log_info("gpio irq enable! index->group=%X\r\n",index->group);
//        /*时钟配置*/
//		PeriphClock = stm32f103_gpio_get_PeriphClock(index->group);
//		RCC_APB2PeriphClockCmd(PeriphClock | RCC_APB2Periph_AFIO, ENABLE);    //复用和GPIO时钟使能
//		/*GPIO初始化*/
//		GPIO_InitStructure.GPIO_Pin = index->pin;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
//		GPIO_Init(index->group, &GPIO_InitStructure);

//		GPIO_EXTILineConfig((pin >> 4), (pin & 0x0f));	  //外部中断线配置	
//		/*外部中断配置*/
//		irq_map_index = pin & 0x0f;
//		if(pin_irq_hdr_tab[irq_map_index].index == irq_map_index){
//			switch (pin_irq_hdr_tab[irq_map_index].mode){
//				case VD_GPIO_IRQ_MODE_RISING :  //上升沿触发                    
//					EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//					log_info("set EXTI_Trigger_Rising\r\n");					
//					break;
//				
//				case VD_GPIO_IRQ_MODE_FALLING : //下降沿触发
//					EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
//					log_info("set EXTI_Trigger_Falling\r\n");					
//					break;
//				
//				case VD_GPIO_IRQ_MODE_RISING_FALLING : //上升和下降沿同时触发 
//					log_info("set EXTI_Trigger_Rising_Falling\r\n");						
//					EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;                 
//					break;
//				
//				default:
//					break;
//            }
//			log_info("EXTI_Lin = %x, irqn = %d\r\n", index->pin, irqn);
//			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //中断请求模式
//			EXTI_InitStructure.EXTI_Line = index->pin;              //外部中断线和pin相同
//			EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //使能
//			EXTI_Init(&EXTI_InitStructure);                         //初始化
//			/*set NVIC*/        
//			NVIC_InitStructure.NVIC_IRQChannel =irqn;               //外部中断号
//			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//抢占优先级
//			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //响应优先级
//			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//			NVIC_Init(&NVIC_InitStructure);				
//		}
//   
//    }else if (enabled == VD_GPIO_IRQ_DISABLE){
//        NVIC_InitStructure.NVIC_IRQChannel =irqn;              		    //外部中断号
//		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                //停止中断
//		NVIC_Init(&NVIC_InitStructure);		
//    }
//	
//    return vd_true;
//}


///* ======================================================================================
// * implementation
// */
///**
// * @brief  平台层注册
// * 
// * @param plat_gpio 
// * @return ** int 
// */
//vd_bool_t vd_stm32f103_gpio_register(vd_gpio_ops_t *ops)
// { 
//	vd_check_return_val(ops == NULL, vd_false);
//	 
//    ops->init = stm32f103_gpio_init;
//    ops->write = stm32f103_gpio_write;
//    ops->read = stm32f103_gpio_read;
//    ops->irq_requst = stm32f103_gpio_attach_irq;
//    ops->irq_unrequst = stm32f103_gpio_dettach_irq;
//    ops->irq_enable = stm32f103_gpio_irq_enable;
//	 
//	 return vd_true;
// }
// 
///*
//************************************************************
//*	函数名称：	vd_bsp_gpio_get_PeriphClock
//*	函数功能：	获取GPIO的时钟
//*	入口参数：	gpio：GPIO组
//*	返回参数：	时钟		
//************************************************************
//*/
//static uint32_t stm32f103_gpio_get_PeriphClock(GPIO_TypeDef* gpio)
//{
//	uint32_t PeriphClock = 0;
//	
//	if(gpio == GPIOA){
//		PeriphClock = RCC_APB2Periph_GPIOA;
//	}else if(gpio == GPIOB){
//		PeriphClock = RCC_APB2Periph_GPIOB;
//	}else if(gpio == GPIOC){
//		PeriphClock = RCC_APB2Periph_GPIOC;
//	}else if(gpio == GPIOD){
//		PeriphClock = RCC_APB2Periph_GPIOD;
//	}else if(gpio == GPIOE){
//		PeriphClock = RCC_APB2Periph_GPIOE;
//	}else if(gpio == GPIOF){
//		PeriphClock = RCC_APB2Periph_GPIOF;
//	}else if(gpio == GPIOG){
//		PeriphClock = RCC_APB2Periph_GPIOG;
//	}
//	
//	return PeriphClock;

//}

////中断处理函数
//void GPIO_IRQHandler(VD_EXTI_E EXTIx)
//{
//	vd_uint8_t start_num = 0, end_mun = 0, index = 0;
//	index = EXTIx;
//	
//	switch(index){
//		case VD_EXTI0:    start_num = 0; end_mun = start_num +1; break;
//		case VD_EXTI1:    start_num = 1; end_mun = start_num +1; break;
//		case VD_EXTI2:    start_num = 2; end_mun = start_num +1; break;
//		case VD_EXTI3:    start_num = 3; end_mun = start_num +1; break;
//		case VD_EXTI4:    start_num = 4; end_mun = start_num +1; break;
//		case VD_EXTI9_5:  start_num = 5; end_mun = start_num +5; break;
//		case VD_EXTI15_10:start_num = 10;end_mun = start_num +6; break;
//	}
//		
//	for(int i = start_num; i < end_mun; i++){
//		if(pin_irq_hdr_tab[i].index != -1){
//			if(EXTI_GetITStatus(pin_irq_hdr_tab[i].line != RESET)){
//				log_info("EXTI%d, GPIO_IRQHandler \r\n",i);
//				pin_irq_hdr_tab[i].hdr(pin_irq_hdr_tab[i].args);
//				EXTI_ClearITPendingBit(pin_irq_hdr_tab[i].line);	
//			}	
//		}			
//	}
//}

///******************** 系统中断函数****************/

//void EXTI0_IRQHandler(void)
//{	
//	 GPIO_IRQHandler(VD_EXTI0);
//}

//void EXTI1_IRQHandler(void)
//{
//	GPIO_IRQHandler(VD_EXTI1);
//}

//void EXTI2_IRQHandler(void)
//{
//	GPIO_IRQHandler(VD_EXTI2);
//}

//void EXTI3_IRQHandler(void)
//{
//	GPIO_IRQHandler(VD_EXTI3);
//}

//void EXTI4_IRQHandler(void)
//{
//	GPIO_IRQHandler(VD_EXTI4);
//}

//void EXTI9_5_IRQHandler(void)
//{
//	GPIO_IRQHandler(VD_EXTI9_5);
//}

//void EXTI15_10_IRQHandler(void)
//{
//	GPIO_IRQHandler(VD_EXTI15_10);
//}

