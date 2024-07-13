#include "vd_bsp_soft_i2c.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"

vd_master_i2c_t  master_i2c;
/*
************************************************************
*	函数名称：	IIC_Start
*	函数功能：	起始信号
*	入口参数：	无
*	返回参数：	无
************************************************************
*/
static void IIC_Start(void)
{			
	SDA_H;						//拉高SDA线
	SCL_H;						//拉高SCL线
	IIC_Delay();		        //延时，速度控制	
	SDA_L;						//当SCL线为高时，SDA线一个下降沿代表开始信号
	IIC_Delay();		        //延时，速度控制
	SCL_L;						//钳住SCL线，以便发送数据
}


/*
************************************************************
*	函数名称：	IIC_Stop
*	函数功能：	停止信号
*	入口参数：	无
*	返回参数：	无
************************************************************
*/
static void IIC_Stop(void)
{		
	SDA_L;						//拉低SDA线
	SCL_L;						//拉低SCL先
	IIC_Delay();		        //延时，速度控制	
	SCL_H;						//拉高SCL线
	SDA_H;						//拉高SDA线，当SCL线为高时，SDA线一个上升沿代表停止信号
	IIC_Delay();
}


/*
************************************************************
*	函数名称：	IIC_WaitAck
*	函数功能：	等待ack信号
*	入口参数：	timeout: 超时时间
*	返回参数：	成功返回0 ，失败返回1
************************************************************
*/
static vd_bool_t IIC_WaitAck(vd_uint32_t timeout)
{
	SDA_H;
	IIC_Delay();			//拉高SDA线
	SCL_H;
	IIC_Delay();			//拉高SCL线
	
	while(SDA_R){							//如果读到SDA线为1，则等待。应答信号应是0
		if(--timeout){
			log_info("WaitAck TimeOut\r\n");
			IIC_Stop();						//超时未收到应答，则停止总线			
			return IIC_Err;					//返回失败
		}	
		IIC_Delay();
	}
	SCL_L;									//拉低SCL线，以便继续收发数据
	
	return IIC_OK;							//返回成功
	
}


/*
************************************************************
*	函数名称：	IIC_Ack
*	函数功能：	软件IIC产生一个应答
*	入口参数：	无
*	返回参数：	无
************************************************************
*/
static void IIC_Ack(void)
{	
	SCL_L;						//拉低SCL线
	SDA_L;						//拉低SDA线
	IIC_Delay();
	SCL_H;						//拉高SCL线
	IIC_Delay();
	SCL_L;						//拉低SCL线
}


/*
************************************************************
*	函数名称：	IIC_NAck
*	函数功能：	产生一个非应答
*	入口参数：	无
*	返回参数：	无
************************************************************
*/
static void IIC_NAck(void)
{	
	SCL_L;						//拉低SCL线
	SDA_H;						//拉高SDA线
	IIC_Delay();
	SCL_H;						//拉高SCL线
	IIC_Delay();
	SCL_L;						//拉低SCL线	
}

/*
************************************************************
*	函数名称：	vd_bsp_soft_i2c_write_byte
*	函数功能：	发送一个字节
*	入口参数：	byte：发送数据
*	返回参数：	无
************************************************************
*/
static vd_void_t IIC_SendByte(vd_uint8_t byte)
{
	vd_uint8_t count = 0;
	
    SCL_L;		                //拉低时钟开始数据传输	
    for(; count < 8; count++){	//循环8次，每次发送一个bit
		if(byte & 0x80){		//发送最高位
			SDA_H;
		}else{
			SDA_L;
		}
		byte <<= 1;				//byte左移1位		
		IIC_Delay();
		SCL_H;
		IIC_Delay();
		SCL_L;
    }
}

