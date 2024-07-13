/*
 * Project: DT1866
 * Module: Protocol Format Util
 * File: protocol_format.c
 * Created Date: 2021-07-22 11:47:30
 * Author: meizhen
 * Description: description
 * -----
 * Last Modified: 2022-03-24 11:46:01
 * Modified By: meizhen
 * Commont: iservice2.0��Ƭ����
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
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
#include "protocol_format.h"
#include "vd_display_lib.h"
/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */
static vd_bool_t data_cmp(vd_uint8_t* data1, vd_uint8_t* data2, vd_uint32_t len); 
static vd_uint32_t data_contian(vd_uint8_t* data, vd_uint32_t len, vd_uint8_t target);
static vd_void_t data_move_backforward(vd_uint8_t* data, vd_uint32_t len);
static vd_void_t data_recover_position(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len);
static vd_uint16_t data_divert(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t len);
static vd_uint32_t calc_check_code(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t len);

/* ======================================================================================
 * globals
 */

static protocol_format_t g_protocol_format_array[PROTOCOL_FORMAT_MAX] = {
    {
        // �����׼Э���ʽ
        .is_valid        = 0,
        .send_frame_diff = 1,  // ����֡��ʽ�ͽ���֡��ͬ
        .frame =
            {.head_code           = {0x7E},
             .head_code_bytes     = 1,
             .len_pos             = 1,
             .len_bytes           = 1,
             .len_left            = 0,
						 .len_byte_order			= LITTLE_ENDIAN,
             .palyload_pos        = 2,
             .check_code_type     = "sum8",
             .check_code_bytes    = 1,
             .check_code_calc_pos = 1,
						 .check_code_byte_order = LITTLE_ENDIAN,
             .end_code            = {0x7E},
             .end_code_bytes      = 1,
             .divert_bytes        = 2,
             .diverts =
                 {{
                      .before_code = {0x7E},
                      .after_code  = {0x7D, 0x02},
                  },
                  {
                      .before_code = {0x7D},
                      .after_code  = {0x7D, 0x01},
                  }},
             },
				.send_frame =
            {.head_code           = {0x7E},
             .head_code_bytes     = 1,
             .len_pos             = 1,
             .len_bytes           = 1,
             .len_left            = 0,
             .palyload_pos        = 2,
             .check_code_type     = "sum8",
             .check_code_bytes    = 1,
             .check_code_calc_pos = 1,
             .end_code            = {0x7E},
             .end_code_bytes      = 1,
             .divert_bytes        = 2,
             .diverts =
                 {{
                      .before_code = {0x7E},
                      .after_code  = {0x7D, 0x02},
                  },
                  {
                      .before_code = {0x7D},
                      .after_code  = {0x7D, 0x01},
                  }}},
		},
    {
        // ��������Э���ʽ
        .is_valid        = 0,
        .send_frame_diff = 1,  // ����֡��ʽ�ͽ���֡��ͬ����Ҫ����send_frame�ֶ�
        .frame =
            {
                .head_code           = {0xFA},
                .head_code_bytes     = 1,
                .len_pos             = 0,
                .len_bytes           = 0,   // ��֡����
                .len_left            = 17,  // ֡ȫ���̶�17�ֽ�
                .check_code_type     = "crc16",
                .check_code_calc_pos = 1,
                .end_code            = {0x00},
                .end_code_bytes      = 0,  // ��֡β
                .divert_bytes        = 0,  // ��ת��
            },
        .send_frame =
            {
                .head_code           = {0xFB},
                .head_code_bytes     = 1,
                .len_pos             = 0,
                .len_bytes           = 0,   // ��֡����
                .len_left            = 11,  // ֡���ȹ̶�11�ֽ�
                .check_code_type     = "crc16",
                .check_code_calc_pos = 1,  // �ӵ�һ���ֽڿ�ʼ����У����
                .end_code            = {0x00},
                .end_code_bytes      = 0,  // ��֡β
                .divert_bytes        = 0,  // ��ת��
            },
    },
    {
        //һ��ͨЭ���ʽ
        .is_valid        = 0,
        .send_frame_diff = 0,
        .frame =
            {
                .head_code           = {0x08},
                .head_code_bytes     = 1,
                .len_pos             = 0,
                .len_bytes           = 0,
                .len_left            = 12,
                .check_code_type     = "bbc8",
                .check_code_bytes    = 1,
                .check_code_calc_pos = 0,
                .end_code            = {0},
                .end_code_bytes      = 0,
                .divert_bytes        = 0,
            },
			},
			{
        // �����׼Э���ʽ
        .is_valid        = 0,
        .send_frame_diff = 0,  // ����֡��ʽ�ͽ���֡��ͬ
        .frame =
            {.head_code           = {0x7E},
             .head_code_bytes     = 1,
             .len_pos             = 1,
             .len_bytes           = 1,
             .len_left            = 0,
             .palyload_pos        = 2,
             .check_code_type     = "sum8",
             .check_code_bytes    = 1,
             .check_code_calc_pos = 1,
             .end_code            = {0x7E},
             .end_code_bytes      = 1,
             .divert_bytes        = 2,
             .diverts =
                 {{
                      .before_code = {0x7E},
                      .after_code  = {0x7D, 0x02},
                  },
                  {
                      .before_code = {0x7D},
                      .after_code  = {0x7D, 0x01},
                  }}},
    }
};
/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/**
 * @brief : �ж��������������Ƿ�һ��
 *
 * @param data1
 * @param data2
 * @param len
 * @return vd_bool_t
 */
