#include <stdio.h>
#include <stdlib.h>

#define NULL ((void *)0)

typedef struct __hash_node__
{
	int val;
	struct __hash_node__ *next;
}hash_node;

int hashListInser(hash_node **pr_list,int val,int len)
{
	int tmp=0;
	int key=len;
	hash_node *pr_cur=NULL;
	hash_node *pr_node = malloc(sizeof(hash_node));
	
	pr_node->val=val;
	pr_node->next=NULL;

	tmp=pr_node->val%key;
	tmp=(tmp+key)%key;
	if(((pr_list)[tmp])==NULL)	/*放入第一个*/
	{
		(pr_list)[tmp]=pr_node;
	}
	else //在最后一个节点后插入新的node
	{
		pr_cur=(pr_list)[tmp];
		while(pr_cur->next!=NULL)
		{
			pr_cur=pr_cur->next;
		}
		pr_cur->next=pr_node;
		pr_node->next=NULL;
	}
	
	return 1;
}

int hashListFind(hash_node **pr_list,int val,int len)
{
	int tmp=val%len;
	hash_node *pr_cur=NULL;
	int testval=0;

	tmp=(tmp+len)%len;
	pr_cur=(pr_list)[tmp];
	if(pr_cur==NULL)
	{
		return -1;
	}
	if(pr_cur->val==val)
	{
		return tmp;
	}
	else
	{
		while(pr_cur!=NULL)
		{
			testval=pr_cur->val;
			if(testval==val)
			{
				return tmp;
			}
			else
			{
				pr_cur=pr_cur->next;
			}		
		}	
	}

	
	return -1;
}




#define KEY 11


int* twoSum(int* nums, int numsSize, int target, int* returnSize)
{
	static hash_node * hashlist[KEY];
	int i=0;
	int j=0;
	int st=0;
	int *pr=malloc(8);
	*pr=-1;
	*(pr+1)=-1;
	for(i=0;i<KEY;i++)
	{
		hashlist[i]=NULL;
	}
	
	for(i=0;i<numsSize;i++)
	{
		hashListInser(hashlist,nums[i],KEY);
	}
	printf("after-inser\n");
	for(i=0;i<numsSize;i++)
	{	
		printf("result=%d \n",target-nums[i]);
		printf("before find %d\n",i);
		st=hashListFind(hashlist,target-nums[i],KEY);
		printf("find=%d  st = %d after find %d\n",target-nums[i],st,i);
		if(st!=-1)
		{
			pr[0]=i;
			pr[1]=-1;
			*returnSize=2;
			for(j=i+1;j<numsSize;j++)
			{
				if((target-nums[i])==nums[j])
				{
					pr[1]=j;
				}
			}
			if(pr[1]==-1)
			{
				continue;
			}
			
			
			printf("pr=%d %d\n",pr[0],pr[1]);
			return pr;
		}
		
		
	}
	
	if(*pr==-1)
	{
		free(pr);
	}
	return NULL;
}



int main(int argc, char *argv[])
{
	int *pr=NULL;
	int returnSize=0;
	int nums[]={5,75,25};
	
	printf("hello =%d \n",(-3)%10);
	
	pr=twoSum(nums, 3, 100, & returnSize);
	printf("pr=%d %d \n",pr[0],pr[1]);
	
	return 0;
}