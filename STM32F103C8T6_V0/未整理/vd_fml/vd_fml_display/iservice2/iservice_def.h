#ifndef __ISERVICE_DEF_H
#define __ISERVICE_DEF_H


/* 协议驱动端口读取数据方式枚举 */
typedef enum {
	READ_METHOD_POLL = 0x00,   	// 轮询的方式读数据
	READ_METHOD_INT,    				// 中断方式
	READ_METHOD_MAX,
}read_method_e;

//协议格式枚举
typedef enum {
    PROTOCOL_FORMAT_FJ = 0,  // 发掘标准协议格式
    PROTOCOL_FORMAT_HMJD,    // 华美俊达协议
    PROTOCOL_FORMAT_ISDN,    //一线通信协议
    PROTOCOL_FORMAT_BLE,    //BLE通信协议
		PROTOCOL_FORMAT_MAX,
} protocol_format_e;  // 协议格式

#endif  // ISERVICE_CONFIG_H