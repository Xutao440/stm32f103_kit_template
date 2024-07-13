/*
 * Project: mcu-kit
 * Module: iservice2.1单片机版
 * File: iservice.c
 * Created Date: 2022-03-24 11:49:41
 * Author: meizhen
 * Description: description
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
#include "project_config.h"
#if (ISERVICE_EN == ON)
#include "iservice.h"
#include "protocol_format.h"
#include "protocol_driver.h"
#include "vd_display_lib.h"



/* ======================================================================================
 * macros
 */
//#define READ_TIMEOUT 5000  // ms
/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */
 
/* iservice 驱动对象（端口--协议格式映射） */
typedef struct {
	vd_bsp_port_e 								port_id;					//端口号
	read_method_e									read_method;			//数据读取方式
	protocol_format_e 						format_id;				//格式id
	vd_uint64_t										receive_timeout;	//数据接收超时时间
	
	protocol_driver_t							driver;						//协议驱动
	cmd_queue_t       						rx_queue;  				//用于接收数据的循环队列
	vd_uint8_t										queue_buf[200];		//队列缓存
	vd_uint64_t 									pre_timestamp;		//用于记录等待匹配一帧完整数据的起始时间戳
}iservice_drv_t;
 
static vd_uint32_t iservice_deformat_read(iservice_drv_t *iservice, vd_uint8_t *buf);

static vd_uint32_t iservice_check_a_frame(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len, vd_uint8_t *frame_buf, vd_uint32_t *frame_len);


/* ======================================================================================
 * globals
 */
 
/**
 * @brief :协议格式映射表
 *
 */
static iservice_drv_t g_iservice_port_format_map[] = ISERVICE_PORT_FORMAT_MAP;
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

iservice_receive_callback_t g_recvive_handler = vd_null;
/* ======================================================================================
 * implementation
 */

/**
 * @brief : 主动push数据到接收循环队列。
 *
 * @param port_id:对应的端口号
 * @param data:
 * @param data_len
 * @return vd_int8_t
 */
 vd_void_t iservice_push_queue(vd_bsp_port_e port_id, vd_uint8_t* pbuf, vd_uint16_t len, vd_void_t *arg) 
{
	vd_uint8_t i;
	
	for (i = 0; i < vd_array_size(g_iservice_port_format_map); i++){//将数据push到对应端口的队列中
		if(port_id == g_iservice_port_format_map[i].port_id){
			vd_queue_push(&g_iservice_port_format_map[i].rx_queue, pbuf, len);
		}
	}
}

/**
 * @brief : 去格式化读，从端口读取一帧数据，并去格式（去掉帧头帧尾长度校验和），返回payload
 *
 * @param iservice		iservice驱动对象
 * @param frame_buf		有效数据缓存
 * @return vd_int32_t 负值：失败
											非负值：有效数据长度
 */
