#include "vd_bsp_soft_i2c.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"

vd_master_i2c_t  master_i2c;
/*
************************************************************
*	�������ƣ�	IIC_Start
*	�������ܣ�	��ʼ�ź�
*	��ڲ�����	��
*	���ز�����	��
************************************************************
*/
static void IIC_Start(void)
{			
	SDA_H;						//����SDA��
	SCL_H;						//����SCL��
	IIC_Delay();		        //��ʱ���ٶȿ���	
	SDA_L;						//��SCL��Ϊ��ʱ��SDA��һ���½��ش���ʼ�ź�
	IIC_Delay();		        //��ʱ���ٶȿ���
	SCL_L;						//ǯסSCL�ߣ��Ա㷢������
}


/*
************************************************************
*	�������ƣ�	IIC_Stop
*	�������ܣ�	ֹͣ�ź�
*	��ڲ�����	��
*	���ز�����	��
************************************************************
*/
static void IIC_Stop(void)
{		
	SDA_L;						//����SDA��
	SCL_L;						//����SCL��
	IIC_Delay();		        //��ʱ���ٶȿ���	
	SCL_H;						//����SCL��
	SDA_H;						//����SDA�ߣ���SCL��Ϊ��ʱ��SDA��һ�������ش���ֹͣ�ź�
	IIC_Delay();
}


/*
************************************************************
*	�������ƣ�	IIC_WaitAck
*	�������ܣ�	�ȴ�ack�ź�
*	��ڲ�����	timeout: ��ʱʱ��
*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
************************************************************
*/
static vd_bool_t IIC_WaitAck(vd_uint32_t timeout)
{
	SDA_H;
	IIC_Delay();			//����SDA��
	SCL_H;
	IIC_Delay();			//����SCL��
	
	while(SDA_R){							//�������SDA��Ϊ1����ȴ���Ӧ���ź�Ӧ��0
		if(--timeout){
			log_info("WaitAck TimeOut\r\n");
			IIC_Stop();						//��ʱδ�յ�Ӧ����ֹͣ����			
			return IIC_Err;					//����ʧ��
		}	
		IIC_Delay();
	}
	SCL_L;									//����SCL�ߣ��Ա�����շ�����
	
	return IIC_OK;							//���سɹ�
	
}


/*
************************************************************
*	�������ƣ�	IIC_Ack
*	�������ܣ�	���IIC����һ��Ӧ��
*	��ڲ�����	��
*	���ز�����	��
************************************************************
*/
static void IIC_Ack(void)
{	
	SCL_L;						//����SCL��
	SDA_L;						//����SDA��
	IIC_Delay();
	SCL_H;						//����SCL��
	IIC_Delay();
	SCL_L;						//����SCL��
}


/*
************************************************************
*	�������ƣ�	IIC_NAck
*	�������ܣ�	����һ����Ӧ��
*	��ڲ�����	��
*	���ز�����	��
************************************************************
*/
static void IIC_NAck(void)
{	
	SCL_L;						//����SCL��
	SDA_H;						//����SDA��
	IIC_Delay();
	SCL_H;						//����SCL��
	IIC_Delay();
	SCL_L;						//����SCL��	
}

/*
************************************************************
*	�������ƣ�	vd_bsp_soft_i2c_write_byte
*	�������ܣ�	����һ���ֽ�
*	��ڲ�����	byte����������
*	���ز�����	��
************************************************************
*/
static vd_void_t IIC_SendByte(vd_uint8_t byte)
{
	vd_uint8_t count = 0;
	
    SCL_L;		                //����ʱ�ӿ�ʼ���ݴ���	
    for(; count < 8; count++){	//ѭ��8�Σ�ÿ�η���һ��bit
		if(byte & 0x80){		//�������λ
			SDA_H;
		}else{
			SDA_L;
		}
		byte <<= 1;				//byte����1λ		
		IIC_Delay();
		SCL_H;
		IIC_Delay();
		SCL_L;
    }
}

