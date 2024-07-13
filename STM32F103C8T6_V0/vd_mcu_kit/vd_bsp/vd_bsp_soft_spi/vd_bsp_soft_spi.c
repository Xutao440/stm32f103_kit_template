#include "vd_bsp_soft_spi.h"
#include "base_check.h"
#include "base_utils.h"
#include "log.h"

vd_soft_spi_t  master_spi;

vd_slave_spi_t register_slave_dev[SLAVE_DEV_MAX] = {0};
vd_uint8_t     register_slave_mask = 0;


/*
************************************************************
*	函数名称：	SPI_Start
*	函数功能：	起始信号
*	入口参数：	无
*	返回参数：	无
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
*	函数名称：	SPI_Stop
*	函数功能：	停止信号
*	入口参数：	无
*	返回参数：	无
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
*	函数名称：	vd_bsp_spi_send_byte
*	函数功能：	发送一个字节
*	入口参数：	byte：发送的数据
*	返回参数：	无
************************************************************
*/
vd_uint8_t vd_bsp_spi_send_byte(uint8_t byte)
{	
	vd_uint8_t spi_mode = 0, index = 0, rx_data = 0;
	spi_mode = master_spi.spi_mode;
	
	switch(spi_mode){
		case SPI_00_MODE:   //空闲SCK为低电平，SCK第一个边沿移入数据，第二个边沿移出数据
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
		case SPI_01_MODE:   //空闲SCK为低电平, SCK第一个边沿移出数据，第二个边沿移入数据     #### 接收数据异常 #####
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
		case SPI_10_MODE:   //空闲SCK为高电平，下降沿读数据，上升沿写数据                    #### 接收数据异常 #####
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
			
		case SPI_11_MODE:   //空闲SCK为高电平, 下降沿写数据，上升沿读数据                    #### 接收数据异常 #####
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
*	函数名称：	vd_bsp_soft_i2c_init
*	函数功能：	软件i2c初始化
*	入口参数：	soft_i2c：结构体
*	返回参数：	成功返回0 ， 失败返回 < 0
************************************************************
*/
vd_int32_t vd_bsp_soft_spi_init(vd_soft_spi_t*  master)
{
	vd_int32_t ret_value = VD_BSP_NOERROR;
	vd_soft_spi_t* soft_spi = NULL;
	soft_spi = &master_spi;
	
	vd_check_return_val_error(master == NULL, -VD_BSP_ERR_PARAMETER, "[ %s ]: parameter error\r\n",__FUNCTION__ );
	
	/*GPIO初始化*/
	ret_value = vd_bsp_gpio_init(master->mosi_port, VD_GPIO_MODE_OUT_OD);  //初始化mosi引脚
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: mosi init fail\r\n",__FUNCTION__ );
	ret_value = vd_bsp_gpio_init(master->sck_port, VD_GPIO_MODE_OUT_OD);   //初始化sck引脚
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: sck init fail\r\n",__FUNCTION__ );
	ret_value = vd_bsp_gpio_init(master->ss_port, VD_GPIO_MODE_OUT_OD);    //初始化ss引脚
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: ss init fail\r\n",__FUNCTION__ );
	ret_value = vd_bsp_gpio_init(master->miso_port, VD_GPIO_MODE_IPU);     //初始化miso引脚
	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: miso init fail\r\n",__FUNCTION__ );

	
	/*注册*/
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
*	函数名称：	vd_bsp_soft_i2c_init
*	函数功能：	软件i2c初始化
*	入口参数：	soft_i2c：结构体
*	返回参数：	成功返回0 ， 失败返回 < 0
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
	
	vd_bsp_gpio_init(slave_dev->ss_port, VD_GPIO_MODE_OUT_OD);    //初始化ss引脚
	
	
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
//*	函数名称：	IIC_Stop
//*	函数功能：	停止信号
//*	入口参数：	无
//*	返回参数：	无
//************************************************************
//*/
//static void IIC_Stop(void)
//{
//	SDA_L;						//拉低SDA线
//	SCL_L;						//拉低SCL先
//	IIC_Delay();		        //延时，速度控制	
//	SCL_H;						//拉高SCL线
//	SDA_H;						//拉高SDA线，当SCL线为高时，SDA线一个上升沿代表停止信号
//	IIC_Delay();
//}


///*
//************************************************************
//*	函数名称：	IIC_WaitAck
//*	函数功能：	等待ack信号
//*	入口参数：	timeout: 超时时间
//*	返回参数：	成功返回0 ，失败返回1
//************************************************************
//*/
//static vd_bool_t IIC_WaitAck(vd_uint32_t timeout)
//{	
//	SDA_H;
//	IIC_Delay();			//拉高SDA线
//	SCL_H;
//	IIC_Delay();			//拉高SCL线
//	
//	while(SDA_R){							//如果读到SDA线为1，则等待。应答信号应是0
//		if(--timeout){
//			log_info("WaitAck TimeOut\r\n");
//			IIC_Stop();						//超时未收到应答，则停止总线			
//			return IIC_Err;					//返回失败
//		}	
//		IIC_Delay();
//	}
//	SCL_L;									//拉低SCL线，以便继续收发数据
//	
//	return IIC_OK;							//返回成功
//	
//}


///*
//************************************************************
//*	函数名称：	IIC_Ack
//*	函数功能：	软件IIC产生一个应答
//*	入口参数：	无
//*	返回参数：	无
//************************************************************
//*/
//static void IIC_Ack(void)
//{
//	SCL_L;						//拉低SCL线
//	SDA_L;						//拉低SDA线
//	IIC_Delay();
//	SCL_H;						//拉高SCL线
//	IIC_Delay();
//	SCL_L;						//拉低SCL线
//}


///*
//************************************************************
//*	函数名称：	IIC_NAck
//*	函数功能：	产生一个非应答
//*	入口参数：	无
//*	返回参数：	无
//************************************************************
//*/
//static void IIC_NAck(void)
//{	
//	SCL_L;						//拉低SCL线
//	SDA_H;						//拉高SDA线
//	IIC_Delay();
//	SCL_H;						//拉高SCL线
//	IIC_Delay();
//	SCL_L;						//拉低SCL线	
//}

///*
//************************************************************
//*	函数名称：	vd_bsp_soft_i2c_write_byte
//*	函数功能：	发送一个字节
//*	入口参数：	byte：发送数据
//*	返回参数：	无
//************************************************************
//*/
//static vd_void_t IIC_SendByte(vd_uint8_t byte)
//{
//	vd_uint8_t count = 0;
//	
//    SCL_L;							//拉低时钟开始数据传输	
//    for(; count < 8; count++){		//循环8次，每次发送一个bit
//		if(byte & 0x80){		    //发送最高位
//			SDA_H;
//		}else{
//			SDA_L;
//		}
//		byte <<= 1;					//byte左移1位		
//		IIC_Delay();
//		SCL_H;
//		IIC_Delay();
//		SCL_L;
//    }
//}

///*
//************************************************************
//*	函数名称：	vd_bsp_soft_i2c_read_byte
//*	函数功能：  读一个字节
//*	入口参数：	无
//*	返回参数：	接收到的字节数据
//************************************************************
//*/
//static vd_uint8_t IIC_RecvByte(vd_void_t)
//{
//	vd_uint8_t count = 0, receive = 0;
//	
//	SDA_H;							//拉高SDA线，开漏状态下，需线拉高以便读取数据
//    for(; count < 8; count++ ){		//循环8次，每次发送一个bit
//		SCL_L;
//		IIC_Delay();
//		SCL_H;	
//        receive <<= 1;				//左移一位	
//        if(SDA_R){					//如果SDA线为1，则receive变量自增，每次自增都是对bit0的+1，然后下一次循环会先左移一次
//			receive++;
//		}
//		IIC_Delay();
//    }
//	
//    return receive;
//}


///*
//************************************************************
//*	函数名称：	vd_bsp_soft_i2c_init
//*	函数功能：	软件i2c初始化
//*	入口参数：	soft_i2c：结构体
//*	返回参数：	成功返回0 ， 失败返回 < 0
//************************************************************
//*/
//vd_int32_t vd_bsp_soft_i2c_init(vd_soft_i2c_t* soft_i2c)
//{
//	vd_int32_t ret_value = VD_BSP_NOERROR;
//	
//	vd_check_return_val_error(soft_i2c == NULL, -VD_BSP_ERR_PORT_INVALID, "[ %s ]: EINVAL \r\n",__FUNCTION__ );
//	
//	/*GPIO初始化*/
//	ret_value = vd_bsp_gpio_init(soft_i2c->scl_prot, VD_GPIO_MODE_OUT_OD);  //初始化scl引脚
//	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: gpio scl init fail\r\n",__FUNCTION__ );
//	ret_value = vd_bsp_gpio_init(soft_i2c->sda_prot, VD_GPIO_MODE_OUT_OD);  //初始化scl引脚
//	vd_check_return_val_error(ret_value < 0, -VD_BSP_ERR_SDK, "[ %s ]: gpio sda init fail\r\n",__FUNCTION__ );
//	
//	/*注册*/
//	vd_g_soft_i2c_master.scl_prot = soft_i2c->scl_prot;
//	vd_g_soft_i2c_master.sda_prot = soft_i2c->sda_prot;
//	vd_g_soft_i2c_master.reg_bit = soft_i2c->reg_bit;
//	vd_g_soft_i2c_master.speed = soft_i2c->speed;
//	vd_g_soft_i2c_master.slave_addr = soft_i2c->slave_addr;
//	
//	SDA_H;													//拉高SDA线，处于空闲状态
//	SCL_H;													//拉高SCL线，处于空闲状态
//	
//	return ret_value;
//}

///*
//************************************************************
//*	函数名称：	vd_bsp_soft_i2c_write_byte
//*	函数功能：  写一个字节
//*	入口参数：	reg_addr：寄存器地址，byte：数据
//*	返回参数：	成功返回0 ，失败返回1
//************************************************************
//*/
//vd_bool_t vd_bsp_soft_i2c_write_byte(vd_uint8_t reg_addr, vd_uint8_t byte)
//{
//	vd_soft_i2c_t*  soft_i2c_master = &vd_g_soft_i2c_master;	
//	vd_uint8_t slave_addr = 0;
//	
//	slave_addr = (soft_i2c_master->slave_addr & 0x7f) << 1;

//	/*发送从机地址*/
//	IIC_Start();				                           //起始信号
//	IIC_SendByte(slave_addr);			                   //发送设备地址(写)
//	if(IIC_WaitAck(5000)){		                           //等待应答
//		return IIC_Err;
//	}
//	/*有寄存器地址*/
//	if(soft_i2c_master->reg_bit == I2C_REG_BIT_8BIT){
//		IIC_SendByte(reg_addr);		                       //发送寄存器地址
//		if(IIC_WaitAck(5000)){		                       //等待应答
//			return IIC_Err;
//		}			
//	}
//	/*发送数据*/
//	IIC_SendByte(byte);	                                   //发送数据
//	if(IIC_WaitAck(5000)){	                               //等待应答
//		return IIC_Err;
//	}
//	IIC_Stop();					                           //停止信号
//	
//	return IIC_OK;
//}


///*
//************************************************************
//*	函数名称：	vd_bsp_soft_i2c_read_byte
//*	函数功能：  读一个字节
//*	入口参数：	reg_addr：寄存器地址，byte：数据
//*	返回参数：	成功返回0 ，失败返回1
//************************************************************
//*/
//vd_bool_t vd_bsp_soft_i2c_read_byte(vd_uint8_t reg_addr, vd_uint8_t* byte)
//{
//	vd_soft_i2c_t*  soft_i2c_master = &vd_g_soft_i2c_master;	
//	vd_uint8_t slave_addr = 0;
//	
//	vd_check_return_val_error(byte == NULL, vd_false, "[ %s ]: EINVAL \r\n",__FUNCTION__ );
//	
//	slave_addr = (soft_i2c_master->slave_addr & 0x7f) << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
//	
//	/*发送从机地址*/
//	IIC_Start();				                           //起始信号
//	IIC_SendByte(slave_addr);			                   //发送设备地址(写)
//	if(IIC_WaitAck(5000)){		                           //等待应答
//		return IIC_Err;
//	}	
//	/*有寄存器地址*/
//	if(soft_i2c_master->reg_bit == I2C_REG_BIT_8BIT){
//		IIC_SendByte(reg_addr);		                       //发送寄存器地址
//		if(IIC_WaitAck(5000)){		                       //等待应答
//			return IIC_Err;
//		}			
//	}	
//	IIC_Start();				                           //重启信号	
//	IIC_SendByte(slave_addr + 1);		                   //发送设备地址(读)
//	if(IIC_WaitAck(5000)){		                           //等待应答
//		return IIC_Err;
//	}
//	*byte = IIC_RecvByte();		                           //接收
//	IIC_NAck();					                           //产生一个非应答信号，代表读取接收
//	IIC_Stop();					                           //停止信号
//	
//	return IIC_OK;
//}


///*
//************************************************************
//*	函数名称：	vd_bsp_soft_i2c_write_bytes
//*	函数功能：  写多个字节数据
//*	入口参数：	reg_addr：寄存器地址，buf：数据, len:数据长度
//*	返回参数：	成功返回0 ，失败返回1
//************************************************************
//*/
//vd_bool_t vd_bsp_soft_i2c_write_bytes(vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len)
//{
//	vd_soft_i2c_t*  soft_i2c_master = &vd_g_soft_i2c_master;	
//	vd_uint8_t slave_addr = 0;
//	
//	slave_addr = (soft_i2c_master->slave_addr & 0x7f) << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读，0-写
//	
//	/*发送从机地址*/
//	IIC_Start();				                           //起始信号
//	IIC_SendByte(slave_addr);			                   //发送设备地址(写)
//	if(IIC_WaitAck(5000)){		                           //等待应答
//		return IIC_Err;
//	}	
//	/*有寄存器地址*/
//	if(soft_i2c_master->reg_bit == I2C_REG_BIT_8BIT){
//		IIC_SendByte(reg_addr);		                       //发送寄存器地址
//		if(IIC_WaitAck(5000)){		                       //等待应答
//			return IIC_Err;
//		}			
//	}
//	
//	while(len--){				//循环写入数据
//		IIC_SendByte(*buf);		//发送数据
//		if(IIC_WaitAck(5000)){	//等待应答
//			return IIC_Err;
//		}
//		buf++;					//数据指针偏移到下一个	
//		//DelayUs(10);
//		IIC_Delay();
//	}
//	IIC_Stop();					//停止信号
//	
//	return IIC_OK;
//}


///*
//************************************************************
//*	函数名称：	vd_bsp_soft_i2c_read_bytes
//*	函数功能：  读多个字节数据
//*	入口参数：	reg_addr：寄存器地址，buf：数据, len:数据长度
//*	返回参数：	成功返回0 ，失败返回1
//************************************************************
//*/	
//vd_bool_t vd_bsp_soft_i2c_read_bytes(vd_uint8_t reg_addr, vd_uint8_t *buf, vd_uint8_t len)
//{
//	vd_soft_i2c_t*  soft_i2c_master = &vd_g_soft_i2c_master;	
//	vd_uint8_t slave_addr = 0;
//	
//	slave_addr = (soft_i2c_master->slave_addr & 0x7f) << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读，0-写
//	
//	/*发送从机地址*/
//	IIC_Start();				                                //起始信号
//	IIC_SendByte(slave_addr);			                        //发送设备地址(写)
//	if(IIC_WaitAck(5000)){		                                //等待应答
//		return IIC_Err;
//	}	
//	/*有寄存器地址*/
//	if(soft_i2c_master->reg_bit == I2C_REG_BIT_8BIT){
//		IIC_SendByte(reg_addr);		                            //发送寄存器地址
//		if(IIC_WaitAck(5000)){		                            //等待应答
//			return IIC_Err;
//		}			
//	}
//	IIC_Start();				                                //重启信号
//	IIC_SendByte(slave_addr + 1);		                        //发送设备地址(读)
//	if(IIC_WaitAck(5000)){		                                //等待应答
//		return IIC_Err;
//	}	
//	while(len--){
//		*buf = IIC_RecvByte();
//		buf++;					                               //偏移到下一个数据存储地址	
//		if(len == 0){
//           IIC_NAck();			                               //最后一个数据需要回NOACK   
//        }else{
//          IIC_Ack();			                               //回应ACK
//		}
//	}	
//	IIC_Stop();                                                //停止信号
//	
//	return IIC_OK;
//}
