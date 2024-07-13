#include "dt_log.h"
//#include "stdarg.h"
#include "vdiscoveryConfig.h"
#if USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#elif USE_BLOS

#endif
typedef uint32_t TickType_t;
unsigned int usleep(useconds_t useconds)
{
	#if USE_SWM320
		TickType_t t = (TickType_t)((uint64_t)useconds / (1000 * portTICK_PERIOD_MS));
    useconds_t r = useconds - ((useconds_t)t * (1000 * portTICK_PERIOD_MS));
	#else
	TickType_t t = useconds;
	#endif
    

    if (t > 0){
			#if USE_SWM320
		 vTaskDelay(t);
			#else
			dt_delay_us(t);
			#endif
		}
       

    //if (r > 0)
    //    ithDelay(r);

    return 0;
}

unsigned int msleep(useconds_t mseconds)
{
    if (mseconds > 0)
        usleep((uint64_t)mseconds * 1000);

    return 0;
}

unsigned int sleep(unsigned seconds)
{
    if (seconds > 0)
        usleep((uint64_t)seconds * 1000000);
    else
			#if USE_FREERTOS
        taskYIELD();
			#else
			#endif

    return 0;
}

// Note
//      Get current time
//      Add by meizhen 20200806
u32 now(void)
{
	#if USE_FREERTOS
    return xTaskGetTickCount();
	#elif USE_BLOS
	return rf_get_sysTick();
	#endif
}

void system_delay(volatile unsigned int delay)
{
    unsigned int tickstart = 0;
    tickstart = now();
    while ((now() - tickstart) < delay)
    {
        ;
    }
}

//void delay_ms(uint32_t ms)
//{	
//	uint32_t t_tick = now();
//	while((now() - t_tick) < ms);
//}
//uint8_t delay_ms_exe(uint32_t last_tick,uint32_t ms)
//{
//	return (now()-last_tick) > ms;
//}

void float_to_4hex(float num,unsigned char *recdata)
{
    unsigned char tbuf[4];
    unsigned char *p = (unsigned char*)&num + 3;//ָ��p��ָ��float������ֽ�
    float res;//��֤float���Ϊ4���ֽں�����Ϊfloat�Ľ��
    //�ȴ�ӡһ�´����float��ֵ
	
   

    //��ȡ��Ӧ��4���ֽڣ��ӵ�λ����λ����ʱ�Ϳ�������hex��ʽ�����ݴ�����
    tbuf[0] = *(p-3);
    tbuf[1] = *(p-2);
    tbuf[2] = *(p-1);
    tbuf[3] = *p;
		//log_debug("float:[%f]-[%x,%x,%x,%x]\r\n", num, tbuf[0], tbuf[1], tbuf[2], tbuf[3]);
		#if 0
    //��ӡ��һ��
    log_debug("\r\n�鿴float��ÿ���ֽ�����(16����):\r\n");
    

    //�Բ�ֺ��4���ֽڽ������飬ģ����յ�hex������ݻ�ԭ����
    unsigned char *pp = (unsigned char*)&res;
    pp[0] = tbuf[0];
    pp[1] = tbuf[1];  
    pp[2] = tbuf[2];
    pp[3] = tbuf[3];

    printf("\r\n������float��ֵ��%f\r\n", res);
		#endif
    memcpy(recdata,tbuf,4);
}
void int_to_4hex(int num,unsigned char *recdata,char len)
{
    unsigned char tbuf[4];
    unsigned char *p = (unsigned char*)&num + 3;//ָ��p��ָ��int������ֽ�
    int res;//��֤int���Ϊ4���ֽں�����Ϊfloat�Ľ��

    //�ȴ�ӡһ�´����int��ֵ
    //printf("\r\n�����int��ֵ��%d", num);

    //��ȡ��Ӧ��4���ֽڣ��ӵ�λ����λ����ʱ�Ϳ�������hex��ʽ�����ݴ�����
    tbuf[0] = *(p - 3);
    tbuf[1] = *(p - 2);
    tbuf[2] = *(p - 1);
    tbuf[3] = *p;
		memcpy(recdata,tbuf,len);
    //��ӡ��һ��
    //printf("\r\n�鿴int��ÿ���ֽ�����(16����):\r\n");
    //printf("%x,%x,%x,%x\r\n", tbuf[0], tbuf[1], tbuf[2], tbuf[3]);

    //�Բ�ֺ��4���ֽڽ������飬ģ����յ�hex������ݻ�ԭ����
//    unsigned char *pp = (unsigned char*)&res;
//    pp[0] = tbuf[0];
//    pp[1] = tbuf[1];
//    pp[2] = tbuf[2];
//    pp[3] = tbuf[3];
//		
//    printf("\r\n������int��ֵ��%d\r\n", res);
}
#if 0
void __putchar__ (char s) {
	unsigned char UART_CH_SEL=2;
	switch(UART_CH_SEL){
	case 0:
		//UARTTxByte(USART0,s);
		//USART_TxByte(s);
		break;
	case 1:
		//UARTTxByte(UART1,s);
		break;
	case 2:
//		mcu_app_uart_sendcmd(s);//uart0
		break;
	default:
		break;
	}
}

