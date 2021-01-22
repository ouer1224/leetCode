
#include "stdlib.h"
#include "stdio.h"

#define swap(a,b)	do{a=a^b;b=a^b;a=a^b;}while(0);


/***********************************************
*fun	:插入排序,小->大
*namespace:
*var	:
*return:
************************************************/

int shellSort(int *buf,int len)
{

	int i=0;
	int j=0;
	int h=0;
	
	h=len/3+1;
	
	for(;h>=1;h=h/3)
	{
		for(i=h;i<len;i++)
		{
			for(j=i;j>=h;j-=h)
			{
				if(buf[j]<buf[j-h])
				{
					swap(buf[j],buf[j-h]);
				
				}
				else
				{
					break;
				}
				
			}		
		}	
	}

	
	return 1;
}



/***********************************************
*fun	:插入排序,小->大
*namespace:
*var	:
*return:
************************************************/

int insertSort(int *buf,int len)
{
	int i=0,pos=0;
	int tmp=0;
	
	for(i=1;i<len;i++)
	{
		tmp=buf[i];
		for(pos=i-1;pos>=0;pos--)/*下面的向后移动逻辑,可以修改为swap的逻辑.这样就不用中间变量了*/
		{
			if(buf[pos]>tmp)
			{
				buf[pos+1]=buf[pos];	
			}
			else
			{
				break;
			}
		}
		buf[pos+1]=tmp;
	}
	
	
	return 1;
}


int main(void)
{
	int seed=time(NULL);
	int i=0;
	int buf[128];
	int buf2[128];
	int len=100;
	srand(seed);
	
	for(i=0;i<len;i++)
	{
		buf[i]=rand()%100;
		buf2[i]=buf[i];
	}
	for(i=0;i<len;i++)
	{
		printf(" %d ",buf[i]);
	}	
	puts("\n");
	shellSort(buf,len);
	insertSort(buf2,len);
	printf("shell==\n");
	for(i=0;i<len;i++)
	{
		printf(" %d ",buf[i]);
	}
	puts("\ninser=\n");
	for(i=0;i<len;i++)
	{
		printf(" %d ",buf2[i]);
	}
	
	
	return 1;
}