static vd_uint32_t iservice_deformat_read(iservice_drv_t *iservice, vd_uint8_t *frame_buf) 
{
	vd_uint32_t  pop_size   = 0;
	vd_uint32_t read_count = 0, frame_len = 0;
	vd_uint8_t  read_data[PROTOCOL_FRAME_SIZE_MAX * 2];
	protocol_format_t* format = vd_null;

//	static vd_uint64_t  pre_timestamp = 0;
//	static vd_bool_t is_wait = vd_false;

//	log_debug("=====================in func: %s=====================\n", __func__);
	vd_check_return_val_error(iservice == vd_null || frame_buf == vd_null, -1, "iservice or buffer null");

	memset(read_data, 0, sizeof(read_data));

	// 1,轮询的方式从底层硬件端口读取数据，并缓存到队列
//	log_debug(">>>1.read data\n");
	if (iservice->read_method == READ_METHOD_POLL) {
		if (iservice->driver.d_read != vd_null) {
			if ((read_count = iservice->driver.d_read(iservice->port_id, read_data, sizeof(read_data)))) {
				log_array("push_data", read_data, read_count);
				log_error("vd_queue_push=%d\r\n",read_count);
				vd_queue_push(&iservice->rx_queue, read_data, read_count);
				memset(read_data, 0, read_count);
			}
		} else {
				log_error("format driver.d_read vd_null");
		}
	}

	// 2.获取协议格式
//	log_debug(">>>2.get format\n");
	format = protocol_format_get_by_id(iservice->format_id);
	vd_check_return_val_error(format == vd_null, -1, "format_id=%d error\r\n", iservice->format_id);
	
	
	// 3.从循环队列获取所有数据，并判断一帧数据
//	log_debug(">>>3.data pop\n");
	while(1){// 持续出队
		read_count = vd_queue_pull_notout(&iservice->rx_queue, read_data, sizeof(read_data));
		if(read_count == 0){//队列无数据
			break;
		}
		
//		log_array("queue read data", read_data, read_count);
		
		pop_size = iservice_check_a_frame(format, read_data, read_count, frame_buf, &frame_len);//检查数据是否存在一帧完整数据，将完整数据填入frame_buf，返回需要出队字节数
		
		// log_debug("pop_size=%d\r\n", pop_size);
		if(pop_size == 0){// 此处pop_size=0表示没收全一帧数据而等待，此处需设置等待超时
			
			frame_len = 0;
			
			if(iservice->pre_timestamp){//此时正处于等待中
				if(common_time_get_current_time_ms() - iservice->pre_timestamp > iservice->receive_timeout){//等待超时
					log_info("read timeout\r\n");
//					log_array("pop data", read_data, 1);
					vd_queue_pull(&iservice->rx_queue, read_data, 1);
					iservice->pre_timestamp = 0;
				}
			}else{//新的一轮等待
				iservice->pre_timestamp = common_time_get_current_time_ms();//记录当前时间
			}
			
			break;
		}
		
//		log_array("pop data", read_data, pop_size);
		vd_queue_pull(&iservice->rx_queue, read_data, pop_size);
		
		if(frame_len){//判断一帧成功
			protocol_deformat(format, frame_buf, &frame_len);//数据去格式
			log_debug("-------------->prot_id=%d match a frame success!!!\r\n", iservice->port_id);
//			log_array("payload", frame_buf, frame_len);
			
			iservice->pre_timestamp = 0;//清记录时间
			
			break;
		}
	}

//	log_debug("=====================end=====================\n");
	return frame_len;
}

/**
 * @brief : 判断一帧完整的数据
 *
 * @param format_id ： 协议id
 * @param data : 待判断数据
 * @param len : 数据长度.
 * @param frame_buf : 完整帧buf
 * @param frame_len : 完整帧长度
 * @return vd_uint32_t :	出队字节数
 */
