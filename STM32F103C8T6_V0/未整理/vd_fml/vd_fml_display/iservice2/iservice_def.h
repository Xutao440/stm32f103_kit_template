#ifndef __ISERVICE_DEF_H
#define __ISERVICE_DEF_H


/* Э�������˿ڶ�ȡ���ݷ�ʽö�� */
typedef enum {
	READ_METHOD_POLL = 0x00,   	// ��ѯ�ķ�ʽ������
	READ_METHOD_INT,    				// �жϷ�ʽ
	READ_METHOD_MAX,
}read_method_e;

//Э���ʽö��
typedef enum {
    PROTOCOL_FORMAT_FJ = 0,  // �����׼Э���ʽ
    PROTOCOL_FORMAT_HMJD,    // ��������Э��
    PROTOCOL_FORMAT_ISDN,    //һ��ͨ��Э��
    PROTOCOL_FORMAT_BLE,    //BLEͨ��Э��
		PROTOCOL_FORMAT_MAX,
} protocol_format_e;  // Э���ʽ

#endif  // ISERVICE_CONFIG_H