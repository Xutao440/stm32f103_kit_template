#include "EncoderScan.h"

//u8  tstVar ;
typedef unsigned char u8;
u8 xcnt1;    //������1�ĳ�ʱ������
u8 ecdRcnt1; //������λ������
u8 ecdLcnt1; //������λ������

//u8 encodeEvent;		//�˱��������洢�������Ķ�������� ENC_1_R ����ض���
//�ڼ�⵽����������ʱ�����˲�������Ӧ�ò㣬 Ȼ����������˱�����

//-------------------------------------------------
// �������źų�ʱ������� Ӧ��֤0.1�����һ��
// ���ϵͳ���� ����ÿ��10ms ����һ�Σ�Ȼ���޸ĳ�ʱ�Ĵ���������ʵ�ָ��̵ĳ�ʱ����
void EncodeScanTimeOutCtrl(void)
{
    if (++xcnt1 >= 5)
    { // 5*0.1 = 0.5������û����ת��������������ʱ��
        xcnt1 = 0;
        ecdRcnt1 = 0;
        ecdLcnt1 = 0;
    }
}

/*
�˳������1�ű�������Ӳ���źŽ���ɨ��
�� EncoderScan() ��������ô˳���

�������: u8 pin  �������λ�ֱ����������ܽŵĸߵ͵�ƽ״̬

*/
u8 EncoderPinScan1(u8 pin)
{
    u8 var;
    static u8 lstVar = 0x03; //������¼��һ�εĶ˿�״̬��

    u8 x = pin & 0x03; //��ȡ�����λ(����������AB��·��ƽ״̬)
    u8 res = 0;        //encodeEvent;	//

    var = lstVar << 2;
    var |= x;
    //log_debug("EncoderPinScan1: pin 0x%x, var 0x%x\n", pin, var);

    switch (var)
    {
    case EC_S1:
        ecdRcnt1 |= BIT0;
        //log_debug("--->EC_S1\n");
        break;
    case EC_S2:
        ecdRcnt1 |= BIT1;
        //log_debug("--->EC_S2\n");
        if (ecdRcnt1 == 0x03)
        { //˳ʱ���⵽�������

            res &= ~ENC_1_MSK;
            res |= ENC_1_L;
            //encodeEvent = res; //�˴����������¼�
        }
        ecdRcnt1 = 0;
        ecdLcnt1 = 0;
        break;
    case EC_S3:
        ecdRcnt1 |= BIT2;
        //log_debug("--->EC_S3\n");
        break;
    case EC_S4:
        ecdRcnt1 |= BIT3;
        //log_debug("--->EC_S4\n");
        if (ecdRcnt1 == 0x0C)
        { //˳ʱ���⵽�������

            res &= ~ENC_1_MSK;
            res |= ENC_1_L;
            //encodeEvent = res; //�˴����������¼�
        }
        ecdRcnt1 = 0;
        ecdLcnt1 = 0;
        break;
        //---------------------------------------------------------

    case EC_N1:
        ecdLcnt1 |= BIT4;
        //log_debug("--->EC_N1\n");
        break;
    case EC_N2:
        ecdLcnt1 |= BIT5;
        //log_debug("--->EC_N2\n");
        if (ecdLcnt1 == 0x30)
        { //��ʱ���⵽�������
            res &= ~ENC_1_MSK;
            res |= ENC_1_R;
            //encodeEvent = res;	//�˴����������¼�
        }
        ecdRcnt1 = 0;
        ecdLcnt1 = 0;
        break;
    case EC_N3:
        ecdLcnt1 |= BIT6;
        //log_debug("--->EC_N3\n");
        break;
    case EC_N4:
        ecdLcnt1 |= BIT7;
        //log_debug("--->EC_N4\n");
        if (ecdLcnt1 == 0xC0)
        { //��ʱ���⵽�������
            res &= ~ENC_1_MSK;
            res |= ENC_1_R;
            //encodeEvent = res;	//�˴����������¼�
        }
        ecdRcnt1 = 0;
        ecdLcnt1 = 0;
        break;
    default:
        break;
    }

    xcnt1 = 0;
    lstVar = x;
    return res;
}

u8 encodeV1 = 0;
u8 encodeV2 = 0;
//--------------------------------------------------------
//�˳���Ӧ�ڱ�������AB��·�ź����κ�һ�������仯ʱ ����
//���AB��·�źŶ��ܲ���˫�����жϣ��������жϳ����ﶼ���ô˳���
u8 EncoderScan(unsigned int gpio_A, unsigned int gpio_B)
{
    u8 x, px;
    //u8 px;
    u8 res = 0;
    //volatile u8 pin0=0;

    //-----------------------------------------------------------------
    // ������α���룬��Ҫ���ݾ����CPU���滻����Ӧ�Ķ�ȡ�˿ڵ����
    //    pin0 = GPIO_GET_PIN(PORTA);
    //-------------------------------------------------
    encodeV1 = (u8)((gpio_A << 1) | gpio_B);
    //px = (u8)((gpio_A<<1) | gpio_B);

    //-----------------------------------
    x = encodeV1 ^ encodeV2;

    if (x & (PIN_ENC1A | PIN_ENC1B))
    {
        px = (encodeV1 & PIN_ENC1A);
        px |= (encodeV1 & PIN_ENC1B);
        res = EncoderPinScan1(px);
    }

    encodeV2 = encodeV1;
    //--------------------------
    return res;
}