static vd_uint32_t iservice_check_a_frame(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len, vd_uint8_t *frame_buf, vd_uint32_t *frame_len)
{
	vd_int32_t dedivert_cnt = 0;
	vd_uint8_t *p_head = vd_null;
	vd_bool_t is_full_head = vd_false;//是否是完整头帧
	vd_bool_t ret = vd_false;

	vd_check_return_val_error(data == vd_null, 0, "data null\r\n");
	vd_check_return_val_error(len <= 0, 0, "frame len <= 0\r\n");


	
	// 1, 匹配帧头
	if (len < FORMAT_GET_HEAD_CODE_BYTES(format)) {  // 帧头不够长，等待
		log_debug("XXXXXXXXXXX header len too short,wait... XXXXXXXXXXXX\r\n");
		return 0;
	}
	p_head = protocol_format_match_headcode(format, data, len, &is_full_head);
	if(p_head == vd_null || p_head < data){/* 未找到头帧，全部数据出队；异常保护：正常情况下p_head不可能小于data */
		log_debug("XXXXXXXXXXX header not found,pop all data XXXXXXXXXXXX\r\n");
		protocol_format_print(format);	
		return len;
	}
	/* 找到头帧或疑似头帧 */
	if(p_head != data){/* 头帧或疑似头帧不在数据头部，需要将前面几个无效数据出队 */
		log_debug("XXXXXXXXXXX The header frame is not in the header,pop %d bytes: XXXXXXXXXXXX\r\n", p_head - data);
		return p_head - data;
	}
	if(!is_full_head){//找到的是疑似头帧，需等待完整头帧
//		log_debug("XXXXXXXXXXX header incomplete,wait... XXXXXXXXXXXX\r\n");
		return 0;
	}
	
	
	// 2, 反转义, data和len可能改变
	dedivert_cnt = protocol_format_data_dedivert(format, data, &len);
	if(dedivert_cnt < 0){//反转义失败，证明数据长度不满足转义需要长度，需要等待
		log_debug("dedivert len too short, wait...");
		return 0;
	}
	//转义完成
	log_debug("dedivert_cnt=%d, len=%d\r\n", dedivert_cnt, len);
//	log_array("dedivert after data", data, len);


	
	// 3, 数据位置调整,data可能改变
	protocol_format_data_adjust_position(format, data, len);

	
	// 4, 匹配数据长度
	if (len < FORMAT_GET_FRAME_LEN_MIN(format)){//数据长度小于帧最小长度，需要等待	
		log_debug("len = %d < frame_len_min = %d, wait......\r\n", len, FORMAT_GET_FRAME_LEN_MIN(format));
		return 0;
	}
	ret = protocol_format_match_length(format, data, len, frame_len);//长度校验
	if (!ret){//长度异常，出队一字节
		return 1;
	}
	if(len < *frame_len){//当前数据长度小于帧全长，数据未接完，等待
		log_debug("len = %d < frame_len = %d, wait......\r\n", len, *frame_len);
		*frame_len = 0;
		return 0;
	}
	log_debug("get frame_len = %d\r\n", *frame_len);
	
	
	// 5, 匹配校验和
	if (!protocol_format_match_checkcode(format, data, *frame_len)){
		log_error("XXXXXXXXXXX data check error XXXXXXXXXXXX\r\n");
		protocol_format_print(format);
		log_array("checkcode error,data is", data, len);
		*frame_len = 0;
		return 1;
	}

	
	// 6, 匹配帧尾
	if (!protocol_format_match_endcode(format, data, *frame_len)) {	
		log_error("XXXXXXXXXXX endcode error XXXXXXXXXXXX\r\n");
		protocol_format_print(format);	
		*frame_len = 0;
		return 1;
	}
	
	
	// 此时，全部条件满足，匹配到正确的一帧数据，填充帧buf，返回帧全长+被转义的长度（出队用）
	memcpy(frame_buf, data, *frame_len);
	return *frame_len + FORMAT_DEDIVERT_COMPENSATE_BYTES(format, dedivert_cnt);
}

/**
 * @brief : iservcie初始化
 *
 * @return vd_bool_t
 */
vd_bool_t iservice_init(vd_void_t)
{
    vd_bool_t     ret = vd_false;
    vd_uint8_t    i;
		iservice_drv_t *iservice = vd_null;

    log_debug("iservice init start\r\n");

	 for (i = 0; i < vd_array_size(g_iservice_port_format_map); i++) {
			iservice = &g_iservice_port_format_map[i];
			log_debug("=================iservice info=====================\n", i);
			log_debug("iservice num = %d\n", vd_array_size(g_iservice_port_format_map));
			log_debug("port_id = %d\n", iservice->port_id);
			log_debug("read_method = %d\n", iservice->read_method);
			log_debug("format_id = %d\n", iservice->format_id);
			log_debug("=======================================================\n", i);
	 }
	
		//遍历iservice列表
    for (i = 0; i < vd_array_size(g_iservice_port_format_map); i++) {
			iservice = &g_iservice_port_format_map[i];
			
		//1.端口初始化
		ret = protocol_driver_get(iservice->port_id, &iservice->driver);//获取端口操作接口
		vd_check_return_val_error(!ret, vd_false, "protocol_driver_init error");
		if (iservice->driver.d_init){//端口初始化
			ret = iservice->driver.d_init(iservice->port_id, iservice->read_method);
			if(ret == vd_false){
				break;
			}
		}

		//2.协议初始化
		ret = protocol_format_init(iservice->format_id);
		if (ret == vd_false) {
				break;
		}
		
		//3.接收数据队列初始化
		vd_queue_init(&iservice->rx_queue, iservice->queue_buf, sizeof(iservice->queue_buf));
	}
	
	//端口中断模式接收回调注册
	protocol_driver_rx_it_callback_register(iservice_push_queue, vd_null);

	log_debug("iservice init %s\r\n", ret ? "success" : "failed");

	return ret;
}

