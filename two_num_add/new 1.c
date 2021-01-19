

struct Node_link
{
	int *dat;
	struct Node_link *next;	
}


/***********************************************
*fun	:返回数据中,删除了重复的数据组.
*namespace:
*var	:nums有序数组,从小到大排序
*return:返回值为指向1维数组.所有有效的数据都依次放入到该数组中.	returnSize为有N组的有效组的数据..
************************************************/

int** twoSum(int* nums, int numsSize, int target, int* returnSize)
{
	int len_dat=2;
	int left=0;
	int right=numsSize-1;
	struct Node_link *pr=NULL,*pre=NULL;
	struct Node_link head={NULL,NULL};
	struct Node_link *pos=&head;
	int *pr_dat=NULL;
	int tmp=0;
	int i=0,j=0;
	
	int *buf=NULL;
	
	
	
	*returnSize=0;
	
	for(;left < right;)
	{
		tmp=nums[left]+nums[right];
		if(tmp==target)
		{
			pr=malloc(sizeof(struct Node_link));
			pr_dat=malloc(sizeof(int)*len_dat);
			pr_dat[0]=nums[left];
			pr_dat[1]=nums[right];
			pr->dat=pr_dat;
			pr->next=NULL;
			pos->next=pr;
			pos=pos->next;
			*returnSize++;
		}
		else if(tmp<target)
		{
			left++;
		}
		else
		{
			right--;
		}
		
	}
	
	if(*returnSize==0)
	{
		return NULL;
	}
	
	buf=malloc(4*(*returnSize));
	pos=head->next;
	i=0;
	j=0;
	while(pos!=NULL)
	{
		for(j=0;j<i;j=j*2)
		{
			if((pos->dat[0]==buf[j])&&(pos->dat[1]==buf[j+1]))
			{
				*returnSize--;
				continue;
			}
		}
		
		buf[i]=pos->dat[0];//0必定是小值
		buf[i+1]=pos->dat[1];//1必定是大值
		i=i*2;
		pre=pos;
		pos=pos->next;
		free(pre->dat);
		free(pre);
	}

	return buf;
}