static vd_bool_t data_cmp(vd_uint8_t* data1, vd_uint8_t* data2, vd_uint32_t len) 
{
    vd_uint32_t i;
    for (i = 0; i < len; i++) {
        if (data1[i] != data2[i]) {
            return vd_false;
        }
    }
    return vd_true;
}

/**
 * @brief : �ж��������Ƿ����targetֵ
 *
 * @param data
 * @param len
 * @param target
 * @return vd_uint32_t
 */
static vd_uint32_t data_contian(vd_uint8_t* data, vd_uint32_t len, vd_uint8_t target) 
{
    vd_uint32_t i, count = 0;
    for (i = 0; i < len; i++) {
        if (data[i] == target) {
            count++;
        }
    }
    return count;
}

/**
 * @brief : ���ݺ��ƶ�һλ
 *
 * @param data
 * @param len
 * @return vd_void_t
 */
static vd_void_t data_move_backforward(vd_uint8_t* data, vd_uint32_t len) 
{
    while (len) {
        data[len] = data[len - 1];
        len--;
    }
}


/**
 * @brief : ����˳��ָ��ɿͻ�Э�鶨���˳��
 *
 * @param format
 * @param data
 * @param len
 * @return void_t
 */
static vd_void_t data_recover_position(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len)
{
	
}


/**
 * @brief : ����ת�壬��dataת�����Ȼ������dataλ��
 *
 * @param format
 * @return vd_uint16_t:ת��֮��ĳ���
 */
static vd_uint16_t data_divert(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t len)
{
    vd_uint32_t idx;
    frame_t*    frame;
    if (format->send_frame_diff) {
        frame = &format->send_frame;
    } else {
        frame = &format->frame;
    }

    idx = frame->head_code_bytes;  // ͷβ��ת��
    vd_check_return_val(!frame->divert_bytes, len);

    vd_uint16_t need_divert_size = 0;  // ��Ҫת��ĳ��ȣ�ͷβ��ת��

    need_divert_size = len - frame->head_code_bytes - frame->end_code_bytes;
    // log_debug("need_divert_size=%d,%d\r\n", need_divert_size, frame->divert_bytes);
    while (need_divert_size) {
        if (data[idx] == frame->diverts[0].before_code[0]) {
            data_move_backforward(&data[idx + 1], need_divert_size);  // ����Ųһλ
            memcpy(&data[idx], frame->diverts[0].after_code, 2);
            idx += 2;
            len++;
            log_debug("idx=%d len=%d\r\n", idx, len);
        } else if (data[idx] == frame->diverts[1].before_code[0]) {
            data_move_backforward(&data[idx + 1], need_divert_size);  // ����Ųһλ
            memcpy(&data[idx], frame->diverts[1].after_code, 2);
            idx += 2;
            len++;
            log_debug("idx=%d len=%d\r\n", idx, len);
        } else {
            idx++;
        }

        need_divert_size--;
    }
    return len;
}