/*
************************************************************
*	函数名称：	vd_bsp_soft_i2c_read_byte
*	函数功能：  读一个字节
*	入口参数：	无
*	返回参数：	接收到的字节数据
************************************************************
*/
static vd_uint8_t IIC_RecvByte(void)
{
	vd_uint8_t count = 0, receive = 0;
	
	SDA_H;				        //拉高SDA线，开漏状态下，需线拉高以便读取数据
    for(; count < 8; count++ ){ //循环8次，每次发送一个bit
		SCL_L;
		IIC_Delay();
		SCL_H;	
        receive <<= 1;		    //左移一位	
        if(SDA_R){		        //如果SDA线为1，则receive变量自增，每次自增都是对bit0的+1，然后下一次循环会先左移一次
			receive++;
		}
		IIC_Delay();
    }
	
    return receive;
}


/*
************************************************************
*	函数名称：	vd_bsp_soft_i2c_init
*	函数功能：	软件i2c初始化
*	入口参数：	soft_i2c：结构体
*	返回参数：	成功返回i2c_obj对象id ， 失败返回 < 0
************************************************************
*/
vd_int32_t vd_bsp_soft_i2c_init(vd_master_i2c_t* soft_i2c)
{
	vd_int32_t ret_value = -VD_BSP_ERR_PARAMETER;
	vd_uint8_t index = 0;
	
	/*GPIO初始化*/
	ret_value = vd_bsp_gpio_init(soft_i2c->scl_prot, VD_GPIO_MODE_OUT_OD);  //初始化scl引脚
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: gpio scl init fail\r\n",__FUNCTION__ );
	ret_value = vd_bsp_gpio_init(soft_i2c->sda_prot, VD_GPIO_MODE_OUT_OD);  //初始化scl引脚
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: gpio sda init fail\r\n",__FUNCTION__ );
	
	/*注册*/
	master_i2c.reg_bit = soft_i2c->reg_bit;
	master_i2c.scl_prot = soft_i2c->scl_prot;
	master_i2c.sda_prot = soft_i2c->sda_prot;
	master_i2c.speed = soft_i2c->speed;
		
	SDA_H;							//拉高SDA线，处于空闲状态
	SCL_H;							//拉高SCL线，处于空闲状态
	
	return ret_value;
}

/*
************************************************************
*	函数名称：	vd_bsp_soft_i2c_write_byte
*	函数功能：  写一个字节
*	入口参数：	reg_addr：寄存器地址，byte：数据
*	返回参数：	成功返回0 ，失败返回1
************************************************************
*/
vd_bool_t vd_bsp_soft_i2c_write_byte(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t byte)
{

	slave_addr = (slave_addr & 0x7f) << 1;

	/*发送从机地址*/
	IIC_Start();				                           //起始信号
	IIC_SendByte(slave_addr);			                   //发送设备地址(写)
	if(IIC_WaitAck(5000)){		                           //等待应答
		return IIC_Err;
	}
	/*有寄存器地址*/
	if(master_i2c.reg_bit == I2C_REG_BIT_8BIT){
		IIC_SendByte(reg_addr);		                       //发送寄存器地址
		if(IIC_WaitAck(5000)){		                       //等待应答
			return IIC_Err;
		}			
	}
	/*发送数据*/
	IIC_SendByte(byte);	                                   //发送数据
	if(IIC_WaitAck(5000)){	                               //等待应答
		return IIC_Err;
	}
	IIC_Stop();					                           //停止信号
	
	return IIC_OK;
}


