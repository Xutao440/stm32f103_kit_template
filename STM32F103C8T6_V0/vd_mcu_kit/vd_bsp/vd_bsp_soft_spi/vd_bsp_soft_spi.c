#include "vd_bsp_soft_spi.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"

vd_soft_spi_t  master_spi;

vd_slave_spi_t register_slave_dev[SLAVE_DEV_MAX] = {0};
vd_uint8_t     register_slave_mask = 0;


/*
************************************************************
*	�������ƣ�	SPI_Start
*	�������ܣ�	��ʼ�ź�
*	��ڲ�����	��
*	���ز�����	��
************************************************************
*/
void SPI_Start(vd_int32_t slave_dev_id)
{
	GPIO_PORT_E slave_port = 0;
	
	slave_port = register_slave_dev[slave_dev_id].ss_port;
	SPI_W_SS(slave_port, VD_GPIO_LOW);	

}

/*
************************************************************
*	�������ƣ�	SPI_Stop
*	�������ܣ�	ֹͣ�ź�
*	��ڲ�����	��
*	���ز�����	��
************************************************************
*/
void SPI_Stop(vd_int32_t slave_dev_id)
{
	GPIO_PORT_E slave_port = 0;
	
	slave_port = register_slave_dev[slave_dev_id].ss_port;
	SPI_W_SS(slave_port, VD_GPIO_HIGH);

}