/*
************************************************************
*	�������ƣ�	vd_bsp_soft_i2c_read_byte
*	�������ܣ�  ��һ���ֽ�
*	��ڲ�����	��
*	���ز�����	���յ����ֽ�����
************************************************************
*/
static vd_uint8_t IIC_RecvByte(void)
{
	vd_uint8_t count = 0, receive = 0;
	
	SDA_H;				        //����SDA�ߣ���©״̬�£����������Ա��ȡ����
    for(; count < 8; count++ ){ //ѭ��8�Σ�ÿ�η���һ��bit
		SCL_L;
		IIC_Delay();
		SCL_H;	
        receive <<= 1;		    //����һλ	
        if(SDA_R){		        //���SDA��Ϊ1����receive����������ÿ���������Ƕ�bit0��+1��Ȼ����һ��ѭ����������һ��
			receive++;
		}
		IIC_Delay();
    }
	
    return receive;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_soft_i2c_init
*	�������ܣ�	���i2c��ʼ��
*	��ڲ�����	soft_i2c���ṹ��
*	���ز�����	�ɹ�����i2c_obj����id �� ʧ�ܷ��� < 0
************************************************************
*/
vd_int32_t vd_bsp_soft_i2c_init(vd_master_i2c_t* soft_i2c)
{
	vd_int32_t ret_value = -VD_BSP_ERR_PARAMETER;
	vd_uint8_t index = 0;
	
	/*GPIO��ʼ��*/
	ret_value = vd_bsp_gpio_init(soft_i2c->scl_prot, VD_GPIO_MODE_OUT_OD);  //��ʼ��scl����
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: gpio scl init fail\r\n",__FUNCTION__ );
	ret_value = vd_bsp_gpio_init(soft_i2c->sda_prot, VD_GPIO_MODE_OUT_OD);  //��ʼ��scl����
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: gpio sda init fail\r\n",__FUNCTION__ );
	
	/*ע��*/
	master_i2c.reg_bit = soft_i2c->reg_bit;
	master_i2c.scl_prot = soft_i2c->scl_prot;
	master_i2c.sda_prot = soft_i2c->sda_prot;
	master_i2c.speed = soft_i2c->speed;
		
	SDA_H;							//����SDA�ߣ����ڿ���״̬
	SCL_H;							//����SCL�ߣ����ڿ���״̬
	
	return ret_value;
}

/*
************************************************************
*	�������ƣ�	vd_bsp_soft_i2c_write_byte
*	�������ܣ�  дһ���ֽ�
*	��ڲ�����	reg_addr���Ĵ�����ַ��byte������
*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
************************************************************
*/
vd_bool_t vd_bsp_soft_i2c_write_byte(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t byte)
{

	slave_addr = (slave_addr & 0x7f) << 1;

	/*���ʹӻ���ַ*/
	IIC_Start();				                           //��ʼ�ź�
	IIC_SendByte(slave_addr);			                   //�����豸��ַ(д)
	if(IIC_WaitAck(5000)){		                           //�ȴ�Ӧ��
		return IIC_Err;
	}
	/*�мĴ�����ַ*/
	if(master_i2c.reg_bit == I2C_REG_BIT_8BIT){
		IIC_SendByte(reg_addr);		                       //���ͼĴ�����ַ
		if(IIC_WaitAck(5000)){		                       //�ȴ�Ӧ��
			return IIC_Err;
		}			
	}
	/*��������*/
	IIC_SendByte(byte);	                                   //��������
	if(IIC_WaitAck(5000)){	                               //�ȴ�Ӧ��
		return IIC_Err;
	}
	IIC_Stop();					                           //ֹͣ�ź�
	
	return IIC_OK;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_soft_i2c_read_byte
*	�������ܣ�  ��һ���ֽ�
*	��ڲ�����	reg_addr���Ĵ�����ַ��byte������
*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
************************************************************
*/
vd_bool_t vd_bsp_soft_i2c_read_byte(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t* byte)
{		
	slave_addr = (slave_addr & 0x7f) << 1;		//IIC��ַ��7bit��������Ҫ����1λ��bit0��1-��	0-д
	
	/*���ʹӻ���ַ*/
	IIC_Start();				                           //��ʼ�ź�
	IIC_SendByte(slave_addr);			                   //�����豸��ַ(д)
	if(IIC_WaitAck(5000)){		                           //�ȴ�Ӧ��
		return IIC_Err;
	}	
	/*�мĴ�����ַ*/
	if(master_i2c.reg_bit == I2C_REG_BIT_8BIT){
		IIC_SendByte(reg_addr);		                       //���ͼĴ�����ַ
		if(IIC_WaitAck(5000)){		                       //�ȴ�Ӧ��
			return IIC_Err;
		}			
	}	
	IIC_Start();				                           //�����ź�	
	IIC_SendByte(slave_addr + 1);		                   //�����豸��ַ(��)
	if(IIC_WaitAck(5000)){		                           //�ȴ�Ӧ��
		return IIC_Err;
	}
	*byte = IIC_RecvByte();		                           //����
	IIC_NAck();					                           //����һ����Ӧ���źţ������ȡ����
	IIC_Stop();					                           //ֹͣ�ź�
	
	return IIC_OK;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_soft_i2c_write_bytes
*	�������ܣ�  д����ֽ�����
*	��ڲ�����	reg_addr���Ĵ�����ַ��buf������, len:���ݳ���
*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
************************************************************
*/
vd_bool_t vd_bsp_soft_i2c_write_bytes(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len)
{	
	slave_addr = (slave_addr & 0x7f) << 1;		   //IIC��ַ��7bit��������Ҫ����1λ��bit0��1-����0-д
	
	/*���ʹӻ���ַ*/
	IIC_Start();				                    //��ʼ�ź�
	IIC_SendByte(slave_addr);			            //�����豸��ַ(д)
	if(IIC_WaitAck(5000)){		                    //�ȴ�Ӧ��
		return IIC_Err;
	}	
	/*�мĴ�����ַ*/
	if(master_i2c.reg_bit == I2C_REG_BIT_8BIT){
		IIC_SendByte(reg_addr);		                //���ͼĴ�����ַ
		if(IIC_WaitAck(5000)){		                //�ȴ�Ӧ��
			return IIC_Err;
		}			
	}
	
	while(len--){				                    //ѭ��д������
		IIC_SendByte(*buf);		                    //��������
		if(IIC_WaitAck(5000)){	                    //�ȴ�Ӧ��
			return IIC_Err;
		}
		buf++;					                    //����ָ��ƫ�Ƶ���һ��	
		IIC_Delay();
	}
	IIC_Stop();					                    //ֹͣ�ź�
	
	return IIC_OK;
}


/*
************************************************************
*	�������ƣ�	vd_bsp_soft_i2c_read_bytes
*	�������ܣ�  ������ֽ�����
*	��ڲ�����	reg_addr���Ĵ�����ַ��buf������, len:���ݳ���
*	���ز�����	�ɹ�����0 ��ʧ�ܷ���1
************************************************************
*/	
vd_bool_t vd_bsp_soft_i2c_read_bytes(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len)
{	
	
	slave_addr = (slave_addr & 0x7f) << 1;		     //IIC��ַ��7bit��������Ҫ����1λ��bit0��1-����0-д
	
	/*���ʹӻ���ַ*/
	IIC_Start();				                     //��ʼ�ź�
	IIC_SendByte(slave_addr);			             //�����豸��ַ(д)
	if(IIC_WaitAck(5000)){		                     //�ȴ�Ӧ��
		return IIC_Err;
	}	
	/*�мĴ�����ַ*/
	if(master_i2c.reg_bit == I2C_REG_BIT_8BIT){
		IIC_SendByte(reg_addr);		                  //���ͼĴ�����ַ
		if(IIC_WaitAck(5000)){		                  //�ȴ�Ӧ��
			return IIC_Err;
		}			
	}
	IIC_Start();				                      //�����ź�
	IIC_SendByte(slave_addr + 1);		              //�����豸��ַ(��)
	if(IIC_WaitAck(5000)){		                      //�ȴ�Ӧ��
		return IIC_Err;
	}	
	while(len--){
		*buf = IIC_RecvByte();
		buf++;					                      //ƫ�Ƶ���һ�����ݴ洢��ַ	
		if(len == 0){
           IIC_NAck();			                      //���һ��������Ҫ��NOACK   
        }else{
          IIC_Ack();			                      //��ӦACK
		}
	}	
	IIC_Stop();                                      //ֹͣ�ź�
	
	return IIC_OK;
}
