#include "stdio.h"
#include "stdlib.h"

/***********************************************
*fun	:移除数组中的重复数据
*namespace:
*var	:numsSize是数组的len
*return:数组中剩余不重复的数据的个数.
************************************************/
int removeDuplicates(int* nums, int numsSize)
{
	int findDup=0;
	int arrayTowr=0;
	
	findDup=1;
	arrayTowr=1;
	
    if(numsSize==0)
    {
        return 0;
    }

	for(;findDup<numsSize;findDup++)
	{
		if(nums[findDup]!=nums[findDup-1])
		{
			if(arrayTowr!=findDup)    //当两个指针位置一样时,就不需要移动数据了.
            {
                nums[arrayTowr]=nums[findDup];
            }
			
			arrayTowr++;
		}
		else
		{
			
		}
	}
	
	return  arrayTowr;
}