/*
************************************************************
*	�������ƣ�	vd_bsp_spi_send_byte
*	�������ܣ�	����һ���ֽ�
*	��ڲ�����	byte�����͵�����
*	���ز�����	��
************************************************************
*/
vd_uint8_t vd_bsp_spi_send_byte(uint8_t byte)
{	
	vd_uint8_t spi_mode = 0, index = 0, rx_data = 0;
	spi_mode = master_spi.spi_mode;
	
	switch(spi_mode){
		case SPI_00_MODE:   //����SCKΪ�͵�ƽ��SCK��һ�������������ݣ��ڶ��������Ƴ�����
			log_info("SPI_00_MODE\r\n");
			for (index = 0; index < 8; index++){
				SPI_W_MOSI((byte & (0x80 >> index)) ? VD_GPIO_HIGH : VD_GPIO_LOW);	
				SPI_W_SCK(VD_GPIO_HIGH);										
				if(SPI_R_MISO() == 1){
					rx_data |= (0x80 >> index);
				}
				SPI_W_SCK(VD_GPIO_LOW);				
			}
			break;
		case SPI_01_MODE:   //����SCKΪ�͵�ƽ, SCK��һ�������Ƴ����ݣ��ڶ���������������     #### ���������쳣 #####
			log_info("SPI_01_MODE\r\n");
			for (index = 0; index < 8; index++){
				SPI_W_SCK(VD_GPIO_HIGH);				
				SPI_W_MOSI((byte & (0x80 >> index)) ? VD_GPIO_HIGH : VD_GPIO_LOW);
				SPI_W_SCK(VD_GPIO_LOW);					
				if(SPI_R_MISO() == 1){
					rx_data |= (0x80 >> index);
				}							
			}
			break;
		case SPI_10_MODE:   //����SCKΪ�ߵ�ƽ���½��ض����ݣ�������д����                    #### ���������쳣 #####
			log_info("SPI_10_MODE\r\n");
			for (index = 0; index < 8; index++){
				SPI_W_MOSI((byte & (0x80 >> index)) ? VD_GPIO_HIGH : VD_GPIO_LOW);	
				SPI_W_SCK(VD_GPIO_LOW);										
				if(SPI_R_MISO() == 1){
					rx_data |= (0x80 >> index);
				}
				SPI_W_SCK(VD_GPIO_HIGH);				
			}
			break;
			
		case SPI_11_MODE:   //����SCKΪ�ߵ�ƽ, �½���д���ݣ������ض�����                    #### ���������쳣 #####
			log_info("SPI_11_MODE\r\n");
			for (index = 0; index < 8; index++){
				SPI_W_SCK(VD_GPIO_LOW);				
				SPI_W_MOSI((byte & (0x80 >> index)) ? VD_GPIO_HIGH : VD_GPIO_LOW);
				SPI_W_SCK(VD_GPIO_HIGH);					
				if(SPI_R_MISO() == 1){
					rx_data |= (0x80 >> index);
				}							
			}
			break;
		
		default:
			break;	
	}
	
	return rx_data;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_soft_i2c_init
*	�������ܣ�	���i2c��ʼ��
*	��ڲ�����	soft_i2c���ṹ��
*	���ز�����	�ɹ�����0 �� ʧ�ܷ��� < 0
************************************************************
*/
vd_int32_t vd_bsp_soft_spi_init(vd_soft_spi_t*  master)
{
	vd_int32_t ret_value = VD_BSP_NOERROR;
	vd_soft_spi_t* soft_spi = NULL;
	soft_spi = &master_spi;
	
	vd_check_return_val_error(master == NULL, -VD_BSP_ERR_PARAMETER, "[ %s ]: parameter error\r\n",__FUNCTION__ );
	
	/*GPIO��ʼ��*/
	ret_value = vd_bsp_gpio_init(master->mosi_port, VD_GPIO_MODE_OUT_OD);  //��ʼ��mosi����
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: mosi init fail\r\n",__FUNCTION__ );
	ret_value = vd_bsp_gpio_init(master->sck_port, VD_GPIO_MODE_OUT_OD);   //��ʼ��sck����
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: sck init fail\r\n",__FUNCTION__ );
	ret_value = vd_bsp_gpio_init(master->ss_port, VD_GPIO_MODE_OUT_OD);    //��ʼ��ss����
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: ss init fail\r\n",__FUNCTION__ );
	ret_value = vd_bsp_gpio_init(master->miso_port, VD_GPIO_MODE_IPU);     //��ʼ��miso����
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: miso init fail\r\n",__FUNCTION__ );

	
	/*ע��*/
	master_spi.miso_port = master->miso_port;
	master_spi.mosi_port = master->mosi_port;
	master_spi.sck_port = master->sck_port;
	master_spi.ss_port = master->ss_port;
	master_spi.spi_mode = master->spi_mode;
	
	register_slave_dev[0].spi_mode = master->spi_mode;
	register_slave_dev[0].ss_port = master->ss_port;
	register_slave_mask = 0x01;
	if((master->spi_mode == SPI_00_MODE) ||(master->spi_mode == SPI_01_MODE)){
		SPI_W_SCK(VD_GPIO_LOW);
	}else{
		SPI_W_SCK(VD_GPIO_HIGH);
	}
	SPI_W_SS(master->ss_port, VD_GPIO_HIGH);
	return 0;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_soft_i2c_init
*	�������ܣ�	���i2c��ʼ��
*	��ڲ�����	soft_i2c���ṹ��
*	���ز�����	�ɹ�����0 �� ʧ�ܷ��� < 0
************************************************************
*/
vd_int32_t vd_bsp_soft_spi_add_slave_dev(vd_slave_spi_t*  slave_dev)
{
	vd_int32_t slave_dev_id = -1;
	vd_int8_t  index = 0;
	
	vd_check_return_val_error(slave_dev == NULL, -VD_BSP_ERR_PARAMETER, "[ %s ]: parameter error\r\n",__FUNCTION__ );
	for(index = 0; index < SLAVE_DEV_MAX; index++){
		if(((register_slave_mask >> index) &0x01) == 0x00){
			register_slave_mask |= (0x01 << index);
			register_slave_dev[index].spi_mode = slave_dev->spi_mode;
			register_slave_dev[index].ss_port = slave_dev->ss_port;
			slave_dev_id = index;
			break;
		}
	}
	
	vd_check_return_val_error(index >= SLAVE_DEV_MAX, -VD_BSP_ERR_SDK, "[ %s ]: add slave dev fail\r\n",__FUNCTION__ );
	
	vd_bsp_gpio_init(slave_dev->ss_port, VD_GPIO_MODE_OUT_OD);    //��ʼ��ss����
	
	
	if((slave_dev->spi_mode == SPI_00_MODE) ||(slave_dev->spi_mode == SPI_01_MODE)){
		SPI_W_SCK(VD_GPIO_LOW);
	}else{
		SPI_W_SCK(VD_GPIO_HIGH);
	}
	SPI_W_SS(slave_dev->ss_port, VD_GPIO_HIGH);
	return slave_dev_id;
}



///*
//************************************************************
//*	�������ƣ�	IIC_Stop
//*	�������ܣ�	ֹͣ�ź�
//*	��ڲ�����	��
//*	���ز�����	��
//************************************************************
//*/
//static void IIC_Stop(void)
//{
//	SDA_L;						//����SDA��
//	SCL_L;						//����SCL��
//	IIC_Delay();		        //��ʱ���ٶȿ���	
//	SCL_H;						//����SCL��
//	SDA_H;						//����SDA�ߣ���SCL��Ϊ��ʱ��SDA��һ�������ش���ֹͣ�ź�
//	IIC_Delay();
//}


///*
//************************************************************
//*	�������ƣ�	IIC_WaitAck
//*	�������ܣ�	�ȴ�ack�ź�
//*	��ڲ�����	timeout: ��ʱʱ��
//*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
//************************************************************
//*/
//static vd_bool_t IIC_WaitAck(vd_uint32_t timeout)
//{	
//	SDA_H;
//	IIC_Delay();			//����SDA��
//	SCL_H;
//	IIC_Delay();			//����SCL��
//	
//	while(SDA_R){							//�������SDA��Ϊ1����ȴ���Ӧ���ź�Ӧ��0
//		if(--timeout){
//			log_info("WaitAck TimeOut\r\n");
//			IIC_Stop();						//��ʱδ�յ�Ӧ����ֹͣ����			
//			return IIC_Err;					//����ʧ��
//		}	
//		IIC_Delay();
//	}
//	SCL_L;									//����SCL�ߣ��Ա�����շ�����
//	
//	return IIC_OK;							//���سɹ�
//	
//}


///*
//************************************************************
//*	�������ƣ�	IIC_Ack
//*	�������ܣ�	���IIC����һ��Ӧ��
//*	��ڲ�����	��
//*	���ز�����	��
//************************************************************
//*/
//static void IIC_Ack(void)
//{
//	SCL_L;						//����SCL��
//	SDA_L;						//����SDA��
//	IIC_Delay();
//	SCL_H;						//����SCL��
//	IIC_Delay();
//	SCL_L;						//����SCL��
//}


///*
//************************************************************
//*	�������ƣ�	IIC_NAck
//*	�������ܣ�	����һ����Ӧ��
//*	��ڲ�����	��
//*	���ز�����	��
//************************************************************
//*/
//static void IIC_NAck(void)
//{	
//	SCL_L;						//����SCL��
//	SDA_H;						//����SDA��
//	IIC_Delay();
//	SCL_H;						//����SCL��
//	IIC_Delay();
//	SCL_L;						//����SCL��	
//}

///*
//************************************************************
//*	�������ƣ�	vd_bsp_soft_i2c_write_byte
//*	�������ܣ�	����һ���ֽ�
//*	��ڲ�����	byte����������
//*	���ز�����	��
//************************************************************
//*/
//static vd_void_t IIC_SendByte(vd_uint8_t byte)
//{
//	vd_uint8_t count = 0;
//	
//    SCL_L;							//����ʱ�ӿ�ʼ���ݴ���	
//    for(; count < 8; count++){		//ѭ��8�Σ�ÿ�η���һ��bit
//		if(byte & 0x80){		    //�������λ
//			SDA_H;
//		}else{
//			SDA_L;
//		}
//		byte <<= 1;					//byte����1λ		
//		IIC_Delay();
//		SCL_H;
//		IIC_Delay();
//		SCL_L;
//    }
//}

///*
//************************************************************
//*	�������ƣ�	vd_bsp_soft_i2c_read_byte
//*	�������ܣ�  ��һ���ֽ�
//*	��ڲ�����	��
//*	���ز�����	���յ����ֽ�����
//************************************************************
//*/
//static vd_uint8_t IIC_RecvByte(vd_void_t)
//{
//	vd_uint8_t count = 0, receive = 0;
//	
//	SDA_H;							//����SDA�ߣ���©״̬�£����������Ա��ȡ����
//    for(; count < 8; count++ ){		//ѭ��8�Σ�ÿ�η���һ��bit
//		SCL_L;
//		IIC_Delay();
//		SCL_H;	
//        receive <<= 1;				//����һλ	
//        if(SDA_R){					//���SDA��Ϊ1����receive����������ÿ���������Ƕ�bit0��+1��Ȼ����һ��ѭ����������һ��
//			receive++;
//		}
//		IIC_Delay();
//    }
//	
//    return receive;
//}


///*
//************************************************************
//*	�������ƣ�	vd_bsp_soft_i2c_init
//*	�������ܣ�	���i2c��ʼ��
//*	��ڲ�����	soft_i2c���ṹ��
//*	���ز�����	�ɹ�����0 �� ʧ�ܷ��� < 0
//************************************************************
//*/
//vd_int32_t vd_bsp_soft_i2c_init(vd_soft_i2c_t* soft_i2c)
//{
//	vd_int32_t ret_value = VD_BSP_NOERROR;
//	
//	vd_check_return_val_error(soft_i2c == NULL, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: EINVAL \r\n",__FUNCTION__ );
//	
//	/*GPIO��ʼ��*/
//	ret_value = vd_bsp_gpio_init(soft_i2c->scl_prot, VD_GPIO_MODE_OUT_OD);  //��ʼ��scl����
//	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: gpio scl init fail\r\n",__FUNCTION__ );
//	ret_value = vd_bsp_gpio_init(soft_i2c->sda_prot, VD_GPIO_MODE_OUT_OD);  //��ʼ��scl����
//	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: gpio sda init fail\r\n",__FUNCTION__ );
//	
//	/*ע��*/
//	vd_g_soft_i2c_master.scl_prot = soft_i2c->scl_prot;
//	vd_g_soft_i2c_master.sda_prot = soft_i2c->sda_prot;
//	vd_g_soft_i2c_master.reg_bit = soft_i2c->reg_bit;
//	vd_g_soft_i2c_master.speed = soft_i2c->speed;
//	vd_g_soft_i2c_master.slave_addr = soft_i2c->slave_addr;
//	
//	SDA_H;													//����SDA�ߣ����ڿ���״̬
//	SCL_H;													//����SCL�ߣ����ڿ���״̬
//	
//	return ret_value;
//}

///*
//************************************************************
//*	�������ƣ�	vd_bsp_soft_i2c_write_byte
//*	�������ܣ�  дһ���ֽ�
//*	��ڲ�����	reg_addr���Ĵ�����ַ��byte������
//*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
//************************************************************
//*/
//vd_bool_t vd_bsp_soft_i2c_write_byte(vd_uint8_t reg_addr, vd_uint8_t byte)
//{
//	vd_soft_i2c_t*  soft_i2c_master = &vd_g_soft_i2c_master;	
//	vd_uint8_t slave_addr = 0;
//	
//	slave_addr = (soft_i2c_master->slave_addr & 0x7f) << 1;

//	/*���ʹӻ���ַ*/
//	IIC_Start();				                           //��ʼ�ź�
//	IIC_SendByte(slave_addr);			                   //�����豸��ַ(д)
//	if(IIC_WaitAck(5000)){		                           //�ȴ�Ӧ��
//		return IIC_Err;
//	}
//	/*�мĴ�����ַ*/
//	if(soft_i2c_master->reg_bit == I2C_REG_BIT_8BIT){
//		IIC_SendByte(reg_addr);		                       //���ͼĴ�����ַ
//		if(IIC_WaitAck(5000)){		                       //�ȴ�Ӧ��
//			return IIC_Err;
//		}			
//	}
//	/*��������*/
//	IIC_SendByte(byte);	                                   //��������
//	if(IIC_WaitAck(5000)){	                               //�ȴ�Ӧ��
//		return IIC_Err;
//	}
//	IIC_Stop();					                           //ֹͣ�ź�
//	
//	return IIC_OK;
//}


///*
//************************************************************
//*	�������ƣ�	vd_bsp_soft_i2c_read_byte
//*	�������ܣ�  ��һ���ֽ�
//*	��ڲ�����	reg_addr���Ĵ�����ַ��byte������
//*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
//************************************************************
//*/
//vd_bool_t vd_bsp_soft_i2c_read_byte(vd_uint8_t reg_addr, vd_uint8_t* byte)
//{
//	vd_soft_i2c_t*  soft_i2c_master = &vd_g_soft_i2c_master;	
//	vd_uint8_t slave_addr = 0;
//	
//	vd_check_return_val_error(byte == NULL, vd_false, "[ %s ]: EINVAL \r\n",__FUNCTION__ );
//	
//	slave_addr = (soft_i2c_master->slave_addr & 0x7f) << 1;		//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
//	
//	/*���ʹӻ���ַ*/
//	IIC_Start();				                           //��ʼ�ź�
//	IIC_SendByte(slave_addr);			                   //�����豸��ַ(д)
//	if(IIC_WaitAck(5000)){		                           //�ȴ�Ӧ��
//		return IIC_Err;
//	}	
//	/*�мĴ�����ַ*/
//	if(soft_i2c_master->reg_bit == I2C_REG_BIT_8BIT){
//		IIC_SendByte(reg_addr);		                       //���ͼĴ�����ַ
//		if(IIC_WaitAck(5000)){		                       //�ȴ�Ӧ��
//			return IIC_Err;
//		}			
//	}	
//	IIC_Start();				                           //�����ź�	
//	IIC_SendByte(slave_addr + 1);		                   //�����豸��ַ(��)
//	if(IIC_WaitAck(5000)){		                           //�ȴ�Ӧ��
//		return IIC_Err;
//	}
//	*byte = IIC_RecvByte();		                           //����
//	IIC_NAck();					                           //����һ����Ӧ���źţ������ȡ����
//	IIC_Stop();					                           //ֹͣ�ź�
//	
//	return IIC_OK;
//}


///*
//************************************************************
//*	�������ƣ�	vd_bsp_soft_i2c_write_bytes
//*	�������ܣ�  д����ֽ�����
//*	��ڲ�����	reg_addr���Ĵ�����ַ��buf������, len:���ݳ���
//*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
//************************************************************
//*/
//vd_bool_t vd_bsp_soft_i2c_write_bytes(vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len)
//{
//	vd_soft_i2c_t*  soft_i2c_master = &vd_g_soft_i2c_master;	
//	vd_uint8_t slave_addr = 0;
//	
//	slave_addr = (soft_i2c_master->slave_addr & 0x7f) << 1;		//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-����0-д
//	
//	/*���ʹӻ���ַ*/
//	IIC_Start();				                           //��ʼ�ź�
//	IIC_SendByte(slave_addr);			                   //�����豸��ַ(д)
//	if(IIC_WaitAck(5000)){		                           //�ȴ�Ӧ��
//		return IIC_Err;
//	}	
//	/*�мĴ�����ַ*/
//	if(soft_i2c_master->reg_bit == I2C_REG_BIT_8BIT){
//		IIC_SendByte(reg_addr);		                       //���ͼĴ�����ַ
//		if(IIC_WaitAck(5000)){		                       //�ȴ�Ӧ��
//			return IIC_Err;
//		}			
//	}
//	
//	while(len--){				//ѭ��д������
//		IIC_SendByte(*buf);		//��������
//		if(IIC_WaitAck(5000)){	//�ȴ�Ӧ��
//			return IIC_Err;
//		}
//		buf++;					//����ָ��ƫ�Ƶ���һ��	
//		//DelayUs(10);
//		IIC_Delay();
//	}
//	IIC_Stop();					//ֹͣ�ź�
//	
//	return IIC_OK;
//}


///*
//************************************************************
//*	�������ƣ�	vd_bsp_soft_i2c_read_bytes
//*	�������ܣ�  ������ֽ�����
//*	��ڲ�����	reg_addr���Ĵ�����ַ��buf������, len:���ݳ���
//*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
//************************************************************
//*/	
//vd_bool_t vd_bsp_soft_i2c_read_bytes(vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len)
//{
//	vd_soft_i2c_t*  soft_i2c_master = &vd_g_soft_i2c_master;	
//	vd_uint8_t slave_addr = 0;
//	
//	slave_addr = (soft_i2c_master->slave_addr & 0x7f) << 1;		//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-����0-д
//	
//	/*���ʹӻ���ַ*/
//	IIC_Start();				                                //��ʼ�ź�
//	IIC_SendByte(slave_addr);			                        //�����豸��ַ(д)
//	if(IIC_WaitAck(5000)){		                                //�ȴ�Ӧ��
//		return IIC_Err;
//	}	
//	/*�мĴ�����ַ*/
//	if(soft_i2c_master->reg_bit == I2C_REG_BIT_8BIT){
//		IIC_SendByte(reg_addr);		                            //���ͼĴ�����ַ
//		if(IIC_WaitAck(5000)){		                            //�ȴ�Ӧ��
//			return IIC_Err;
//		}			
//	}
//	IIC_Start();				                                //�����ź�
//	IIC_SendByte(slave_addr + 1);		                        //�����豸��ַ(��)
//	if(IIC_WaitAck(5000)){		                                //�ȴ�Ӧ��
//		return IIC_Err;
//	}	
//	while(len--){
//		*buf = IIC_RecvByte();
//		buf++;					                               //ƫ�Ƶ���һ�����ݴ洢��ַ	
//		if(len == 0){
//           IIC_NAck();			                               //���һ��������Ҫ��NOACK   
//        }else{
//          IIC_Ack();			                               //��ӦACK
//		}
//	}	
//	IIC_Stop();                                                //ֹͣ�ź�
//	
//	return IIC_OK;
//}
