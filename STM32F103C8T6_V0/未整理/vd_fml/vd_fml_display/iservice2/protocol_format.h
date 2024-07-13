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
 * Commont: iservice2.0单片机版
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
 
#define FORMAT_DIVERT_AFTER_BYTES		2	//转义之后字节数

#define FORMAT_GET_HEAD_CODE_BYTES(format)													format->frame.head_code_bytes	//获取头帧字节数

#define FORMAT_GET_END_CODE_BYTES(format)													format->frame.end_code_bytes	//获取尾帧字节数

#define FORMAT_GET_FRAME_LEN_MIN(format)													format->frame.head_code_bytes + /* 获取最小帧长度 */\
																																			format->frame.len_bytes + \
																																			format->frame.check_code_bytes + \
																																			format->frame.end_code_bytes
																																			
#define FORMAT_DEDIVERT_COMPENSATE_BYTES(format, dedivert_cnt)		dedivert_cnt * (FORMAT_DIVERT_AFTER_BYTES - 1)	//反转义字节数补偿（出队用）

/* ======================================================================================
 * types
 */

//字节序
typedef enum{
	LITTLE_ENDIAN = 0,
	BIG_ENDIAN,
}frame_byte_order_e;


typedef struct {
    vd_uint8_t before_code[1];  // 需要被转义字符, 0x7E, 0x7D
    vd_uint8_t after_code[2];   // 转义之后的字符, 0x7D01, 0x7D02
} divert_t;

typedef struct {
    vd_uint8_t 					head_code[4];     // 协议头，不为空,字符串转为整型数组保存, "7E"-->0x7E
    vd_uint8_t 					head_code_bytes;  // 协议头所占的字节数， 0：表示无帧头

    vd_uint8_t 					end_code[4];     // 协议尾,字符串转为整型数组保存, "7E"-->0x7E
    vd_uint8_t 					end_code_bytes;  // 协议尾所占的字节数，0：表示无帧尾

    vd_uint8_t 					len_pos;  	// 协议长度所在的位置
    vd_uint8_t 					len_bytes;  // 帧长度所占的字节数，0：表示无帧长度（如固定帧长度）
    vd_uint8_t 					len_left;   // 组成帧全长还需要的字节数，len+len_left = 帧全长
		frame_byte_order_e 	len_byte_order;//长度字节序

    vd_uint8_t 					palyload_pos;               // 有效数据起始位置
    vd_uint8_t 					palyload_before_len_bytes;  // 长度前面有多少字节的数据

    vd_char_t  					check_code_type[6];   // 校验和的类型，如sum,crc8,crc16， 不为空
    vd_uint8_t 					check_code_bytes;     // 校验和所占的字节数, 0:表示无检验和
    vd_uint8_t 					check_code_calc_pos;  // 开始校验数据的起始位置
		frame_byte_order_e 	check_code_byte_order;// 校验字节序

    // 转义相关数据
    divert_t   diverts[2];
    vd_uint8_t divert_bytes;  // 需要转义的字节数, 0：表示无需转义
} frame_t;

typedef struct {
    vd_uint8_t  is_valid : 1;         // 协议格式是否有效
    vd_uint8_t  send_frame_diff : 1;  // 发送帧格式不同于接收帧

    frame_t frame;       // 帧数据相关属性
    frame_t send_frame;  // 发送帧数据相关属性

} protocol_format_t;



/* ======================================================================================
 * declaration
 */

/**
 * @brief :  协议初始化，判断协议格式是否有效
 *
 * @param formats : 协议格式数组
 * @param array_size ： 协议格式数组大小
 * @return vd_int8_t
 */
vd_bool_t protocol_format_init(vd_uint8_t format_id);

/**
 * @brief : 获取协议格式
 *
 * @param protocol						协议
 * @return protocol_format_t*	协议格式
 */
protocol_format_t* protocol_format_get_by_id(protocol_format_e format_id);

/**
 * @brief : 打印协议配置
 *
 * @param format
 * @return vd_void_t
 */
vd_void_t protocol_format_print(protocol_format_t* format);

/**
 * @brief								数据反转义， 如：0x7D01 --> 0x7E, 0x7D02 > 0x7D
 *
 * @param format_id			格式
 * @param data 					待处理数据
 * @param len						待处理数据长度
 * @param is_match_end	是否匹配到尾帧
 * @return vd_int32_t		非负值：成功转义次数
												负值：失败
 */
vd_int32_t protocol_format_data_dedivert(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t* len) ;

/**
 * @brief : 数据顺序调整为标准顺序：帧头+长度+数据+校验和+帧尾
 *
 * @param format_id
 * @param data
 * @param len
 * @return void_t
 */
vd_void_t protocol_format_data_adjust_position(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len);

/**
 * @brief :							匹配头帧
 *
 * @param format				协议格式
 * @param data					待处理数据
 * @param len						数据长度
 * @param ret						匹配结果：true--匹配到正确头帧并返回头帧地址；false--未匹配到头帧函数返回null或匹配到疑似头帧并函数返回疑似头帧地址
 * @return vd_uint8_t *	头帧或疑似头帧在数据中的地址
 */
vd_uint8_t *protocol_format_match_headcode(protocol_format_t *format, vd_uint8_t* data, vd_uint32_t len, vd_bool_t *ret);

/**
 * @brief : 判断数据长度与帧全长是否匹配，并返回帧全长
 *
 * @param data : 读取的数据
 * @param len : 读取的数据长度
 * @param format : 协议格式化数据结构
 * @param frame_len : 帧全长
* @return vd_bool_t 结果
 */
vd_bool_t protocol_format_match_length(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t len, vd_uint32_t *frame_len);

/**
 * @brief : 计算校验和，并和数据的校验和匹配
 *
 * @param data
 * @param format
 * @return vd_bool_t
 */
vd_bool_t protocol_format_match_checkcode(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t frame_len);

/**
 * @brief : 匹配帧尾
 *
 * @param data
 * @param format
 * @return vd_int_t
 */
vd_bool_t protocol_format_match_endcode(protocol_format_t* format, vd_uint8_t* data, vd_uint32_t frame_len);

/**
 * @brief :协议格式化，加头加尾加校验和，封装palyload,组成一条完整的命令
 *
 * @param data: 需要格式化的数据
 * @param len: 需要格式化的数据长度
 * @param write_buf: 格式化之后的数据缓存
 * @param buf_size: 格式化之后的数据缓存大小
 * @return vd_uint32_t: 格式化之后的数据长度
 */
vd_uint32_t protocol_format(protocol_format_t* format, vd_uint8_t *data, vd_uint16_t len, vd_uint8_t* write_buf, vd_uint32_t buf_size);

/**
 * @brief : 完整指令去格式化，去掉转义，帧头帧尾长度校验和
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
