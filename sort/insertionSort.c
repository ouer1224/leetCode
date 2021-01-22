
#include "stdlib.h"
#include "stdio.h"

#define swap(a,b)	do{a=a^b;b=a^b;a=a^b;}wihle(0);


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
	srand(seed);
	
	for(i=0;i<10;i++)
	{
		buf[i]=rand()%100;
	}
	for(i=0;i<10;i++)
	{
		printf(" %d ",buf[i]);
	}	
	puts("\n");
	insertSort(buf,10);
	
	for(i=0;i<10;i++)
	{
		printf(" %d ",buf[i]);
	}
	
	
	return 1;
}