/**
 * @brief : ����У������ͼ���У���
 *
 * @param check_type
 * @param data
 * @param len
 * @return vd_bool_t
 */
static vd_uint32_t calc_check_code(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t len)
{
    vd_uint32_t calc_code = 0;
    frame_t*    frame;

    vd_check_return_val_error(format == vd_null, 0, "format null\r\n");
    vd_check_return_val_error(data == vd_null || len <= 0, 0, "check data error");
    if (format->send_frame_diff) {
        frame = &format->send_frame;
    } else {
        frame = &format->frame;
    }
    vd_check_return_val_error(frame->check_code_type == vd_null, 0, "check_code_type null");

    if (!strcmp(frame->check_code_type, "sum8")) {
        calc_code = common_sum8(data, len);
    } else if (!strcmp(frame->check_code_type, "sum16")) {
        calc_code = common_sum16(data, len);
    } else if (!strcmp(frame->check_code_type, "sum32")) {
        calc_code = common_sum32(data, len);
    } else if (!strcmp(frame->check_code_type, "crc8")) {
        calc_code = common_crc8(data, len);
    } else if (!strcmp(frame->check_code_type, "crc16")) {
        // calc_code = common_crc16(data, len);
        calc_code = common_crc16_gen(0, data, len);
    } else if (!strcmp(frame->check_code_type, "crc32")) {
        calc_code = common_crc32(data, len);
    } else if (!strcmp(frame->check_code_type, "bbc8")) {
        calc_code = common_bbc8(data, len);
    }

    return calc_code;
}

/**
 * @brief : ��ȡЭ���ʽ
 *
 * @param protocol						Э��
 * @return protocol_format_t*	Э���ʽ
 */
protocol_format_t* protocol_format_get_by_id(protocol_format_e format_id)
{
    if (format_id < vd_array_size(g_protocol_format_array)){
			if(g_protocol_format_array[format_id].is_valid){
				return &g_protocol_format_array[format_id];
			}   
    }
    return vd_null;
}

/**
 * @brief : ��ӡЭ������
 *
 * @param format
 * @return vd_void_t
 */
vd_void_t protocol_format_print(protocol_format_t* format)
{
    vd_check_return(format == vd_null);

		log_debug("============================================ frame format print =============================================================\r\n");
		log_debug("frame:\r\n");
    log_debug("is_valid=%d\r\n", format->is_valid);
    log_debug("head_code=0x%x,head_code_bytes=%d", format->frame.head_code[0], format->frame.head_code_bytes);
    log_debug("end_code=0x%x,end_code_bytes=%d", format->frame.end_code[0], format->frame.end_code_bytes);
    log_debug(
        "len_left=%d,len_pos=%d,len_bytes=%d,palyload_pos=%d\r\n",
        format->frame.len_left,
        format->frame.len_pos,
        format->frame.len_bytes,
        format->frame.palyload_pos);
    log_debug(
        "check_code_type=%s,check_code_bytes=%d,check_code_calc_pos=%d\r\n",
        format->frame.check_code_type,
        format->frame.check_code_bytes,
        format->frame.check_code_calc_pos);
    if (format->send_frame_diff) {
        log_debug(
            "send frame:\r\nhead_code=0x%x,head_code_bytes=%d\r\n",
            format->send_frame.head_code[0],
            format->send_frame.head_code_bytes);
        log_debug("end_code=0x%x,end_code_bytes=%d", format->send_frame.end_code[0], format->send_frame.end_code_bytes);
        log_debug(
            "len_left=%d,len_pos=%d,len_bytes=%d,palyload_pos=%d\r\n",
            format->send_frame.len_left,
            format->send_frame.len_pos,
            format->send_frame.len_bytes,
            format->send_frame.palyload_pos);
        log_debug(
            "check_code_type=%s,check_code_bytes=%d,check_code_calc_pos=%d\r\n",
            format->send_frame.check_code_type,
            format->send_frame.check_code_bytes,
            format->send_frame.check_code_calc_pos);
    }
		log_debug("=========================================================================================================================\r\n");
}

