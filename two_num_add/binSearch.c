#include "stdio.h"
#include "string.h"
#include "time.h"
#define NULL 	((void *)0)
#define null	0

typedef struct
{
	void *top;
	void *bottom;
	int deep;
	int maxdeep;
}infor_stack;

#define stackAdressSubOne(adress,size)	((void *)adress)=((void *)(adress)-size)
#define stackAdressAddOne(adress,size)	((void *)adress)=((void *)(adress)+size)

/***********************************************
*fun:初始化栈,需要知道深度及类型--栈的向下增长--且限定:初始值top=bottom;该空间不存放数据.
*数据是从低地址向高地址占用空间.因此bottom其实也不应该放数据.会造成数据的溢出.
*每次push时,top--,然后在top的位置存储数据.每次pop时,取出top位置的数据,然后top++;
*取出数据后,在top++后,如果top==bottom,则栈为空.
*namespace:
*var:
*return:
************************************************/
int stack_init(int deep,int size,infor_stack *stack)
{
	void *pr=NULL;
	
	pr=malloc(size*(deep));
	
	stack->bottom=pr+(size*(deep));//这个地址是已经不输出数组的空间了,因此该地址不能存放数据.只有top--后才能存放数据.
	stack->top=stack->bottom;
	stack->deep=0;
	stack->maxdeep=deep;
	
	return 1;
	
}

int stack_free(infor_stack *stack,int size)
{
	
	return free((void *)(stack->bottom)-stack->maxdeep*size);
	
}

int stack_push(infor_stack *stack,void *dat,int size)
{
	if(stack->deep>=stack->maxdeep)
	{
		return 0;
	}
	stackAdressSubOne(stack->top,size);
	memcpy(stack->top,dat,size);
	stack->deep++;
	
	return 1;
	
}

int stack_pop(infor_stack *stack,void *dat,int size)
{
	if((stack->top==stack->bottom)||(stack->deep<=0))
	{
		return 0;
	}
	
	memcpy(dat,stack->top,size);
	stackAdressAddOne(stack->top,size);
	stack->deep--;
	
	return 1;
	
}

int stack_deep(infor_stack *stack)
{
	return stack->deep;
}




/***********************************************
*fun	:二分排序,使用挖坑法,但是需要使用C自己写的栈
*namespace:binSort
*var	:
*return:	1:ok,-1:err
************************************************/
int binSort(int * buf,int len,int *id)
{
	int head=0,tail=0;
	infor_stack dat_stack;
	int st=0;
	int i=0;
	int ht=0;
	int tmp=0;
	int pitval=0;
	int pitid=0;
	int pos=0;
	stack_init(len,sizeof(ht),&dat_stack);

	ht=(0x00)|(len<<16);
	stack_push(&dat_stack,&ht,sizeof(ht));
	while(stack_deep(&dat_stack)!=0)
	{
		st=stack_pop(&dat_stack,&ht,sizeof(ht));
		if(st==0)
		{
			printf("\npop err\n");
			return -1;
		}
		head=ht&0xffff;
		tail=(ht>>16);
		
		pitval=buf[head];
		pitid=id[head];
		buf[head]=null;
		id[head]=null;
		pos='t';
		for(;head<tail;)
		{
			if(pos=='t')
			{
				tail--;
				if(buf[tail]<pitval)
				{
					buf[head]=buf[tail];
					id[head]=id[tail];
					buf[tail]=null;
					id[tail]=null;
					pos='h';
				}
				
			}
			else
			{
				head++;
				if(buf[head]>pitval)
				{
					buf[tail]=buf[head];
					id[tail]=id[head];
					buf[head]=null;
					id[head]=null;
					pos='t';
				}
				
			}
			
		}
		buf[head]=pitval;
		id[head]=pitid;
		tmp=(ht&0xffff)|(head<<16);
		if((tmp>>16)>(tmp&0xffff))
		{
			stack_push(&dat_stack,&tmp,sizeof(ht));	
			
		}		
		tmp=(tail+1)|((ht&0xffff0000));
		if((tmp>>16)>(tmp&0xffff))
		{
			stack_push(&dat_stack,&tmp,sizeof(ht));	

		}		

	}
	
	
	return 1;
}

