#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


#if LOG_ENABLE

vd_uint8_t vd_g_usart_num = 0;

//log初始化
void log_init(VD_USARTx_E USARTx)
{
	vd_bsp_usart_init(USARTx);
	vd_g_usart_num = USARTx;
}



//串口数据发送
void log_send(char* buff)
{
	if(buff == NULL){
		return ;
	}
	vd_bsp_usart_SendString((VD_USARTx_E)vd_g_usart_num, (char*)buff);              //串口发送一个字符串
	
}

//数字转换为字符串
char* log_itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
    unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
    int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
 
    //获取要转换的整数的绝对值
    if(radix==10&&num<0)//要转换成十进制数并且是负数
    {
        unum=(unsigned)-num;//将num的绝对值赋给unum
        str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
    }
    else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
    //转换部分，注意转换后是逆序的
    do
    {
        str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
        unum/=radix;//unum去掉最后一位
 
    }while(unum);//直至unum为0退出循环
 
    str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
    //将顺序调整过来
    if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
    else k=0;//不是负数，全部都要调整
 
    char temp;//临时变量，交换两个值时用到
    for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
    {
        temp=str[j];//头部赋值给临时变量
        str[j]=str[i-1+k-j];//尾部赋值给头部
        str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
    }
 
    return str;//返回转换后的字符串
 
}

////log字符串拷贝
//static char* log_strcpy(char* dest, const char* str)
//{
//	uint8_t len = 0;
//	while(str[len] != '\0'){		
//		dest[len] = str[len];
//		len++;
//	}
//	
//	return dest+len;	
//}

//将字符放入字符串
static signed int PutChar(char *pStr, char c)
{
    *pStr = c;
    return 1;
}

//将字符串放入字符串
static signed int PutString(char *pStr, const char *pSource)
{
    signed int num = 0;
 
    while (*pSource != 0) {
 
        *pStr++ = *pSource++;
        num++;
    }
 
    return num;
}

//将hex转为字符串
static signed int PutHexa(char *pStr, char fill, signed int width, unsigned char maj, unsigned int value)
{
    signed int num = 0;
 
    // Decrement width
    width--;
 
    // Recursively output upper digits
    if ((value >> 4) > 0) {
 
        num += PutHexa(pStr, fill, width, maj, value >> 4);
        pStr += num;
    }
    // Write filler chars
    else {
 
        while (width > 0) {
 
            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }
    }
 
    // Write current digit
    if ((value & 0xF) < 10) {
 
        PutChar(pStr, (value & 0xF) + '0');
    }
    else if (maj) {
 
        PutChar(pStr, (value & 0xF) - 10 + 'A');
    }
    else {
 
        PutChar(pStr, (value & 0xF) - 10 + 'a');
    }
    num++;
 
    return num;
}

//无符号整型转字符串
static signed int PutUnsignedInt(char *pStr, char fill, signed int width, unsigned int value)
{
    signed int num = 0;
 
    // Take current digit into account when calculating width
    width--;
 
    // Recursively write upper digits
    if ((value / 10) > 0) {
 
        num = PutUnsignedInt(pStr, fill, width, value / 10);
        pStr += num;
    }
    // Write filler characters
    else {
 
        while (width > 0) {
 
            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }
    }
 
    // Write lower digit
    num += PutChar(pStr, (value % 10) + '0');
 
    return num;
}

//有符号整型转字符串
static signed int PutSignedInt(char *pStr, char fill, signed int width, signed int value)
{
    signed int num = 0;
    unsigned int absolute;
 
    // Compute absolute value
    if (value < 0) {
 
        absolute = -value;
    }
    else {
 
        absolute = value;
    }
 
    // Take current digit into account when calculating width
    width--;
 
    // Recursively write upper digits
    if ((absolute / 10) > 0) {
 
        if (value < 0) {
        
            num = PutSignedInt(pStr, fill, width, -(absolute / 10));
        }
        else {
 
            num = PutSignedInt(pStr, fill, width, absolute / 10);
        }
        pStr += num;
    }
    else {
 
        // Reserve space for sign
        if (value < 0) {
 
            width--;
        }
 
        // Write filler characters
        while (width > 0) {
 
            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }
 
        // Write sign
        if (value < 0) {
 
            num += PutChar(pStr, '-');
            pStr++;
        }
    }
 
    // Write lower digit
    num += PutChar(pStr, (absolute % 10) + '0');
 
    return num;
}



static signed int t_vsnprintf(char *pStr, unsigned int length, const char *pFormat, va_list ap)
{
    char          fill;
    unsigned char width;
    signed int    num = 0;
    signed int    size = 0;
 
    // Clear the string
    if (pStr) {
 
        *pStr = 0;
    }
 
    // Phase string
    while (*pFormat != 0 && size < length) {
 
        // Normal character
        if (*pFormat != '%') {
 
            *pStr++ = *pFormat++;
            size++;
        }
        // Escaped '%'
        else if (*(pFormat+1) == '%') {
 
            *pStr++ = '%';
            pFormat += 2;
            size++;
        }
        // Token delimiter
        else {
 
            fill = ' ';
            width = 0;
            pFormat++;
 
            // Parse filler
            if (*pFormat == '0') {
 
                fill = '0';
                pFormat++;
            }
 
            // Parse width
            while ((*pFormat >= '0') && (*pFormat <= '9')) {
        
                width = (width*10) + *pFormat-'0';
                pFormat++;
            }
 
            // Check if there is enough space
            if (size + width > length) {
 
                width = length - size;
            }
        
            // Parse type
            switch (*pFormat) {
            case 'd': num = PutUnsignedInt(pStr, fill, width, va_arg(ap, unsigned int)); break;
            case 'i': num = PutSignedInt(pStr, fill, width, va_arg(ap, signed int)); break;
            case 'u': num = PutUnsignedInt(pStr, fill, width, va_arg(ap, unsigned int)); break;
            case 'x': num = PutHexa(pStr, fill, width, 0, va_arg(ap, unsigned int)); break;
            case 'X': num = PutHexa(pStr, fill, width, 1, va_arg(ap, unsigned int)); break;
            case 's': num = PutString(pStr, va_arg(ap, char *)); break;
            case 'c': num = PutChar(pStr, va_arg(ap, unsigned int)); break;
            default:
                return -1;  //EOF
            }
 
            pFormat++;
            pStr += num;
            size += num;
        }
    }
 
    // NULL-terminated (final \0 is not counted)
    if (size < length) {
 
        *pStr = 0;
    }
    else {
 
        *(--pStr) = 0;
        size--;
    }
 
    return size;
}

//格式化转换函数
static signed int t_vsprintf(char *pString, const char *pFormat, va_list ap)
{
    return t_vsnprintf(pString, MAX_STRING_SIZE, pFormat, ap);
}

#endif

//错误log处理函数
void log_printf(char* format, ...)
{
	#if LOG_ENABLE
	va_list arg;
	char string[MAX_STRING_SIZE] = {0};
	
	va_start(arg, format);
	t_vsprintf(string, format, arg);
	va_end(arg);
	
	log_send(string);
	#endif

}

