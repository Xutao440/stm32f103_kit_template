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
    unsigned char *p = (unsigned char*)&num + 3;//指针p先指向float的最高字节
    float res;//验证float拆分为4个字节后，重组为float的结果
    //先打印一下传入的float的值
	
   

    //获取对应的4个字节，从低位到高位，这时就可以用于hex格式的数据传输了
    tbuf[0] = *(p-3);
    tbuf[1] = *(p-2);
    tbuf[2] = *(p-1);
    tbuf[3] = *p;
		//log_debug("float:[%f]-[%x,%x,%x,%x]\r\n", num, tbuf[0], tbuf[1], tbuf[2], tbuf[3]);
		#if 0
    //打印看一下
    log_debug("\r\n查看float的每个字节内容(16进制):\r\n");
    

    //对拆分后的4个字节进行重组，模拟接收到hex后的数据还原过程
    unsigned char *pp = (unsigned char*)&res;
    pp[0] = tbuf[0];
    pp[1] = tbuf[1];  
    pp[2] = tbuf[2];
    pp[3] = tbuf[3];

    printf("\r\n重组后的float的值：%f\r\n", res);
		#endif
    memcpy(recdata,tbuf,4);
}
void int_to_4hex(int num,unsigned char *recdata,char len)
{
    unsigned char tbuf[4];
    unsigned char *p = (unsigned char*)&num + 3;//指针p先指向int的最高字节
    int res;//验证int拆分为4个字节后，重组为float的结果

    //先打印一下传入的int的值
    //printf("\r\n传入的int的值：%d", num);

    //获取对应的4个字节，从低位到高位，这时就可以用于hex格式的数据传输了
    tbuf[0] = *(p - 3);
    tbuf[1] = *(p - 2);
    tbuf[2] = *(p - 1);
    tbuf[3] = *p;
		memcpy(recdata,tbuf,len);
    //打印看一下
    //printf("\r\n查看int的每个字节内容(16进制):\r\n");
    //printf("%x,%x,%x,%x\r\n", tbuf[0], tbuf[1], tbuf[2], tbuf[3]);

    //对拆分后的4个字节进行重组，模拟接收到hex后的数据还原过程
//    unsigned char *pp = (unsigned char*)&res;
//    pp[0] = tbuf[0];
//    pp[1] = tbuf[1];
//    pp[2] = tbuf[2];
//    pp[3] = tbuf[3];
//		
//    printf("\r\n重组后的int的值：%d\r\n", res);
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

char* Int2String(long num,char *str)//10进制 
{
    long i = 0;//指示填充str 
    if(num<0)//如果num为负数，将num变正 
    {
        num = -num;
        str[i++] = '-';
    } 
    //转换 
    do
    {
        str[i++] = num%10+'0';//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
        num /= 10;//去掉最低位    
    }while(num);//num不为0继续循环
    
    str[i] = '\0';

    //确定开始调整的位置 
    long j = 0;
    if(str[0]=='-')//如果有负号，负号不用调整 
    {
        j = 1;//从第二位开始调整 
        ++i;//由于有负号，所以交换的对称轴也要后移1位 
    }
    //对称交换 
    for(;j<i/2;j++)
    {
        //对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b; 
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 

    return str;//返回转换后的值 
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
	char szShow[32] = {0};  // 预计常用数值显示字符在32位之内，允许输出截断
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