/**
 * @brief								���ݷ�ת�壬 �磺0x7D01 --> 0x7E, 0x7D02 > 0x7D
 *
 * @param format_id			��ʽ
 * @param data 					����������
 * @param len						���������ݳ���
 * @param is_match_end	�Ƿ�ƥ�䵽β֡
 * @return vd_int32_t		�Ǹ�ֵ���ɹ�ת�����
												��ֵ��ʧ��
 */
vd_int32_t protocol_format_data_dedivert(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t* len) 
{
	vd_uint32_t i, j, divert_after_bytes, move_len = 0;
	vd_int32_t dedivert_cnt = 0;
	divert_after_bytes = FORMAT_DIVERT_AFTER_BYTES;//ת�����ֽ���
	
	//�������
	vd_check_return_val_error(format == vd_null, -1, "format null\r\n");
	vd_check_return_val_error(data == vd_null, -1, "data null\r\n");
	vd_check_return_val_error(len == vd_null, -1, "len null\r\n");
	
	vd_check_return_val_error(divert_after_bytes <= 1, -1, "no dedivert\r\n");//����Ҫת��
	
	if(*len < format->frame.head_code_bytes + divert_after_bytes){//ͷ֡������ݳ�������Ҫ��һ����ת��ǰ���ַ�����
		return -1;
	}
	if(*len < format->frame.head_code_bytes + format->frame.end_code_bytes){//ͷ֡������ݳ�������Ҫ��һ��β֡���ַ�����
		return -1;
	}	
	
	//��������
	for (i = format->frame.head_code_bytes; i < (*len) - (divert_after_bytes - 1); i++) {  // matched 7D01 or 7D02, and replaced it with 7E or 7D
		
		if(memcmp(&data[i], format->frame.end_code, format->frame.end_code_bytes) == 0){//��⵽β֡����Ҫֹͣת��
//			*is_match_end = vd_true;//����ҵ�β֡
			*len = i + 1;//�������ݳ��ȵ�β֡����ֹ
			log_debug("dedivert match endcod,stop,len = %d\r\n", *len);
			break;
		}
	
		for (j = 0; j < format->frame.divert_bytes; j++) {
				if (data[i] == format->frame.diverts[j].after_code[0] &&
						data[i + 1] == format->frame.diverts[j].after_code[1]) {
						// �ҵ���Ҫ�滻�������������������滻�µ����ݣ�����ǰŲ��һ���ֽ�
						dedivert_cnt++;
						data[i]  = format->frame.diverts[j].before_code[0];
						move_len = (*len) - (i + divert_after_bytes);
						if (move_len > 0) {
								memmove(&data[i + divert_after_bytes - 1], &data[i + divert_after_bytes], move_len);	
						}
						*len -= divert_after_bytes - 1;
						break;
				}
		}
	}
//	*len -= dedivert_cnt;
	return dedivert_cnt;
}

/**
 * @brief : ����˳�����Ϊ��׼˳��֡ͷ+����+����+У���+֡β
 *
 * @param format_id
 * @param data
 * @param len
 * @return void_t
 */
vd_void_t protocol_format_data_adjust_position(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len)
{
	
}

/**
 * @brief :							ƥ��ͷ֡
 *
 * @param format				Э���ʽ
 * @param data					����������
 * @param len						���ݳ���
 * @param is_full_head	�Ƿ�������ͷ֡�����ҵ�ͷ֡������ͷ֡ʱ������Ƿ�������ͷ֡
 * @return vd_uint8_t *	ͷ֡������ͷ֡�������еĵ�ַ
 */
