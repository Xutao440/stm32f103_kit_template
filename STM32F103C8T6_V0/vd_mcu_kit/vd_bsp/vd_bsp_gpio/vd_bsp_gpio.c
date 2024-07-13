#include "vd_bsp_gpio.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"

/*�жϺ�����*/
vd_gpio_irq_hdr_t pin_irq_hdr_tab[GPIO_EXTIn_MAX]=
{
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL},
    { -1, 0, 0, 0, NULL, NULL}, 
};

/*
************************************************************
*	�������ƣ�	vd_bsp_gpio_get_irqn
*	�������ܣ�	��ȡgpio�жϺ�
*	��ڲ�����	gpio_port��port
*	���ز�����	�ɹ������жϺ�
************************************************************
*/
static vd_int8_t vd_bsp_gpio_get_irqn(GPIO_PORT_E gpio_port)
{
	vd_int8_t irqn = GPIO_IRQn_ERRO;
	vd_uint8_t pin_number = 0;
	
	vd_check_return_val_error(gpio_port > GPIO_MAX, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: EINVAL \r\n",__FUNCTION__ );

	pin_number = gpio_port & 0x0f;
	
	if(pin_number == 0){
		irqn = EXTI0_IRQn;
		
	}else if(pin_number == 1){
		irqn = EXTI1_IRQn;
		
	}else if(pin_number == 2){
		irqn = EXTI2_IRQn;
		
	}else if(pin_number == 3){
		irqn = EXTI3_IRQn;
		
	}else if(pin_number == 4){
		irqn = EXTI4_IRQn;
		
	}else if(pin_number <= 9){
		irqn = EXTI9_5_IRQn;
		
	}else if(pin_number <= 15){
		irqn = EXTI15_10_IRQn;
	}
    
	return irqn;
}
/*
************************************************************
*	�������ƣ�	vd_bsp_get_group_and_pin
*	�������ܣ�	�õ�group��pin
*	��ڲ�����	gpio_port��port�� group���������ַ��pin������pin
*	���ز�����	��
************************************************************
*/
vd_int32_t vd_bsp_get_group_and_pin(GPIO_PORT_E gpio_port, uint32_t* group_addr, uint16_t* pin)
{
	uint8_t group_4bit_h = 0, pin_4bit_l = 0;
	
	vd_check_return_val((gpio_port > GPIO_MAX) || group_addr == NULL || pin == NULL, -VD_BSP_ERR_PORT_INVALID);
	
	group_4bit_h = gpio_port >> 4;
	*group_addr = (APB2PERIPH_BASE + (0x0800 + 0x0400 * group_4bit_h));
	pin_4bit_l = gpio_port & 0x0f;
	*pin = (uint16_t)(0x0001 << pin_4bit_l);
	
	return VD_BSP_NOERROR;
}

/*
************************************************************
*	�������ƣ�	vd_bsp_gpio_get_PeriphClock
*	�������ܣ�	��ȡGPIO��ʱ��
*	��ڲ�����	gpio��GPIO��
*	���ز�����	ʱ��		
************************************************************
*/
uint32_t vd_bsp_gpio_get_PeriphClock(GPIO_TypeDef* gpio)
{
	uint32_t PeriphClock = 0;
	
	if(gpio == GPIOA){
		PeriphClock = RCC_APB2Periph_GPIOA;
	}else if(gpio == GPIOB){
		PeriphClock = RCC_APB2Periph_GPIOB;
	}else if(gpio == GPIOC){
		PeriphClock = RCC_APB2Periph_GPIOC;
	}else if(gpio == GPIOD){
		PeriphClock = RCC_APB2Periph_GPIOD;
	}else if(gpio == GPIOE){
		PeriphClock = RCC_APB2Periph_GPIOE;
	}else if(gpio == GPIOF){
		PeriphClock = RCC_APB2Periph_GPIOF;
	}else if(gpio == GPIOG){
		PeriphClock = RCC_APB2Periph_GPIOG;
	}
	
	return PeriphClock;

}


/*
************************************************************
*	�������ƣ�	vd_bsp_gpio_init
*	�������ܣ�	gpio��ʼ��
*	��ڲ�����	gpio_port��port�� mode������ģʽ
*	���ز�����	��
************************************************************
*/
vd_int32_t vd_bsp_gpio_init(GPIO_PORT_E gpio_port, GPIO_MODE_E mode)
{		
    GPIO_InitTypeDef GPIO_InitStruct = {0};
	vd_uint32_t PeriphClock = 0;
	vd_int32_t ret_value = VD_BSP_NOERROR;
	uint32_t group_adrr = 0;
	GPIO_TypeDef* group = NULL;
	uint16_t pin = 0;
	
	vd_check_return_val(gpio_port > GPIO_MAX, -VD_BSP_ERR_PORT_INVALID);
	
	GPIO_StructInit(&GPIO_InitStruct);
    /*1.gpio get pin*/
	ret_value = vd_bsp_get_group_and_pin(gpio_port, &group_adrr, &pin);
	group = (GPIO_TypeDef*) group_adrr;
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: get pin fail! \r\n",__FUNCTION__ );
    /* 2.GPIO Periph clock enable */
	PeriphClock = vd_bsp_gpio_get_PeriphClock(group);
	RCC_APB2PeriphClockCmd(PeriphClock, ENABLE);     		
	
    /*3. Configure GPIO_InitStructure */
    GPIO_InitStruct.GPIO_Pin = pin;
    /* mode setting */
	 GPIO_InitStruct.GPIO_Mode = (GPIOMode_TypeDef)mode;
    /* spend setting */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	log_info("GPIO_Init index->gpio=%X \r\n", group);
    /*gpio sdk init*/
    GPIO_Init(group, &GPIO_InitStruct);
	ret_value = vd_true;
	
    return VD_BSP_NOERROR;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_gpio_write
*	�������ܣ�	gpioд
*	��ڲ�����	gpio_port��port�� value������ֵ
*	���ز�����	�ɹ�����0
************************************************************
*/
vd_int32_t vd_bsp_gpio_write( GPIO_PORT_E gpio_port, GPIO_STATE_E value)
{   
	uint32_t group_adrr = 0;
	vd_int32_t ret_value = VD_BSP_NOERROR;
	GPIO_TypeDef* group = NULL;
	uint16_t pin = 0;
	vd_check_return_val(gpio_port > GPIO_MAX, -VD_BSP_ERR_PORT_INVALID);
	
	ret_value = vd_bsp_get_group_and_pin(gpio_port, &group_adrr, &pin);
	group = (GPIO_TypeDef*) group_adrr;
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: get pin fail! \r\n",__FUNCTION__ );
	GPIO_WriteBit(group, pin, (BitAction)value);
	
    return ret_value;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_gpio_read
*	�������ܣ�	gpio��
*	��ڲ�����	gpio_port��port
*	���ز�����	�ɹ�����0
************************************************************
*/
vd_int32_t vd_bsp_gpio_read(GPIO_PORT_E gpio_port)
{  
	uint32_t group_adrr = 0;
	vd_int32_t ret_value = VD_BSP_NOERROR;
	GPIO_TypeDef* group = NULL;
	uint16_t pin = 0;
	
	vd_check_return_val(gpio_port > GPIO_MAX, -VD_BSP_ERR_PORT_INVALID);
	
	ret_value = vd_bsp_get_group_and_pin(gpio_port, &group_adrr, &pin);
	group = (GPIO_TypeDef*) group_adrr;
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: get pin fail! \r\n",__FUNCTION__ );
	ret_value = GPIO_ReadInputDataBit(group, pin);
	
    return ret_value;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_gpio_requst_irq
*	�������ܣ�	ע��GPIO�ⲿ�ж�
*	��ڲ�����	gpio_port��port�� mode���ж�ģʽ�� hdr���ص�����ָ�룬 args���ص���������
*	���ز�����	�ɹ�����0
************************************************************
*/
vd_int32_t vd_bsp_gpio_register_irq(GPIO_PORT_E gpio_port, GPIO_MODE_E gpio_mode, GPIO_IRQ_MODE_E mode, vd_void_t (*hdr)(vd_void_t *args), vd_void_t  *args)
{   
    GPIO_InitTypeDef GPIO_Handle;
	GPIO_TypeDef* group = NULL;
	uint32_t group_adrr = 0, PeriphClock = 0;
	vd_uint8_t irq_map_index = 0;
	vd_int32_t ret_value = VD_BSP_NOERROR;
	uint16_t pin = 0;	
		
	vd_check_return_val_error(gpio_port > GPIO_MAX || mode > VD_GPIO_IRQ_MODE_MAX, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: EINVAL \r\n",__FUNCTION__ );
	
	GPIO_StructInit(&GPIO_Handle);
	ret_value = vd_bsp_get_group_and_pin(gpio_port, &group_adrr, &pin);
	group = (GPIO_TypeDef*) group_adrr;
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: get pin fail! \r\n",__FUNCTION__ );
	
    /*****1.open clk*****/
	PeriphClock = vd_bsp_gpio_get_PeriphClock(group);
	RCC_APB2PeriphClockCmd(PeriphClock, ENABLE);
		
    /* 2.Configure GPIO_InitStructure */
	irq_map_index = gpio_port & 0x0f;
	if(pin_irq_hdr_tab[irq_map_index].index == -1){
		pin_irq_hdr_tab[irq_map_index].line = pin;
		pin_irq_hdr_tab[irq_map_index].index = irq_map_index;  
		pin_irq_hdr_tab[irq_map_index].mode = mode;
		pin_irq_hdr_tab[irq_map_index].hdr = hdr;
		pin_irq_hdr_tab[irq_map_index].args = args;
		pin_irq_hdr_tab[irq_map_index].gpio_mode = gpio_mode;
		log_info("irq_map_index = %d, gpio attach irq succeed! \r\n", irq_map_index);
		
	}else{
		log_info("gpio attach irq fail!");
		ret_value = vd_false;
	}

    return ret_value;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_gpio_unregister_irq
*	�������ܣ�	ȡ��ע��GPIO�ⲿ�ж�
*	��ڲ�����	gpio_port��port
*	���ز�����	�ɹ�����0
************************************************************
*/
vd_int32_t vd_bsp_gpio_unregister_irq(GPIO_PORT_E gpio_port)
{
    vd_int_t irqindex = 0;
	vd_uint8_t irq_map_index = 0;
	
	vd_check_return_val_error(gpio_port > GPIO_MAX, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: EINVAL \r\n",__FUNCTION__ );
			
	irq_map_index = gpio_port & 0x0f;
	if(pin_irq_hdr_tab[irq_map_index].index != -1){
		pin_irq_hdr_tab[irqindex].index = -1;
		pin_irq_hdr_tab[irqindex].mode = 0;
		pin_irq_hdr_tab[irqindex].hdr = NULL;
		pin_irq_hdr_tab[irqindex].args = NULL;		
	}
   return VD_BSP_NOERROR;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_gpio_irq_enable
*	�������ܣ�	gpio�жϿ��غ���
*	��ڲ�����	gpio_port��port, state:�ж�״̬
*	���ز�����	�ɹ������жϺ�
************************************************************
*/
vd_int32_t vd_bsp_gpio_irq_switch(GPIO_PORT_E gpio_port, GPIO_IRQ_STATE_E state)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	uint32_t PeriphClock = 0, group_adrr = 0;
	vd_uint8_t irq_map_index = 0, irqn = GPIO_IRQn_ERRO;
	uint16_t pin = 0;
	GPIO_TypeDef* group = NULL;
	vd_int32_t ret_value = VD_BSP_NOERROR;
	
	vd_check_return_val_error(gpio_port > GPIO_MAX || state > VD_GPIO_IRQ_ENABLE_MAX,        \
	                          -VD_BSP_ERR_PORT_INVALID, "[ %s ]: EINVAL \r\n",__FUNCTION__ );
	
	irq_map_index = gpio_port & 0x0f;
	/*�ṹ��Ĭ��ֵ*/
	GPIO_StructInit(&GPIO_InitStructure);
	EXTI_StructInit(&EXTI_InitStructure);
	
	ret_value = vd_bsp_get_group_and_pin(gpio_port, &group_adrr, &pin);
	group = (GPIO_TypeDef*) group_adrr;
	vd_check_return_val_error(ret_value != VD_BSP_NOERROR, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: get pin fail! \r\n",__FUNCTION__ );
		
	irqn = vd_bsp_gpio_get_irqn(gpio_port);
	vd_check_return_val_error(irqn == GPIO_IRQn_ERRO, vd_false, "[ %s ]: get irqn fail \r\n",__FUNCTION__);
	log_info("irqn = %d, gpio get irqn succeed \r\n",irqn);
	
    if (state == VD_GPIO_IRQ_ENABLE){
		log_info("gpio irq enable! group = %X\r\n",group);
        /*ʱ������*/
		PeriphClock = vd_bsp_gpio_get_PeriphClock(group);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);    //���ú�GPIOʱ��ʹ��
		
		/*GPIO��ʼ��*/
		GPIO_InitStructure.GPIO_Pin = pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Mode = pin_irq_hdr_tab[irq_map_index].gpio_mode; 
		GPIO_Init(group, &GPIO_InitStructure);

		GPIO_EXTILineConfig((gpio_port >> 4), (gpio_port & 0x0f));	  //�ⲿ�ж�������	

		/*�ⲿ�ж�����*/		
		if(pin_irq_hdr_tab[irq_map_index].index == irq_map_index){
			switch (pin_irq_hdr_tab[irq_map_index].mode){
				case VD_GPIO_IRQ_MODE_RISING :  //�����ش���                    
					EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
					log_info("set EXTI_Trigger_Rising\r\n");					
					break;
				
				case VD_GPIO_IRQ_MODE_FALLING : //�½��ش���
					EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
					log_info("set EXTI_Trigger_Falling\r\n");					
					break;
				
				case VD_GPIO_IRQ_MODE_RISING_FALLING : //�������½���ͬʱ���� 
					log_info("set EXTI_Trigger_Rising_Falling\r\n");						
					EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;                 
					break;
				
				default:
					break;
            }
			log_info("EXTI_Lin = %x, irqn = %d\r\n", pin, irqn);
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //�ж�����ģʽ
			EXTI_InitStructure.EXTI_Line = pin;                     //�ⲿ�ж��ߺ�pin��ͬ
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //ʹ��
			EXTI_Init(&EXTI_InitStructure);                         //��ʼ��
			/*set NVIC*/        
			NVIC_InitStructure.NVIC_IRQChannel =irqn;               //�ⲿ�жϺ�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //��Ӧ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
			NVIC_Init(&NVIC_InitStructure);				
		}
   
    }else if (state == VD_GPIO_IRQ_DISABLE){
        NVIC_InitStructure.NVIC_IRQChannel =irqn;              		    //�ⲿ�жϺ�
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;                //ֹͣ�ж�
		NVIC_Init(&NVIC_InitStructure);		
    }
	
    return vd_true;
}


/********************************** �жϴ����� *************************************/

void GPIO_IRQHandler(VD_EXTI_E EXTIx)
{
	vd_uint8_t start_num = 0, end_mun = 0, index = 0;
	index = EXTIx;
	
	switch(index){
		case VD_EXTI0:    start_num = 0; end_mun = start_num +1; break;
		case VD_EXTI1:    start_num = 1; end_mun = start_num +1; break;
		case VD_EXTI2:    start_num = 2; end_mun = start_num +1; break;
		case VD_EXTI3:    start_num = 3; end_mun = start_num +1; break;
		case VD_EXTI4:    start_num = 4; end_mun = start_num +1; break;
		case VD_EXTI9_5:  start_num = 5; end_mun = start_num +5; break;
		case VD_EXTI15_10:start_num = 10;end_mun = start_num +6; break;
	}
		
	for(int i = start_num; i < end_mun; i++){
		if(pin_irq_hdr_tab[i].index != -1){
			if(EXTI_GetITStatus(pin_irq_hdr_tab[i].line) != RESET){
				log_info("EXTI%d, GPIO_IRQHandler \r\n",i);
				pin_irq_hdr_tab[i].hdr(pin_irq_hdr_tab[i].args);
				EXTI_ClearITPendingBit(pin_irq_hdr_tab[i].line);	
			}	
		}			
	}
}

/******************** ϵͳ�жϺ���****************/

void EXTI0_IRQHandler(void)
{	
	GPIO_IRQHandler(VD_EXTI0);
}

void EXTI1_IRQHandler(void)
{
	GPIO_IRQHandler(VD_EXTI1);
}

void EXTI2_IRQHandler(void)
{
	GPIO_IRQHandler(VD_EXTI2);
}

void EXTI3_IRQHandler(void)
{	
	GPIO_IRQHandler(VD_EXTI3);
}

void EXTI4_IRQHandler(void)
{
	GPIO_IRQHandler(VD_EXTI4);
}

void EXTI9_5_IRQHandler(void)
{
	GPIO_IRQHandler(VD_EXTI9_5);
}

void EXTI15_10_IRQHandler(void)
{
	GPIO_IRQHandler(VD_EXTI15_10);
}





