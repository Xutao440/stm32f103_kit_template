/*
 * Project: DT1866
 * Module: Protocol Format Define
 * File: protocol_format.h
 * Created Date: 2021-07-22 10:24:54
 * Author: meizhen
 * Description: description
 * -----
 * Last Modified: 2022-03-24 11:46:01
 * Modified By: meizhen
 * Commont: iservice2.0��Ƭ����
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef ROTOCOL_FORMAT_H
#define ROTOCOL_FORMAT_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#include "iservice_def.h"

/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
 
#define FORMAT_DIVERT_AFTER_BYTES		2	//ת��֮���ֽ���

#define FORMAT_GET_HEAD_CODE_BYTES(format)													format->frame.head_code_bytes	//��ȡͷ֡�ֽ���

#define FORMAT_GET_END_CODE_BYTES(format)													format->frame.end_code_bytes	//��ȡβ֡�ֽ���

#define FORMAT_GET_FRAME_LEN_MIN(format)													format->frame.head_code_bytes + /* ��ȡ��С֡���� */\
																																			format->frame.len_bytes + \
																																			format->frame.check_code_bytes + \
																																			format->frame.end_code_bytes
																																			
#define FORMAT_DEDIVERT_COMPENSATE_BYTES(format, dedivert_cnt)		dedivert_cnt * (FORMAT_DIVERT_AFTER_BYTES - 1)	//��ת���ֽ��������������ã�

/* ======================================================================================
 * types
 */

//�ֽ���
typedef enum{
	LITTLE_ENDIAN = 0,
	BIG_ENDIAN,
}frame_byte_order_e;


typedef struct {
    vd_uint8_t before_code[1];  // ��Ҫ��ת���ַ�, 0x7E, 0x7D
    vd_uint8_t after_code[2];   // ת��֮����ַ�, 0x7D01, 0x7D02
} divert_t;

typedef struct {
    vd_uint8_t 					head_code[4];     // Э��ͷ����Ϊ��,�ַ���תΪ�������鱣��, "7E"-->0x7E
    vd_uint8_t 					head_code_bytes;  // Э��ͷ��ռ���ֽ����� 0����ʾ��֡ͷ

    vd_uint8_t 					end_code[4];     // Э��β,�ַ���תΪ�������鱣��, "7E"-->0x7E
    vd_uint8_t 					end_code_bytes;  // Э��β��ռ���ֽ�����0����ʾ��֡β

    vd_uint8_t 					len_pos;  	// Э�鳤�����ڵ�λ��
    vd_uint8_t 					len_bytes;  // ֡������ռ���ֽ�����0����ʾ��֡���ȣ���̶�֡���ȣ�
    vd_uint8_t 					len_left;   // ���֡ȫ������Ҫ���ֽ�����len+len_left = ֡ȫ��
		frame_byte_order_e 	len_byte_order;//�����ֽ���

    vd_uint8_t 					palyload_pos;               // ��Ч������ʼλ��
    vd_uint8_t 					palyload_before_len_bytes;  // ����ǰ���ж����ֽڵ�����

    vd_char_t  					check_code_type[6];   // У��͵����ͣ���sum,crc8,crc16�� ��Ϊ��
    vd_uint8_t 					check_code_bytes;     // У�����ռ���ֽ���, 0:��ʾ�޼����
    vd_uint8_t 					check_code_calc_pos;  // ��ʼУ�����ݵ���ʼλ��
		frame_byte_order_e 	check_code_byte_order;// У���ֽ���

    // ת���������
    divert_t   diverts[2];
    vd_uint8_t divert_bytes;  // ��Ҫת����ֽ���, 0����ʾ����ת��
} frame_t;

typedef struct {
    vd_uint8_t  is_valid : 1;         // Э���ʽ�Ƿ���Ч
    vd_uint8_t  send_frame_diff : 1;  // ����֡��ʽ��ͬ�ڽ���֡

    frame_t frame;       // ֡�����������
    frame_t send_frame;  // ����֡�����������

} protocol_format_t;



/* ======================================================================================
 * declaration
 */

/**
 * @brief :  Э���ʼ�����ж�Э���ʽ�Ƿ���Ч
 *
 * @param formats : Э���ʽ����
 * @param array_size �� Э���ʽ�����С
 * @return vd_int8_t
 */
vd_bool_t protocol_format_init(vd_uint8_t format_id);

/**
 * @brief : ��ȡЭ���ʽ
 *
 * @param protocol						Э��
 * @return protocol_format_t*	Э���ʽ
 */
protocol_format_t* protocol_format_get_by_id(protocol_format_e format_id);

/**
 * @brief : ��ӡЭ������
 *
 * @param format
 * @return vd_void_t
 */
vd_void_t protocol_format_print(protocol_format_t* format);

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
vd_int32_t protocol_format_data_dedivert(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t* len) ;

/**
 * @brief : ����˳�����Ϊ��׼˳��֡ͷ+����+����+У���+֡β
 *
 * @param format_id
 * @param data
 * @param len
 * @return void_t
 */
vd_void_t protocol_format_data_adjust_position(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len);

/**
 * @brief :							ƥ��ͷ֡
 *
 * @param format				Э���ʽ
 * @param data					����������
 * @param len						���ݳ���
 * @param ret						ƥ������true--ƥ�䵽��ȷͷ֡������ͷ֡��ַ��false--δƥ�䵽ͷ֡��������null��ƥ�䵽����ͷ֡��������������ͷ֡��ַ
 * @return vd_uint8_t *	ͷ֡������ͷ֡�������еĵ�ַ
 */
vd_uint8_t *protocol_format_match_headcode(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len, vd_bool_t *ret);

/**
 * @brief : �ж����ݳ�����֡ȫ���Ƿ�ƥ�䣬������֡ȫ��
 *
 * @param data : ��ȡ������
 * @param len : ��ȡ�����ݳ���
 * @param format : Э���ʽ�����ݽṹ
 * @param frame_len : ֡ȫ��
* @return vd_bool_t ���
 */
vd_bool_t protocol_format_match_length(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t len, vd_uint32_t *frame_len);

/**
 * @brief : ����У��ͣ��������ݵ�У���ƥ��
 *
 * @param data
 * @param format
 * @return vd_bool_t
 */
vd_bool_t protocol_format_match_checkcode(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t frame_len);

/**
 * @brief : ƥ��֡β
 *
 * @param data
 * @param format
 * @return vd_int_t
 */
vd_bool_t protocol_format_match_endcode(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t frame_len);

/**
 * @brief :Э���ʽ������ͷ��β��У��ͣ���װpalyload,���һ������������
 *
 * @param data: ��Ҫ��ʽ��������
 * @param len: ��Ҫ��ʽ�������ݳ���
 * @param write_buf: ��ʽ��֮������ݻ���
 * @param buf_size: ��ʽ��֮������ݻ����С
 * @return vd_uint32_t: ��ʽ��֮������ݳ���
 */
vd_uint32_t protocol_format(protocol_format_t* format, vd_uint8_t *data, vd_uint16_t len, vd_uint8_t* write_buf, vd_uint32_t buf_size);

/**
 * @brief : ����ָ��ȥ��ʽ����ȥ��ת�壬֡ͷ֡β����У���
 *
 * @param format
 * @param write_data
 * @param frame_len
 * @return vd_bool_t
 */
vd_bool_t protocol_deformat(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t* len);

/* ======================================================================================
 * extern
 */
// __vd_extern_c_leave__

#endif  // ROTOCOL_FORMAT_H