vd_uint8_t *protocol_format_match_headcode(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len, vd_bool_t *is_full_head)
{
	vd_uint32_t i = 0;
	vd_uint8_t *p_data = data;
	
	vd_check_return_val(format == vd_null || data == vd_null || len == 0, vd_null);//�������
	vd_check_return_val(format->frame.head_code_bytes == 0, data);//��Э����ͷ֡
	
	*is_full_head = vd_false;//�ȱ��δ�ҵ�
	
    if (len < format->frame.head_code_bytes) {  // ����δ�ﵽͷ֡Ҫ�󳤶ȣ���������ͷ֡
			while(len){//�𽥱Ƚ�
				if (data_cmp(p_data, format->frame.head_code, len)){//������ͷ֡ǰlen���ֽ�ƥ�䣬��������ͷ֡��ַ
					return p_data;
				}else{//��ƥ�䣬�����������ݸ��ֽڣ����ٱȽϳ���
					p_data++;
					len--;
				}
			}
				
			//���⣬˵�����ݲ��ٿ��ܴ�������ͷ֡
      return vd_null;
    }
		
		/* ��������Ѱ��ͷ֡ */
		for (i = 0; i < len - (format->frame.head_code_bytes - 1); i++) {
				if (data_cmp(&data[i], format->frame.head_code, format->frame.head_code_bytes)) {
						break;
				}
		}
		if (i < len - (format->frame.head_code_bytes - 1)) {/* ͷ֡�������� */
				// log_debug("find headcode=0x%x at i=%d, pop %d bytes", format->frame.head_code[0], i, i);
			*is_full_head = vd_true;//����ҵ�����ͷ֡
			return data + i;
		}
		
		//ͷ֡������
		log_debug("headcode not found\r\n");
		return vd_null;

}

/**
 * @brief : �ж����ݳ�����֡ȫ���Ƿ�ƥ�䣬������֡ȫ��
 *
 * @param format : Э���ʽ
 * @param data : ���������
 * @param len : ��������ݳ���
 * @param frame_len : ֡ȫ��
* @return vd_bool_t ���
 */
vd_bool_t protocol_format_match_length(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t len, vd_uint32_t *frame_len)
{
    vd_int32_t i, len_min, len_max;
	
		len_min = FORMAT_GET_FRAME_LEN_MIN(format);
		len_max = PROTOCOL_FRAME_SIZE_MAX;
		*frame_len = 0;
	
    // 1, ���ݳ���С��֡��С����
    if (len < len_min){
        return vd_false;
    }

    // 2, ��ȡ֡���ȣ�����len_left����֡ȫ��
		if(format->frame.len_bytes > sizeof(*frame_len)){//��������
			return vd_false;
		}
    for (i = 0; i < format->frame.len_bytes; i++) {
			if(format->frame.len_byte_order == LITTLE_ENDIAN){
				*frame_len = *frame_len | (data[format->frame.len_pos + i] << (8 * i));//С��ģʽ
			}else{
        *frame_len = (*frame_len << 8) | data[format->frame.len_pos + i];//���ģʽ
			}
    }
		*frame_len += format->frame.len_left;

    // 3, ƥ�����ݳ��Ⱥ�֡ȫ��
    if (*frame_len < len_min) {  // ֡ȫ��С����С֡���ȣ�֡����
        log_error("frame len=%d to short!!! len_min=%d", *frame_len, len_min);
				*frame_len = 0;
        return vd_false;
    }
    
    if (len_max > 0 && *frame_len > len_max) {  //֡ȫ���������ֵ��֡����
        log_array("data", data, len);
        log_error("frame len=%d to long!!! len_max=%d", *frame_len, len_max);
				*frame_len = 0;
        return vd_false;
    }

    return vd_true;
}

/**
 * @brief : ����У��ͣ��������ݵ�У���ƥ��
 *
 * @param data
 * @param format
 * @return vd_bool_t
 */
vd_bool_t protocol_format_match_checkcode(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t frame_len) 
{
    vd_uint32_t check_code = 0, calc_code, check_code_pos, i;
    vd_int32_t  calc_len;

    vd_check_return_val(format->frame.check_code_bytes == 0, vd_true);//��Э����У��λ

    // У�鳤�� = ֡ȫ�� - ֡β�ֽ��� - У����ֽ��� - ��ʼ����У��͵���ʼ����
    calc_len = frame_len - format->frame.end_code_bytes - format->frame.check_code_bytes - format->frame.check_code_calc_pos;
    vd_check_return_val_error(calc_len < 0, vd_false, "calc_len < 0");

    calc_code = calc_check_code(format, &data[format->frame.check_code_calc_pos], calc_len);

    // ����У��ʹ�ŵ�λ��
    check_code_pos = calc_len + format->frame.check_code_calc_pos;

    // ��ȡ�����е�У���
		if(format->frame.check_code_bytes > sizeof(check_code)){//��������
			return vd_false;
		}
    for (i = 0; i < format->frame.check_code_bytes; i++) {
			if(format->frame.check_code_byte_order == LITTLE_ENDIAN){
				check_code = check_code | (data[check_code_pos + i] << (8 * i));//С��ģʽ
			}else{
        check_code = (check_code << 8) | data[check_code_pos + i];//���ģʽ
			}
    }

    if (check_code != calc_code) {
        log_error(
            "checkcode[%d]=0x%02x != calc_code=0x%02x, calc_len=%d!!!",
            check_code_pos,
            check_code,
            calc_code,
            calc_len);
        return vd_false;
    }

    return vd_true;
}

