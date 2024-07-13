#include "EncoderScan.h"

//u8  tstVar ;
typedef unsigned char u8;
u8 xcnt1;    //编码器1的超时计数器
u8 ecdRcnt1; //右旋相位计数器
u8 ecdLcnt1; //左旋相位计数器

//u8 encodeEvent;		//此变量用来存储编码器的动作，详见 ENC_1_R 等相关定义
//在检测到编码器动作时，将此参数传给应用层， 然后立即清零此变量。

//-------------------------------------------------
// 编码器信号超时处理程序， 应保证0.1秒调用一次
// 如果系统允许， 建议每隔10ms 调用一次，然后修改超时的次数，用来实现更短的超时控制
void EncodeScanTimeOutCtrl(void)
{
    if (++xcnt1 >= 5)
    { // 5*0.1 = 0.5秒钟内没有旋转编码器则视作超时。
        xcnt1 = 0;
        ecdRcnt1 = 0;
        ecdLcnt1 = 0;
    }
}

/*
此程序负责对1号编码器的硬件信号进行扫描
由 EncoderScan() 程序负责调用此程序

传入参数: u8 pin  其最低两位分别代表编码器管脚的高低电平状态

*/
u8 EncoderPinScan1(u8 pin)
{
    u8 var;
    static u8 lstVar = 0x03; //用来记录上一次的端口状态。

    u8 x = pin & 0x03; //读取最低两位(即编码器的AB两路电平状态)
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
        { //顺时针检测到半个周期

            res &= ~ENC_1_MSK;
            res |= ENC_1_L;
            //encodeEvent = res; //此处产生右旋事件
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
        { //顺时针检测到半个周期

            res &= ~ENC_1_MSK;
            res |= ENC_1_L;
            //encodeEvent = res; //此处产生右旋事件
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
        { //逆时针检测到半个周期
            res &= ~ENC_1_MSK;
            res |= ENC_1_R;
            //encodeEvent = res;	//此处产生左旋事件
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
        { //逆时针检测到半个周期
            res &= ~ENC_1_MSK;
            res |= ENC_1_R;
            //encodeEvent = res;	//此处产生左旋事件
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
//此程序应在编码器的AB两路信号中任何一个发生变化时 调用
//如果AB两路信号都能产生双边沿中断，则两个中断程序里都调用此程序
u8 EncoderScan(unsigned int gpio_A, unsigned int gpio_B)
{
    u8 x, px;
    //u8 px;
    u8 res = 0;
    //volatile u8 pin0=0;

    //-----------------------------------------------------------------
    // 这里是伪代码，需要根据具体的CPU，替换成相应的读取端口的语句
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
