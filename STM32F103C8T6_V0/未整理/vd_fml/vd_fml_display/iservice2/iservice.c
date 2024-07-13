/*
 * Project: mcu-kit
 * Module: iservice2.1��Ƭ����
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
 
/* iservice �������󣨶˿�--Э���ʽӳ�䣩 */
typedef struct {
	vd_bsp_port_e 								port_id;					//�˿ں�
	read_method_e									read_method;			//���ݶ�ȡ��ʽ
	protocol_format_e 						format_id;				//��ʽid
	vd_uint64_t										receive_timeout;	//���ݽ��ճ�ʱʱ��
	
	protocol_driver_t							driver;						//Э������
	cmd_queue_t       						rx_queue;  				//���ڽ������ݵ�ѭ������
	vd_uint8_t										queue_buf[200];		//���л���
	vd_uint64_t 									pre_timestamp;		//���ڼ�¼�ȴ�ƥ��һ֡�������ݵ���ʼʱ���
}iservice_drv_t;
 
static vd_uint32_t iservice_deformat_read(iservice_drv_t *iservice, vd_uint8_t *buf);

static vd_uint32_t iservice_check_a_frame(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len, vd_uint8_t *frame_buf, vd_uint32_t *frame_len);


/* ======================================================================================
 * globals
 */
 
/**
 * @brief :Э���ʽӳ���
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
 * @brief : ����push���ݵ�����ѭ�����С�
 *
 * @param port_id:��Ӧ�Ķ˿ں�
 * @param data:
 * @param data_len
 * @return vd_int8_t
 */
 vd_void_t iservice_push_queue(vd_bsp_port_e port_id, vd_uint8_t* pbuf, vd_uint16_t len, vd_void_t *arg) 
{
	vd_uint8_t i;
	
	for (i = 0; i < vd_array_size(g_iservice_port_format_map); i++){//������push����Ӧ�˿ڵĶ�����
		if(port_id == g_iservice_port_format_map[i].port_id){
			vd_queue_push(&g_iservice_port_format_map[i].rx_queue, pbuf, len);
		}
	}
}

/**
 * @brief : ȥ��ʽ�������Ӷ˿ڶ�ȡһ֡���ݣ���ȥ��ʽ��ȥ��֡ͷ֡β����У��ͣ�������payload
 *
 * @param iservice		iservice��������
 * @param frame_buf		��Ч���ݻ���
 * @return vd_int32_t ��ֵ��ʧ��
											�Ǹ�ֵ����Ч���ݳ���
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

	// 1,��ѯ�ķ�ʽ�ӵײ�Ӳ���˿ڶ�ȡ���ݣ������浽����
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

	// 2.��ȡЭ���ʽ
//	log_debug(">>>2.get format\n");
	format = protocol_format_get_by_id(iservice->format_id);
	vd_check_return_val_error(format == vd_null, -1, "format_id=%d error\r\n", iservice->format_id);
	
	
	// 3.��ѭ�����л�ȡ�������ݣ����ж�һ֡����
//	log_debug(">>>3.data pop\n");
	while(1){// ��������
		read_count = vd_queue_pull_notout(&iservice->rx_queue, read_data, sizeof(read_data));
		if(read_count == 0){//����������
			break;
		}
		
//		log_array("queue read data", read_data, read_count);
		
		pop_size = iservice_check_a_frame(format, read_data, read_count, frame_buf, &frame_len);//��������Ƿ����һ֡�������ݣ���������������frame_buf��������Ҫ�����ֽ���
		
		// log_debug("pop_size=%d\r\n", pop_size);
		if(pop_size == 0){// �˴�pop_size=0��ʾû��ȫһ֡���ݶ��ȴ����˴������õȴ���ʱ
			
			frame_len = 0;
			
			if(iservice->pre_timestamp){//��ʱ�����ڵȴ���
				if(common_time_get_current_time_ms() - iservice->pre_timestamp > iservice->receive_timeout){//�ȴ���ʱ
					log_info("read timeout\r\n");
//					log_array("pop data", read_data, 1);
					vd_queue_pull(&iservice->rx_queue, read_data, 1);
					iservice->pre_timestamp = 0;
				}
			}else{//�µ�һ�ֵȴ�
				iservice->pre_timestamp = common_time_get_current_time_ms();//��¼��ǰʱ��
			}
			
			break;
		}
		
//		log_array("pop data", read_data, pop_size);
		vd_queue_pull(&iservice->rx_queue, read_data, pop_size);
		
		if(frame_len){//�ж�һ֡�ɹ�
			protocol_deformat(format, frame_buf, &frame_len);//����ȥ��ʽ
			log_debug("-------------->prot_id=%d match a frame success!!!\r\n", iservice->port_id);
//			log_array("payload", frame_buf, frame_len);
			
			iservice->pre_timestamp = 0;//���¼ʱ��
			
			break;
		}
	}

//	log_debug("=====================end=====================\n");
	return frame_len;
}

/**
 * @brief : �ж�һ֡����������
 *
 * @param format_id �� Э��id
 * @param data : ���ж�����
 * @param len : ���ݳ���.
 * @param frame_buf : ����֡buf
 * @param frame_len : ����֡����
 * @return vd_uint32_t :	�����ֽ���
 */