char *myitoa(int value, char* string, int radix)
{

		char tmp[33];
		char* tp = tmp;
		int i;
		unsigned v;
		int sign;
		char* sp;
		
		if (radix > 36 || radix <= 1)
		{
			return 0;
		}
		
		sign = (radix == 10 && value < 0);
		if (sign)
		v = -value;
		else
		v = (unsigned)value;
		while (v || tp == tmp)
		{
			i = v % radix;
			v = v / radix;
			if (i < 10)
			*tp++ = i+'0';
			else
			*tp++ = i + 'a' - 10;
		}
		
		sp = string;
		
		if (sign)
		*sp++ = '-';
		while (tp > tmp)
		*sp++ = *--tp;
		*sp = 0;
		return string;
}

char* Int2String(long num,char *str)//10���� 
{
    long i = 0;//ָʾ���str 
    if(num<0)//���numΪ��������num���� 
    {
        num = -num;
        str[i++] = '-';
    } 
    //ת�� 
    do
    {
        str[i++] = num%10+'0';//ȡnum���λ �ַ�0~9��ASCII����48~57������˵����0+48=48��ASCII���Ӧ�ַ�'0' 
        num /= 10;//ȥ�����λ    
    }while(num);//num��Ϊ0����ѭ��
    
    str[i] = '\0';

    //ȷ����ʼ������λ�� 
    long j = 0;
    if(str[0]=='-')//����и��ţ����Ų��õ��� 
    {
        j = 1;//�ӵڶ�λ��ʼ���� 
        ++i;//�����и��ţ����Խ����ĶԳ���ҲҪ����1λ 
    }
    //�Գƽ��� 
    for(;j<i/2;j++)
    {
        //�Գƽ������˵�ֵ ��ʵ����ʡ���м��������a+b��ֵ��a=a+b;b=a-b;a=a-b; 
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 

    return str;//����ת�����ֵ 
}


void my_printf__(const char *fmt, ...)
{
//#if DEBUG_LOG
    const char *s;
   int d;
    char ch, *pbuf, buf[16];
    va_list ap;
    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt != '%') {
              __putchar__(*fmt++);
            continue;
        }
        switch (*++fmt) {
            case 's':
                s = va_arg(ap, const char *);
                for ( ; *s; s++) {
                 __putchar__(*s);
                }
                break;
            case 'd':
                d = va_arg(ap, int);
                myitoa(d, buf, 10);
                for (s = buf; *s; s++) {
                 __putchar__(*s);
                }
                break;             
            case 'x':
            case 'X':
                d = va_arg(ap, int);
                myitoa(d, buf, 16);
                for (s = buf; *s; s++) {
                 __putchar__(*s);
                }
                break;                
            // Add other specifiers here...
            case 'c':
            case 'C':
            	ch = (unsigned char)va_arg(ap, int);
        	    pbuf = &ch;
            	__putchar__(*pbuf);
                break;
            default:  
                __putchar__(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
//#endif
}

#if !USE_STD_PRINTF
int fputc(int ch, FILE *f)
{
	//UARTTxByte(UART1,(unsigned char)ch);
	__putchar__ ((char)ch);
	return ch;
}
#endif 


void putchar2buf(char *str,char s) 
{
	*str = s;
	//USART_TxByte(s);
}

int msnprintf(char *str, int size, const char *fmt, ...)
{
	const char *s;
    int d;
	long long l;
    char ch, *pbuf, buf[16];
	int cpy_cnt = 0;
    va_list ap;
    va_start(ap, fmt);
	size = size - 2;
    while (*fmt) {
        if (*fmt != '%') {
					putchar2buf(str,*fmt++);
					str ++;
					cpy_cnt++;
					if(cpy_cnt > size){ va_end(ap); return (-1);}
							continue;
				}
        switch (*++fmt) {
            case 's':
                s = va_arg(ap, const char *);
                for ( ; *s; s++) {
                 putchar2buf(str,*s);
								 str++;
								 cpy_cnt++;
								 if(cpy_cnt > size){ va_end(ap); return (-1);}
                }
                break;
            case 'd':
                d = va_arg(ap, int);
                myitoa(d, buf, 10);
                for (s = buf; *s; s++) {
                 putchar2buf(str,*s);
								 str++;
								 cpy_cnt++;
									if(cpy_cnt > size){ va_end(ap); return (-1);}
												}
                break;       
						case 'f':
							d = va_arg(ap, int);
							myitoa(d, buf, 10);
							for (s = buf; *s; s++) {
								 putchar2buf(str,*s);
								 str++;
								 cpy_cnt++;
								 if(cpy_cnt > size){ va_end(ap); return (-1);}
							}
							break;  												
            case 'x':
            case 'X':
                d = va_arg(ap, int);
                myitoa(d, buf, 16);
                for (s = buf; *s; s++) {
								putchar2buf(str,*s);
								str++;
								cpy_cnt++;
								if(cpy_cnt > size){ va_end(ap); return (-1);}
										}
                break;                
            // Add other specifiers here...
            case 'c':
            case 'C':
            	ch = (unsigned char)va_arg(ap, int);
        	    pbuf = &ch;
            	putchar2buf(str,*pbuf);
							str++;
							cpy_cnt++;
							if(cpy_cnt > size){ va_end(ap); return (-1);}
              break;
						case 'l':
                l = va_arg(ap, long);
								myitoa(l, buf, 10);
                for (s = buf; *s; s++) {
                 putchar2buf(str,*s);
								 str++;
								 cpy_cnt++;
									if(cpy_cnt > size){ va_end(ap); return (-1);}
								}
              break;	
            default:  
                putchar2buf(str,*fmt);
				str++;
				cpy_cnt++;
			    if(cpy_cnt > size){ va_end(ap); return (-1);}
                break;
        }
        fmt++;
    }
    va_end(ap);
	return size;
}
int printf_fl()
{
	char str[18]={0};
	snprintf(str, 18, "0123456789012345678");
	printf("[1]str = %s",str);
	double dValue = 3.1415962;
	char szShow[32] = {0};  // Ԥ�Ƴ�����ֵ��ʾ�ַ���32λ֮�ڣ���������ض�
	snprintf(szShow, sizeof(szShow), "%.8lf", dValue);
	printf("[2]szShow=%s\r\n",szShow);
	float tempe = 37.45;
	char tmep[6]={0};
	snprintf(tmep, sizeof(tmep), "%f", tempe);
	printf("[3]tmep=%s\r\n",tmep); 
	long long timeers = 1608024020000;
	char tmep23[14]={0};
	snprintf(tmep23, sizeof(tmep23), "%lld", timeers);
	printf("[4]tmep23=%s\r\n",tmep23);
}

uint8_t delay_ms_exe(uint32_t last_tick,uint32_t ms)
{
	return (xTaskGetTickCount()-last_tick) > ms;
}
void delay_ms(uint32_t ms)
{	
	uint32_t t_tick = xTaskGetTickCount();
	while((xTaskGetTickCount() - t_tick) < ms);
}

#endif