#define swap(a,b)   do{a=a^b;b=a^b;a=a^b;}while(0)
int shellSort(int *buf,int len,int *id)
{
    int i=0;
    int j=0;
    int h=0;

   // h=len/3+1;
    while(h < len/2)    h = 2*h + 1;
    for(;h>=1;h=h/2)
    {
        for(i=h;i<len;i++)
        {
            for(j=i;j>=h;j=j-h)
            {
                if(buf[j]<buf[j-h])
                {
                	printf("before--%d=%d,%d=%d\n",j,buf[j],j-h,buf[j-h]);
                    swap(buf[j],buf[j-h]);
                    printf("after---%d=%d,%d=%d\n",j,buf[j],j-h,buf[j-h]);
                    swap(id[j],id[j-h]);
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


int* twoSum(int* nums, int numsSize, int target, int* returnSize)
{
	int left=0,right=numsSize-1;	//左闭右闭区间
	int i=0;
	int *pr=malloc(4*2);
	int *id=malloc(4*numsSize);
	
	for(i=0;i<numsSize;i++)
	{
		id[i]=i;
	}
	
	shellSort(nums,numsSize,id);
	
	for(i=0;i<numsSize;i++)
	{
		printf("%d ",nums[i]);
	}
	
	*returnSize=2;
	for(;left<right;)
	{
		i=(nums[left]+nums[right]);
		if(i==target)
		{
			pr[0]=id[left];
			pr[1]=id[right];
			//printf("dat=%d %d \n",nums[left],nums[right]);
			return pr;
		}
		else if(i>target)
		{
			right--;
		}
		else
		{
			left++;
		}

	}

	printf("left=%d right=%d\n",left,right);
	*returnSize=0;
	return NULL;
}


#define len_buf		20

int main(int argc, char *argv[])
{
	int *pr=NULL;
	int returnSize=0;
//	int nums[]={1931729,5896941,-56160,-5999108,-713650,3135403,870300,-2938196,-726482,-152831,-5552046,-6857780,2493109,-2409591,7453630,7848984,-806578,4976647,-9199319,-7660981,4168359,5466164,8577624,-7465790,-7808993,-7176396,-3622579,4996177,9623530,-1371773,-5633028,5188756,7971646,-2603602,7490338,3174208,-1825054,6892503,-8201151,-1072749,-2746872,-718104,-1804706,5504856,-9104958,-2181797,3151933,-6116343,1436458,-659793,-9256591,-3487143,3396195,-6211129,-2789647,-9336842,-122743,-3843085,8515885,-8551842,-8668581,-5040156,-9626107,-5645393,-5484621,-6417906,4483313,-4197219,-5762688,-6645419,1077632,7237401,4819732,5220382,8657346,651862,7406097,6668849,5067699,4563307,-4358391,-3996666,-4298897,-9870277,-628517,1842975,-4970541,-8815324,-4174656,-9332000,4647373,9461387,-3504914,2614670,-1315721,6099266,-8562834,-4488116,9742133,307556,791291,-2611787,7656675,9932436,-1905766,5142745,7210001,-4678645,550797,8462302,6815722,-2985059,-1236401,-6093644,-6617192,-9438320,8400183,8936598,5378080,9758788,817106,8613435,3579880,-9100698,993960,5352546,8481471,3526666,2961295,-7819498,834324,3435605,9725655,8490290,7263645,-6863345,2947003,-8152353,5514173,-9865666,343864,-6386758,-8675825,-3252881,1149651,6690889,7528843,-2861961,-4722247,-6985318,-9994086,7192089,-4864512,5773490,-9492163,-2089496,2246943,430576,3466065,1578813,2738295,261493,-425120,3666399,8796329,9220040,7148637,5956722,9980401,7834421,2041136,3763240,-831391,7963736,8744496,9593751,4606042,66708,-6652782,-6619264,2614784,5655774,-9547771,2456855,4268450,864132,4601358,-3464052,-8987625,-4427713,-126397,-5823810,8443040,-7428684,-4365060,-6392451,-6258724,-9124302,-6566562,-4153846,-9120859,7061892,-7707777,9342864,-2133124,-7068189,-8657201,2752682,-9943634,-8650517,-5219095,2305245,-1500154,8915790,-2805210,1860598,3361037,-8040669,5809813,-2243400,-3418428,-1810675,-5203397,-7879177,6898143,7749161,-2810814,-7525359,-793458,608571,-9383497,9531109,6862130,-3381989,2332039,6018975,-3350279,7266303,-7170480,-1127784,578313,6018329,8822950,4214273,-372530,2706552,-4905224,3324641,-5403812,9319424,-2882974,-9228107,6151855,2634284,7000575,3215247,5067619,5076150,1595871,748735,-9225927,1962290,4653714,697462,2996679,-7562085,-5387705,-912530,1763635,-6139098,2103001,-6316004,-3469182,-3818553,8854173,422707,-3826706,-9777012,7452324,3665456,-9458220,4828502,2641583,5107388,-4201437,5486625,-9315573,1417427,-1838955,9042987,-9704280,7868946,-6737802,4462332,-5771637,635858,1438260,8940144,9385885,3522022,-9736738,-4387243,-8611088,8733735,-2087884,8299364,-1004025,2674570,4247204,1972151,-2092476,-1318052,-796315,-6487458,8714867,-9966767,-5459861,-1286737,9442201,-2065902,2575270,7184243,5916475,-5728128,-3044939,6372732,4902791,3474374,-7462481,-9886552,-8361835,-5022392,-4583670,3990789,-1051830,8606314,6365011,6751820,1727390,-2005234,-5845824,9572998,6867269,4000416,5629823,-2918551,7278467,9865305,3781390,-1518411,-7124219,3800842,-4492377,-7479001,4144917,4639464,7161451,3391326,-2429792,8240315,-8667371,101661,-6049927,-9729041,-2210443,-6907416,-1355534,6292201,-1169114,-4565077,3214740,-6122903,8326559,-3973709,2313703,9645383,6119383,8317176,8177731,7758979,3037198,-9543017,-3436171,3280360,-1444577,-40402,-8372623,6672469,9125222,6161730,3783090,-2778572,-8014616,-1588372,-3200391,7126312,-5091311,-3637052,-6600914,9034974,-6246289,-4974981,-9711741,-7867786,-9808240,450272,-3222991,-8251454,3599528,-33487,-1114626,8414308,-2607016,1690025,5981016,-2898849,-4650973,-8085619,-4875585,-2677275,-7329644,-9730701,-5563788,4635028,-4232606,1863619,-9081061,-8966306,4907349,348942,-6309867,2436213,-6340842,8276407,-1112458,4073087,6632641,-6044509,-1714553,-9905887,8140713,-5566484,-8187046,-3636448,560425,9931445,-8142363,1928694,6652925,5094575,5525595,9847315,5323311,9123454,7425631,2116547,9033708,-2228326,2858537,5452249,-3388576,8859366,5965629,-9904937,8961420,-8794807,4363288,-1067547,1903643,-6637956,3736544,9150157,-3977433,4333094,-1170635,-4933425,5815702,1846616,793301,-9206787,-227043,-7076926,-461803,-7477454,-1297664,47425,8506054,8264895,-1199514,-4031592,1245359,2840097,6948011};
	int nums[]={3,2,4};	
	int tar=6;
	int i=0;
	int dat=0;
	char st=0;
	
	int seed=10;
	int buf[len_buf];
	int pre_sum=0,now_sum=0;
	
	int count=100;

	int j=0;
	
	
	pr=twoSum(nums,sizeof(nums)/sizeof(nums[0]),tar,&returnSize);
	
	if(pr!=NULL)
	{
		printf("pr=%d %d \n",pr[0],pr[1]);
	}
	else
	{
		puts("\nerror\n");
	}



#if 0
	for(count=0;count<100;count++)
	{
	
	
		seed=srand(buf[len_buf/2]);
		printf("seed=%d \n",seed);
		
		pre_sum=0,now_sum=0;
		for(i=0;i<len_buf;i++)
		{
			buf[i]=rand(seed)%100;
			printf("%d ",buf[i]);
			pre_sum+=buf[i];
		}
		
		printf("\n----====\n");
		
			
		binSort(buf,len_buf);
		
		printf("\n----\n");
		for(i=0;i<len_buf;i++)
		{
			printf("%d ",buf[i]);
			now_sum+=buf[i];
			if(i>0)
			{
				if((buf[i]-buf[i-1])>=0)
				{
				}
				else
				{
					printf("\n\n--********sort err----\n");
					return 0;
				}
			}
		}
		if(now_sum!=pre_sum)
		{
			printf("\n-------sum_err---------\n");
			return 0;
		}
		
		printf("\n|||||||||||||||||||||||||||||||||\n");
	
	}
	
#endif	
	
	
	
	return 0;
}