/**
 * @brief : 					注册接收回调处理函数
 *
 * @param hander			回调函数
 * @return vd_bool_t 	注册结果
 */
vd_bool_t iservice_receive_callback_register(iservice_receive_callback_t hander)
{
  g_recvive_handler = hander;
	return vd_true;
}

/**
 * @brief : iservice主循环，解析上行数据，下发下行数据
 *
 * @return vd_void_t
 */
vd_void_t iservice_loop(vd_void_t)
{
	vd_uint8_t  i;
	vd_uint8_t  buf[PROTOCOL_FRAME_SIZE_MAX];  // 数据内容payload
	vd_uint32_t frame_len = 0;
	iservice_drv_t *iservice = vd_null;
	
	//遍历iservice列表
	for (i = 0; i < vd_array_size(g_iservice_port_format_map); i++){
		iservice = &g_iservice_port_format_map[i];
		memset(buf, 0, sizeof(buf));
		frame_len = iservice_deformat_read(iservice, buf);//去格式读取数据
		if (frame_len > 0) {
			if (g_recvive_handler) {//回调处理
					g_recvive_handler(iservice->port_id, buf, (vd_uint16_t)frame_len);
			}
		}
	}
}

/**
 * @brief :格式化写，把payload格式化一帧完整的数据，再使用写函数写入端口
 *
 * @param port_id: 端口
 * @param format_id: 协议格式
 * @param pbuf: 待格式化数据
 * @param len: 待格式化数据长度
 * @return vd_bool_t: 格式化以及写入结果
 */
vd_bool_t iservice_write(vd_bsp_port_e port_id, protocol_format_e format_id, vd_uint8_t *pbuf, vd_uint16_t len)
{
	vd_bool_t ret = vd_false;
	vd_uint8_t  i, write_data[PROTOCOL_FRAME_SIZE_MAX * 2];
	vd_uint32_t write_data_len;
	protocol_format_t *format = vd_null;
	
	for (i = 0; i < vd_array_size(g_iservice_port_format_map); i++){
		if(port_id == g_iservice_port_format_map[i].port_id && format_id == g_iservice_port_format_map[i].format_id){
			
			// 获取协议格式
			format = protocol_format_get_by_id(g_iservice_port_format_map[i].format_id);
			vd_check_return_val_error(format == vd_null, vd_false, "format_id=%d error\r\n", g_iservice_port_format_map[i].format_id);
			
			//数据打包（格式化）
			write_data_len = protocol_format(format, pbuf, len, write_data, sizeof(write_data));
			vd_check_return_val_error(!write_data_len, vd_false, "len zero");
			
			//写入格式化后的数据
			if (g_iservice_port_format_map[i].driver.d_write != vd_null){
					ret = g_iservice_port_format_map[i].driver.d_write(port_id, write_data, write_data_len);
			} else {
					log_debug("g_iservice_port_format_map[%d].driver.d_write null\r\n", port_id);
			}
			
			break;
		}
	}
	
	return ret;
}

#endif