/*
************************************************************
*	函数名称：	vd_bsp_soft_i2c_read_byte
*	函数功能：  读一个字节
*	入口参数：	reg_addr：寄存器地址，byte：数据
*	返回参数：	成功返回0 ，失败返回1
************************************************************
*/
vd_bool_t vd_bsp_soft_i2c_read_byte(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t* byte)
{		
	slave_addr = (slave_addr & 0x7f) << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
	
	/*发送从机地址*/
	IIC_Start();				                           //起始信号
	IIC_SendByte(slave_addr);			                   //发送设备地址(写)
	if(IIC_WaitAck(5000)){		                           //等待应答
		return IIC_Err;
	}	
	/*有寄存器地址*/
	if(master_i2c.reg_bit == I2C_REG_BIT_8BIT){
		IIC_SendByte(reg_addr);		                       //发送寄存器地址
		if(IIC_WaitAck(5000)){		                       //等待应答
			return IIC_Err;
		}			
	}	
	IIC_Start();				                           //重启信号	
	IIC_SendByte(slave_addr + 1);		                   //发送设备地址(读)
	if(IIC_WaitAck(5000)){		                           //等待应答
		return IIC_Err;
	}
	*byte = IIC_RecvByte();		                           //接收
	IIC_NAck();					                           //产生一个非应答信号，代表读取接收
	IIC_Stop();					                           //停止信号
	
	return IIC_OK;
}


/*
************************************************************
*	函数名称：	vd_bsp_soft_i2c_write_bytes
*	函数功能：  写多个字节数据
*	入口参数：	reg_addr：寄存器地址，buf：数据, len:数据长度
*	返回参数：	成功返回0 ，失败返回1
************************************************************
*/
vd_bool_t vd_bsp_soft_i2c_write_bytes(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len)
{	
	slave_addr = (slave_addr & 0x7f) << 1;		   //IIC地址是7bit，这里需要左移1位，bit0：1-读，0-写
	
	/*发送从机地址*/
	IIC_Start();				                    //起始信号
	IIC_SendByte(slave_addr);			            //发送设备地址(写)
	if(IIC_WaitAck(5000)){		                    //等待应答
		return IIC_Err;
	}	
	/*有寄存器地址*/
	if(master_i2c.reg_bit == I2C_REG_BIT_8BIT){
		IIC_SendByte(reg_addr);		                //发送寄存器地址
		if(IIC_WaitAck(5000)){		                //等待应答
			return IIC_Err;
		}			
	}
	
	while(len--){				                    //循环写入数据
		IIC_SendByte(*buf);		                    //发送数据
		if(IIC_WaitAck(5000)){	                    //等待应答
			return IIC_Err;
		}
		buf++;					                    //数据指针偏移到下一个	
		IIC_Delay();
	}
	IIC_Stop();					                    //停止信号
	
	return IIC_OK;
}


/*
************************************************************
*	函数名称：	vd_bsp_soft_i2c_read_bytes
*	函数功能：  读多个字节数据
*	入口参数：	reg_addr：寄存器地址，buf：数据, len:数据长度
*	返回参数：	成功返回0 ，失败返回1
************************************************************
*/	
vd_bool_t vd_bsp_soft_i2c_read_bytes(vd_uint8_t slave_addr, vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len)
{	
	
	slave_addr = (slave_addr & 0x7f) << 1;		     //IIC地址是7bit，这里需要左移1位，bit0：1-读，0-写
	
	/*发送从机地址*/
	IIC_Start();				                     //起始信号
	IIC_SendByte(slave_addr);			             //发送设备地址(写)
	if(IIC_WaitAck(5000)){		                     //等待应答
		return IIC_Err;
	}	
	/*有寄存器地址*/
	if(master_i2c.reg_bit == I2C_REG_BIT_8BIT){
		IIC_SendByte(reg_addr);		                  //发送寄存器地址
		if(IIC_WaitAck(5000)){		                  //等待应答
			return IIC_Err;
		}			
	}
	IIC_Start();				                      //重启信号
	IIC_SendByte(slave_addr + 1);		              //发送设备地址(读)
	if(IIC_WaitAck(5000)){		                      //等待应答
		return IIC_Err;
	}	
	while(len--){
		*buf = IIC_RecvByte();
		buf++;					                      //偏移到下一个数据存储地址	
		if(len == 0){
           IIC_NAck();			                      //最后一个数据需要回NOACK   
        }else{
          IIC_Ack();			                      //回应ACK
		}
	}	
	IIC_Stop();                                      //停止信号
	
	return IIC_OK;
}