/**
 * @brief : ƥ��֡β
 *
 * @param data
 * @param format
 * @return vd_int_t
 */
vd_bool_t protocol_format_match_endcode(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t frame_len)
{
    vd_uint32_t i;
    vd_uint32_t end_code_pos;

    vd_check_return_val(format->frame.end_code_bytes == 0, vd_true);

    end_code_pos = frame_len - format->frame.end_code_bytes;
    for (i = 0; i < format->frame.end_code_bytes; i++) {
        if (data[end_code_pos + i] != format->frame.end_code[i]) {
            log_error("endcode=0x%x != 0x%x!!!", data[end_code_pos + i], format->frame.end_code);
            return vd_false;
        }
    }

    return vd_true;
}

/**
 * @brief :Э���ʽ������ͷ��β��У��ͣ���װpalyload,���һ������������
 *
 * @param data: ��Ҫ��ʽ��������
 * @param len: ��Ҫ��ʽ�������ݳ���
 * @param write_buf: ��ʽ��֮������ݻ���
 * @param buf_size: ��ʽ��֮������ݻ����С
 * @return vd_uint32_t: ��ʽ��֮������ݳ���
 */
vd_uint32_t protocol_format(protocol_format_t* format, vd_uint8_t *data, vd_uint16_t len, vd_uint8_t* write_buf, vd_uint32_t buf_size)
{
    vd_uint32_t i = 0, j = 0;
    vd_uint32_t full_len, frame_len, temp;
    vd_uint32_t calc_code, calc_len;
    frame_t*    frame;

    // ��֤ת������鲻Խ��
    vd_check_return_val_error(buf_size < PROTOCOL_FRAME_SIZE_MAX * 2, 0, "buf_size too short");

    if (format->send_frame_diff) {
        frame = &format->send_frame;
    } else {
        frame = &format->frame;
    }

    // 1.1,����֡ȫ��
    full_len =
        len + frame->head_code_bytes + frame->len_bytes + frame->check_code_bytes + frame->end_code_bytes;

    // ��ֹ����Խ��
    vd_check_return_val_error(full_len > buf_size, 0, "full_len=%d > buf_size=%d", full_len, buf_size);

    // 1.2,����֡��
    frame_len = full_len - frame->len_left;  // Э���ж����֡����

    // 2,����֡ͷ
    if (frame->head_code_bytes) {
        memcpy(&write_buf[i], &frame->head_code[0], frame->head_code_bytes);
        i += frame->head_code_bytes;
    }

    // 3,����֡����,(ת��֮ǰ�ĳ���)
    if (frame->len_bytes) {
        temp = frame_len;
        for (j = 0; j < frame->len_bytes; j++) {
            // write_data[i + j] = (temp >> (j * 8)) & 0xff;  // С�ˣ����ֽڷ��ڵ͵�ַ
            write_buf[i + (frame->len_bytes - 1 - j)] = (temp >> (j * 8)) & 0xff;  // ��ˣ����ֽڷ��ڸߵ�ַ
        }
        i += frame->len_bytes;
    }

    // 4,����payload����
    memcpy(&write_buf[i], data, len);
    i += len;

    // 5,����У���
    if (frame->check_code_bytes) {
        calc_len  = full_len - frame->end_code_bytes - frame->check_code_bytes - frame->check_code_calc_pos;
        calc_code = calc_check_code(format, &write_buf[frame->check_code_calc_pos], calc_len);
        for (j = 0; j < frame->check_code_bytes; j++) {
            // write_data[i + j] = (calc_code >> (j * 8)) & 0xff;  // С�ˣ����ֽڷ��ڵ͵�ַ
            write_buf[i + (frame->check_code_bytes - 1 - j)] = (calc_code >> (j * 8)) & 0xff;  // ��ˣ����ֽڷ��ڸߵ�ַ
        }
        i += frame->check_code_bytes;
    }

    // 6������֡β
    if (frame->end_code_bytes) {
        memcpy(&write_buf[i], &frame->end_code[0], frame->end_code_bytes);
        i += frame->end_code_bytes;
    }

//    log_debug("i=%d, full_len=%d, len=%d, len_left=%d\r\n", i, full_len, frame->len, frame->len_left);
//    protocol_format_print(format);

    // 7, ����ת��
    if (frame->divert_bytes) {
        i = data_divert(format, write_buf, i);
    }

    // 8, ����λ�ý���
    data_recover_position(format, write_buf, i);

    return i;
}

