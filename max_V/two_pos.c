#include "stdlib.h"

#define min(a,b)	(a<=b?a:b)
#define max(a,b)	(a>=b?a:b)	

int maxArea(int* height, int heightSize)
{
	int left=0,right=0;
	int Vmax=0;
	int v=0;
	left=0;
	right=heightSize-1;
	
	for(;left<right;)
	{
		v=(right-left)*min(height[right],height[left]);
		Vmax=max(v,Vmax);
		if(height[left]<=height[right])
		{
			left++;
		}
		else
		{
			right--;
		}
	
	}
	return Vmax;
}


int main(void)
{
	int buf[]={
		1,8,6,2,5,4,8,3,7
	};
	int len=0;
	int v=0;
	
	len=sizeof(buf)/sizeof(buf[0]);
	
	v=maxArea(buf,len);
	
	printf("maxv=%d \n",v);
	
}