static vd_uint32_t iservice_check_a_frame(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len, vd_uint8_t *frame_buf, vd_uint32_t *frame_len)
{
	vd_int32_t dedivert_cnt = 0;
	vd_uint8_t *p_head = vd_null;
	vd_bool_t is_full_head = vd_false;//�Ƿ�������ͷ֡
	vd_bool_t ret = vd_false;

	vd_check_return_val_error(data == vd_null, 0, "data null\r\n");
	vd_check_return_val_error(len <= 0, 0, "frame len <= 0\r\n");


	
	// 1, ƥ��֡ͷ
	if (len < FORMAT_GET_HEAD_CODE_BYTES(format)) {  // ֡ͷ���������ȴ�
		log_debug("XXXXXXXXXXX header len too short,wait... XXXXXXXXXXXX\r\n");
		return 0;
	}
	p_head = protocol_format_match_headcode(format, data, len, &is_full_head);
	if(p_head == vd_null || p_head < data){/* δ�ҵ�ͷ֡��ȫ�����ݳ��ӣ��쳣���������������p_head������С��data */
		log_debug("XXXXXXXXXXX header not found,pop all data XXXXXXXXXXXX\r\n");
		protocol_format_print(format);	
		return len;
	}
	/* �ҵ�ͷ֡������ͷ֡ */
	if(p_head != data){/* ͷ֡������ͷ֡��������ͷ������Ҫ��ǰ�漸����Ч���ݳ��� */
		log_debug("XXXXXXXXXXX The header frame is not in the header,pop %d bytes: XXXXXXXXXXXX\r\n", p_head - data);
		return p_head - data;
	}
	if(!is_full_head){//�ҵ���������ͷ֡����ȴ�����ͷ֡
//		log_debug("XXXXXXXXXXX header incomplete,wait... XXXXXXXXXXXX\r\n");
		return 0;
	}
	
	
	// 2, ��ת��, data��len���ܸı�
	dedivert_cnt = protocol_format_data_dedivert(format, data, &len);
	if(dedivert_cnt < 0){//��ת��ʧ�ܣ�֤�����ݳ��Ȳ�����ת����Ҫ���ȣ���Ҫ�ȴ�
		log_debug("dedivert len too short, wait...");
		return 0;
	}
	//ת�����
	log_debug("dedivert_cnt=%d, len=%d\r\n", dedivert_cnt, len);
//	log_array("dedivert after data", data, len);


	
	// 3, ����λ�õ���,data���ܸı�
	protocol_format_data_adjust_position(format, data, len);

	
	// 4, ƥ�����ݳ���
	if (len < FORMAT_GET_FRAME_LEN_MIN(format)){//���ݳ���С��֡��С���ȣ���Ҫ�ȴ�	
		log_debug("len = %d < frame_len_min = %d, wait......\r\n", len, FORMAT_GET_FRAME_LEN_MIN(format));
		return 0;
	}
	ret = protocol_format_match_length(format, data, len, frame_len);//����У��
	if (!ret){//�����쳣������һ�ֽ�
		return 1;
	}
	if(len < *frame_len){//��ǰ���ݳ���С��֡ȫ��������δ���꣬�ȴ�
		log_debug("len = %d < frame_len = %d, wait......\r\n", len, *frame_len);
		*frame_len = 0;
		return 0;
	}
	log_debug("get frame_len = %d\r\n", *frame_len);
	
	
	// 5, ƥ��У���
	if (!protocol_format_match_checkcode(format, data, *frame_len)){
		log_error("XXXXXXXXXXX data check error XXXXXXXXXXXX\r\n");
		protocol_format_print(format);
		log_array("checkcode error,data is", data, len);
		*frame_len = 0;
		return 1;
	}

	
	// 6, ƥ��֡β
	if (!protocol_format_match_endcode(format, data, *frame_len)) {	
		log_error("XXXXXXXXXXX endcode error XXXXXXXXXXXX\r\n");
		protocol_format_print(format);	
		*frame_len = 0;
		return 1;
	}
	
	
	// ��ʱ��ȫ���������㣬ƥ�䵽��ȷ��һ֡���ݣ����֡buf������֡ȫ��+��ת��ĳ��ȣ������ã�
	memcpy(frame_buf, data, *frame_len);
	return *frame_len + FORMAT_DEDIVERT_COMPENSATE_BYTES(format, dedivert_cnt);
}

