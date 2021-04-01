

#include <stdio.h>
#include <stdarg.h>

#ifndef uint8_t 
typedef unsigned char uint8_t;
#endif

#ifndef uint32_t 
typedef unsigned int uint32_t;
#endif

#define USED	1
#define NOUSED	0

typedef struct 
{
	uint8_t * start;
	uint32_t len;
	uint8_t state;
}MsgOutCB;






#define MaxStringLen		256
#define MaxNumReenterMsg	12
static uint8_t s_buf[MaxStringLen];

static MsgOutCB s_bufMCB[MaxNumReenterMsg];

uint8_t os_puts(uint8_t *str,uint32_t len)
{
	puts(str);
}


uint8_t * msg_malloc(uint32_t len)
{
	uint8_t *pr=NULL;
	uint32_t i=0;
	uint32_t j=0;
	MsgOutCB tmp={NULL,0,0};
	uint32_t size_avali=0;
	uint32_t pr_avali=0;
	uint32_t pr_pre_addr=0;
	uint32_t tmp_size=0;
	
	for(i=0;i<MaxNumReenterMsg;i++)
	{
		if((s_bufMCB[i].state)==NOUSED)
		{
			s_bufMCB[i].start=NULL;
		}
	}

	/*bin sort*/
	for(i=1;i<MaxNumReenterMsg;i++)
	{
		for(j=i;j>0;j--)
		{
			if((uint32_t)(s_bufMCB[j].start)<(uint32_t)(s_bufMCB[j-1].start))
			{
				tmp=s_bufMCB[j-1];
				s_bufMCB[j-1]=s_bufMCB[j];
				s_bufMCB[j]=tmp;
			}
			else
			{
				continue;
			}
		}
	}

	for(i=0;i<MaxNumReenterMsg;i++)
	{
		if(s_bufMCB[i].state!=NOUSED)
		{
			break;
		}
	}

	pr_pre_addr=(uint32_t)(s_buf+MaxNumReenterMsg);
	size_avali=MaxNumReenterMsg+1;
	for(j=MaxNumReenterMsg-1;j>i;j--)
	{
		tmp_size=(pr_pre_addr-(uint32_t)(s_bufMCB[j].start)-s_bufMCB[j].len);
		if((tmp_size<size_avali)&&(tmp_size>=len))
		{
			pr_avali=(uint32_t)(s_bufMCB[j].start);
			size_avali=tmp_size;
		}
		pr_pre_addr=(uint32_t)(s_bufMCB[j].start);
	}
	if(tmp_size>=len)
	{
		pr=pr_avali;
	}
	else
	{
		pr=NULL;
	}

	return  pr;
}

#undef USED
#undef NOUSED








uint32_t os_sprintf_x(uint32_t d,uint8_t *buf)
{
	uint32_t i=0;
	uint32_t len=0;
	uint32_t tmp=0;
	tmp=d;
	do
	{
		i++;
		tmp=tmp>>4;
	}while(tmp>0);

	len=i;
	do
	{
		i--;
		tmp=(d&0x0f);
		if(tmp<10)
		{
			buf[i]='0'+tmp;
		}
		else
		{
			buf[i]=tmp-10+'a';
		}
		d=d>>4;
	}while(d>0);

	return len;
}

uint32_t os_sprintf_d(uint32_t d,uint8_t *buf)
{
	uint32_t i=0;
	uint32_t len=0;
	uint32_t tmp=0;
	tmp=d;
	do
	{
		i++;
		tmp=tmp/10;
	}while(tmp>0);

	len=i;
		
	do
	{
		i--;
		tmp=(d%10);
		if(tmp<10)
		{
			buf[i]='0'+tmp;
		}
		else
		{
			buf[i]=tmp-10+'a';
		}
		
		
		d=d/10;
	}while(d>0);

	return len;
}
uint32_t os_sprintf_c(uint32_t c,uint8_t *buf)
{
	buf[0]=c;
	return 1;
}




uint32_t os_sprintf_f(double f,uint8_t *buf)
{
	uint32_t i=0;
	uint32_t len=0;
	uint32_t tmp=0;
	
	tmp=f;
	len=os_sprintf_d(tmp,buf);
	f=f-tmp;
	tmp=f*1000;
	buf[len]='.';
	len++;
	len=len+os_sprintf_d(tmp,buf+len);

	return len;
}


uint32_t os_printf(uint8_t *str , ...)
{
	va_list ap;
	uint32_t count=0;
	uint32_t len=0;
	uint32_t val_d=0;
	double val_f=0;
	uint8_t *pr_buf=s_buf;
	
	va_start(ap,str);
	while(count<MaxStringLen)
	{
		count++;
		if(*str==0)
		{
			goto label_exit;
		}
		*pr_buf=*str;

		if(*str=='%')
		{
			
			str++;	
			switch(*str)
			{
				case 'c':
				{
					val_d=va_arg(ap,int);
					//printf("arg= d =%d\n",val_d);
					len=os_sprintf_c(val_d,pr_buf);
					pr_buf+=len-1;
				}
				break;
				case 'd':
				{
					val_d=va_arg(ap,int);
					//printf("arg= d =%d\n",val_d);
					len=os_sprintf_d(val_d,pr_buf);
					pr_buf+=len-1;
				}
				break;
				
				case 'x':
				case 'X':
				{
					val_d=va_arg(ap,int);
					//printf("arg= d =0x%x\n",val_d);
					len=os_sprintf_x(val_d,pr_buf);
					pr_buf+=len-1;
				}
				break;
				case 'f':
				{
					val_f=va_arg(ap,double);
					//printf("arg= d =%f\n",val_f);
					len=os_sprintf_f(val_f,pr_buf);
					pr_buf+=len-1;
				}
				break;
				
				default:
				break;
		
			}			
		}
		
		pr_buf++;
		str++;

	}

label_exit:
	va_end(ap);
	
	len=(size_t)pr_buf-(size_t)s_buf;
	s_buf[len]=0;
	
	
	os_puts(s_buf,len);
	
	return len;
}







int main(int argc, char *argv[])
{
	uint32_t i=0,len=0;
	uint8_t buf[24];
	
#if 0	
	len=os_sprintf_f(1200034567.8912,buf);
	for(i=0;i<len;i++)
	{
		printf("len=%d buf=%c\n",len,buf[i]);
	}
	return 1;
	len=os_sprintf_d(456789012,buf);
	for(i=0;i<len;i++)
	{
		printf("len=%d buf=%c\n",len,buf[i]);
	}	
	return 1;
#endif	
	
	for(i=0;i<32;i++)
	{
		len=printf("qwe %d qwe %f qwe=0x%x  %c\n\n",i,1.981,i*10,i+0x30);
	}
	return 0;
}




