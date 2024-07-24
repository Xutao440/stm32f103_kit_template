#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


#if LOG_ENABLE

vd_uint8_t vd_g_usart_num = 0;

//log��ʼ��
void log_init(VD_USARTx_E USARTx)
{
	vd_bsp_usart_init(USARTx);
	vd_g_usart_num = USARTx;
}



//�������ݷ���
void log_send(char* buff)
{
	if(buff == NULL){
		return ;
	}
	vd_bsp_usart_SendString((VD_USARTx_E)vd_g_usart_num, (char*)buff);              //���ڷ���һ���ַ���
	
}

//����ת��Ϊ�ַ���
char* log_itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//������
    unsigned unum;//���Ҫת���������ľ���ֵ,ת�������������Ǹ���
    int i=0,j,k;//i����ָʾ�����ַ�����Ӧλ��ת��֮��i��ʵ�����ַ����ĳ��ȣ�ת����˳��������ģ��������������k����ָʾ����˳��Ŀ�ʼλ��;j����ָʾ����˳��ʱ�Ľ�����
 
    //��ȡҪת���������ľ���ֵ
    if(radix==10&&num<0)//Ҫת����ʮ�����������Ǹ���
    {
        unum=(unsigned)-num;//��num�ľ���ֵ����unum
        str[i++]='-';//���ַ�����ǰ������Ϊ'-'�ţ�����������1
    }
    else unum=(unsigned)num;//����numΪ����ֱ�Ӹ�ֵ��unum
 
    //ת�����֣�ע��ת�����������
    do
    {
        str[i++]=index[unum%(unsigned)radix];//ȡunum�����һλ��������Ϊstr��Ӧλ��ָʾ������1
        unum/=radix;//unumȥ�����һλ
 
    }while(unum);//ֱ��unumΪ0�˳�ѭ��
 
    str[i]='\0';//���ַ���������'\0'�ַ���c�����ַ�����'\0'������
 
    //��˳���������
    if(str[0]=='-') k=1;//����Ǹ��������Ų��õ������ӷ��ź��濪ʼ����
    else k=0;//���Ǹ�����ȫ����Ҫ����
 
    char temp;//��ʱ��������������ֵʱ�õ�
    for(j=k;j<=(i-1)/2;j++)//ͷβһһ�Գƽ�����i��ʵ�����ַ����ĳ��ȣ��������ֵ�ȳ�����1
    {
        temp=str[j];//ͷ����ֵ����ʱ����
        str[j]=str[i-1+k-j];//β����ֵ��ͷ��
        str[i-1+k-j]=temp;//����ʱ������ֵ(��ʵ����֮ǰ��ͷ��ֵ)����β��
    }
 
    return str;//����ת������ַ���
 
}

////log�ַ�������
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

//���ַ������ַ���
static signed int PutChar(char *pStr, char c)
{
    *pStr = c;
    return 1;
}

//���ַ��������ַ���
static signed int PutString(char *pStr, const char *pSource)
{
    signed int num = 0;
 
    while (*pSource != 0) {
 
        *pStr++ = *pSource++;
        num++;
    }
 
    return num;
}

//��hexתΪ�ַ���
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

//�޷�������ת�ַ���
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

//�з�������ת�ַ���
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

//��ʽ��ת������
static signed int t_vsprintf(char *pString, const char *pFormat, va_list ap)
{
    return t_vsnprintf(pString, MAX_STRING_SIZE, pFormat, ap);
}

#endif

//����log������
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