/**
 * @brief : iservcie��ʼ��
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
	
		//����iservice�б�
    for (i = 0; i < vd_array_size(g_iservice_port_format_map); i++) {
			iservice = &g_iservice_port_format_map[i];
			
		//1.�˿ڳ�ʼ��
		ret = protocol_driver_get(iservice->port_id, &iservice->driver);//��ȡ�˿ڲ����ӿ�
		vd_check_return_val_error(!ret, vd_false, "protocol_driver_init error");
		if (iservice->driver.d_init){//�˿ڳ�ʼ��
			ret = iservice->driver.d_init(iservice->port_id, iservice->read_method);
			if(ret == vd_false){
				break;
			}
		}

		//2.Э���ʼ��
		ret = protocol_format_init(iservice->format_id);
		if (ret == vd_false) {
				break;
		}
		
		//3.�������ݶ��г�ʼ��
		vd_queue_init(&iservice->rx_queue, iservice->queue_buf, sizeof(iservice->queue_buf));
	}
	
	//�˿��ж�ģʽ���ջص�ע��
	protocol_driver_rx_it_callback_register(iservice_push_queue, vd_null);

	log_debug("iservice init %s\r\n", ret ? "success" : "failed");

	return ret;
}

/**
 * @brief : 					ע����ջص�������
 *
 * @param hander			�ص�����
 * @return vd_bool_t 	ע����
 */
vd_bool_t iservice_receive_callback_register(iservice_receive_callback_t hander)
{
  g_recvive_handler = hander;
	return vd_true;
}

/**
 * @brief : iservice��ѭ���������������ݣ��·���������
 *
 * @return vd_void_t
 */
vd_void_t iservice_loop(vd_void_t)
{
	vd_uint8_t  i;
	vd_uint8_t  buf[PROTOCOL_FRAME_SIZE_MAX];  // ��������payload
	vd_uint32_t frame_len = 0;
	iservice_drv_t *iservice = vd_null;
	
	//����iservice�б�
	for (i = 0; i < vd_array_size(g_iservice_port_format_map); i++){
		iservice = &g_iservice_port_format_map[i];
		memset(buf, 0, sizeof(buf));
		frame_len = iservice_deformat_read(iservice, buf);//ȥ��ʽ��ȡ����
		if (frame_len > 0) {
			if (g_recvive_handler) {//�ص�����
					g_recvive_handler(iservice->port_id, buf, (vd_uint16_t)frame_len);
			}
		}
	}
}

/**
 * @brief :��ʽ��д����payload��ʽ��һ֡���������ݣ���ʹ��д����д��˿�
 *
 * @param port_id: �˿�
 * @param format_id: Э���ʽ
 * @param pbuf: ����ʽ������
 * @param len: ����ʽ�����ݳ���
 * @return vd_bool_t: ��ʽ���Լ�д����
 */
vd_bool_t iservice_write(vd_bsp_port_e port_id, protocol_format_e format_id, vd_uint8_t *pbuf, vd_uint16_t len)
{
	vd_bool_t ret = vd_false;
	vd_uint8_t  i, write_data[PROTOCOL_FRAME_SIZE_MAX * 2];
	vd_uint32_t write_data_len;
	protocol_format_t *format = vd_null;
	
	for (i = 0; i < vd_array_size(g_iservice_port_format_map); i++){
		if(port_id == g_iservice_port_format_map[i].port_id && format_id == g_iservice_port_format_map[i].format_id){
			
			// ��ȡЭ���ʽ
			format = protocol_format_get_by_id(g_iservice_port_format_map[i].format_id);
			vd_check_return_val_error(format == vd_null, vd_false, "format_id=%d error\r\n", g_iservice_port_format_map[i].format_id);
			
			//���ݴ������ʽ����
			write_data_len = protocol_format(format, pbuf, len, write_data, sizeof(write_data));
			vd_check_return_val_error(!write_data_len, vd_false, "len zero");
			
			//д���ʽ���������
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
