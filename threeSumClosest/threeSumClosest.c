

#include "stdlib.h"

#define abs(a)	(((a)>0)?(a):(-(a)))

int cmpfunc(const void * a,const void *b)
{
	return *((int *)a)-*((int*)b);
}

int twoSumClosest(int* nums, int numsSize, int target)
{
	int lval=-65536,rval=65536;
	int i=0;
	int val=0;
	int left=0;
	int right=numsSize-1;
	
	for(;left<right;)
	{
		val=nums[left]+nums[right];
		if(val==target)
		{
			return val;
		}
		else if(val<target)
		{
			if(val>lval)
			{
				lval=val;
			}
			left++;
		}			
		else
		{
			if(rval>val)
			{
				rval=val;
			}
			right--;
		}
	}
	printf("lva=%d rva=%d\n",lval,rval);
	if(abs(lval-target)<abs(rval-target))
	{
		return lval;
	}
	else
	{
		return rval;
	}
	return 1;
}

int threeSumClosest(int* nums, int numsSize, int target)
{
	int i=0;
	int val=0;//nums[0]+nums[1]+nums[2];
	int rt=0;

	qsort(nums,numsSize,sizeof(int),cmpfunc);
	
	val=nums[0]+nums[1]+nums[2];
	
	for(i=0;i<numsSize;i++)
	{
		printf("%d ",nums[i]);
	}
	puts("\n------\n");
	for(i=0;i<numsSize-2;i++)
	{
		rt=twoSumClosest(nums+i+1,numsSize-i-1,target-nums[i]);
		if(abs(val-target)>abs(rt+nums[i]-target))
		{
			val=rt+nums[i];
		}
			
	}

	return val;
}



int main(void)
{
	int buf[]={1,6,9,14,16,70};
	int tar=81;
	
	int ret=0;
	
	ret=threeSumClosest(buf,sizeof(buf)/sizeof(buf[0]),tar);
	
	printf("ret=%d\n",ret);
	
	
}