/**
 * @brief : ����ָ��ȥ��ʽ����ȥ��ת�壬֡ͷ֡β����У���
 *
 * @param format
 * @param data
 * @param frame_len
 * @return vd_bool_t
 */
vd_bool_t protocol_deformat(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t* len) 
{
    vd_uint16_t payload_len;
    vd_uint16_t payload_pos;

    vd_check_return_val_error(format == vd_null, vd_false, "format null\r\n");
    vd_check_return_val_error(data == vd_null, vd_false, "write_data null");
    vd_check_return_val_error(*len <= 0, vd_false, "len <= 0");
    // protocol_format_print(format);

    payload_len = *len - format->frame.head_code_bytes - format->frame.len_bytes -
                  format->frame.check_code_bytes - format->frame.end_code_bytes;

    // ����payload��ʼλ��
    // payload_pos = format->frame.len_pos + format->frame.len_bytes;
    payload_pos = format->frame.head_code_bytes + format->frame.len_bytes;

    memmove(data, &data[payload_pos], payload_len);

    *len = payload_len;

    return vd_true;
}





/**
 * @brief :  Э���ʼ�����ж�Э���ʽ�Ƿ���Ч
 *
 * @param formats : Э���ʽ����
 * @param array_size �� Э���ʽ�����С
 * @return vd_int8_t
 */
vd_bool_t protocol_format_init(vd_uint8_t format_id) 
{
    protocol_format_t* format = vd_null;
	
    if (format_id >= vd_array_size(g_protocol_format_array)){
			log_error("format_id = %d invalid!!!\r\n");
			return vd_false; 
    }
	
		format = &g_protocol_format_array[format_id];

    // У����ֽ�������
    if (!strcmp(format->frame.check_code_type, "sum8")) {
        format->frame.check_code_bytes = 1;
        if (format->send_frame_diff) {
            format->send_frame.check_code_bytes = 1;
        }
    } else if (!strcmp(format->frame.check_code_type, "sum16")) {
        format->frame.check_code_bytes = 2;
        if (format->send_frame_diff) {
            format->send_frame.check_code_bytes = 2;
        }
    } else if (!strcmp(format->frame.check_code_type, "sum32")) {
        format->frame.check_code_bytes = 4;
        if (format->send_frame_diff) {
            format->send_frame.check_code_bytes = 4;
        }
    } else if (!strcmp(format->frame.check_code_type, "crc8")) {
        format->frame.check_code_bytes = 1;
        if (format->send_frame_diff) {
            format->send_frame.check_code_bytes = 1;
        }
    } else if (!strcmp(format->frame.check_code_type, "crc16")) {
        format->frame.check_code_bytes = 2;
        if (format->send_frame_diff) {
            format->send_frame.check_code_bytes = 2;
        }
    } else if (!strcmp(format->frame.check_code_type, "crc32")) {
        format->frame.check_code_bytes = 4;
        if (format->send_frame_diff) {
            format->send_frame.check_code_bytes = 4;
        }
    } else if (!strcmp(format->frame.check_code_type, "bbc8")) {
        format->frame.check_code_bytes = 1;
        if (format->send_frame_diff) {
            format->send_frame.check_code_bytes = 1;
        }
    } else {
        log_error("not support this check_code_type=%s", format->frame.check_code_type);
        return vd_false;
    }

    format->is_valid = vd_true;

    return vd_true;
}
#endif
