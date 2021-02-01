/*
	使用二分法进行处理时,绕不开的似乎就是栈的使用.
	因此本文件也需要创建一个简单的栈操作,供二分法使用.
	
*/


#include "stdio.h"
#include "stdlib.h"
#include "time.h"
int swap(void *a,void *b,int size)
{
	char tmp=0;
	int i=0;
	int j=0;
	i=*((int *)a);
	j=*((int *)b);
	while(size>=1)
	{
		size--;
		tmp=*((char *)a);
		*((char *)a)=*((char *)b);
		*((char *)b)=tmp;
		a++;
		b++;
	}
	return 1;
}
int funcmp(void *a,void *b)
{
	int ret=0;
	int *x=a;
	int *y=b;
	ret=*x-*y;
	return ret;
}
#define valpit	0
#define p_bufi(i)	(buf+i*size)
int quickSort(void *buf,int len,int size,int (*fun)(void *,void *))
{
	int ret=0;
	int left=0,right=len;
	int mvdirection='r';
	void * tmp=malloc(size);
	printf("\nstep0\n");
	memcpy(tmp,buf+0,size);
	memset(buf+0,valpit,size);
	printf("step1\n");
	for(;left<right;)
	{
		printf("step2");
		if(mvdirection=='r')
		{
			right--;
			ret=fun(tmp,p_bufi(right));
			if(ret>0)
			{
				memcpy(p_bufi(left),p_bufi(right),size);
				memset(p_bufi(right),valpit,size);
				mvdirection='l';
			}
			else
			{
			}
		}
		else
		{
			left++;
			ret=fun(tmp,p_bufi(left));
			if(ret>0)
			{
			}
			else
			{
				mvdirection='r';
				memcpy(p_bufi(right),p_bufi(left),size);
				memset(p_bufi(left),valpit,size);			
			}
		}
	}
	memcpy(p_bufi(left),tmp,size);
	free(tmp);
	return 1;
}


/***********************************************
*fun:二分法查找,找到数组中第一个大于val的数据的位置.
*namespace:
*var:	nums为有序数据,小-->大
*return:
************************************************/

int binFind(int *nums,int len,int val)
{
	int left=0;
	int right=len;	/*整体依然使用左闭右开的区间*/
	int mid=0;
	
	while(left<right)
	{
		mid=(left+right)/2;
		
		//printf("");
		
		if(val>nums[mid])
		{
			left=mid+1;
		}
		else
		{
			right=mid;
		}
		
	}
	
	return left;
}


/***********************************************
*fun:	这是通过二分法查找的方式,在已有的顺序数据中,找到新数据的位置,然后将其放入到该位置.
*namespace:
*var	:
*return:
************************************************/

void binSort(int a[], int len)
{
	int i = 0, j = 0;
	int x=0;

	for (i = 1; i <= len - 1; i++)
	//for(i=1;i<3;i++)
	{
		int left = 0;
		int right = i - 1;
		int temp1 = a[i];


		if (temp1 >= a[right])
		{
			continue;
		}


		while (left <= right)
		{

			int mid = (left + right) / 2;

			if (temp1 < a[mid])
			{
				right = mid - 1;
			}
			else 
			{
				left = mid + 1;
			}
		}

		for (j = i - 1; j >= left; j--)
		{
			a[j + 1] = a[j];
		}

		a[left] = temp1;
		
		
		printf("\n count = %d  tmp=%d \n",i,temp1);
		for(x=0;x<10;x++)
		{
			printf("%d ",a[x]);
		}
		
	}
}








int main(void)
{
	int seed=0;
	int i=0;
	int buf[12];
	int pos=0;

	for(i=0;i<10;i++)
	{
		buf[i]=rand()%100;
		printf("%d ",buf[i]);
	}	
#if 0		
	puts("\nbegin swap\n");
	for(i=1;i<10;i++)
	{
		printf("before %d=%d,%d=%d \n",i,buf[i],i-1,buf[i-1]);
		swap(buf+i,buf+i-1,sizeof(int));
		printf("after %d=%d,%d=%d \n\n",i,buf[i],i-1,buf[i-1]);
	}	
	printf("\n after swap \n");
#endif	
	//binSort(buf,10,sizeof(int),funcmp);
	binSort(buf,10);
	
	printf("\n after sort \n");
	for(i=0;i<10;i++)
	{
		printf("%d ",buf[i]);
	}
	
	pos=binFind(buf,10,-1);
	
	printf("\npos=%d\n",pos);	
	
	return 1;
}