/*
目前需要动态开辟的变量空间使用malloc
后续想办法使用其他的方式.
或者:
是否可以直接开辟出足够的变量buf,然后供init和malloc使用.还得方便free函数中对变量空间的释放.

*/


#include "stdio.h"
#include "stdlib.h"

#include "link_list.h"



typedef int int32_t;
typedef char int8_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t; 


struct __list_buddy_unit
{
	uint8_t state;
	uint8_t *pr_mem;
	struct __link_list link;
};


struct __list_buddy
{
	uint32_t Npower;
	struct __list_buddy_unit *pr_list_buddy;
	struct __link_list link;
};

#define MEM_FREEING		0
#define MEM_MALLOCED	1

#define MAX_MEM_UNIOT_BUDDY		1024


struct __list_buddy *s_phead_buddy=NULL;


#define getNextBuddy(pr_list)		container_of((pr_list->link.next),struct __list_buddy,link);
#define getNextBuddyUnit(pr_list)		container_of((pr_list->link.next),struct __list_buddy_unit,link);


/***********************************************
*fun     :使用buddy的算法,获取内存块
*name    :
*var     :
*return  :
************************************************/

void * bmalloc(uint32_t size)
{
	void *pr=NULL;
	struct __list_buddy *pr_list=NULL;
	
	printf("ram_size=%d\n",size);
	pr_list=s_phead_buddy;
	do
	{
		if(pr_list->Npower>=size)
		{
			break;
		}
		pr_list=container_of(pr_list->link.next,struct __list_buddy,link);
	}
	while(pr_list!=s_phead_buddy);
	
	printf("satisfy_N=%d\n",pr_list->Npower);
	
	return pr;
}
/***********************************************
*fun     :释放buddy的内存块
*name    :
*var     :
*return  :
************************************************/

int bfree(void)
{

	return NULL;
}

/***********************************************
*fun     :求一个数值的2的底 
*name    :
*var     :
*return  :
************************************************/ 
uint32_t getExp2(uint32_t val)
{
	uint32_t i=1,j=0;
	
	while(i<=val/2)
	{
		i*=2;
		j++;
	}
	if(val!=i)
	{
		return 0xffffffff;
	}

	return j;
	
}
/***********************************************
*fun     :初始化buddy
*name    :
*var     :
*return  :
*message :	初始化时,设定最大的内存块的大小.
			部分变量直接使用pr开头的内存空间.后续的空间才分配给user使用.
************************************************/

#define __getMemFromPr(pr,size)    ((1:(pr=pr+size)*0:0)?0:pr-size)       //由于__1st_kmalloc返回0,因此必定返回pr-size.

#define prToUint32(pr)		((uint32_t)(pr))
#define SizeBuddy			sizeof(struct __list_buddy)
#define SizeBuddyUnit		sizeof(struct __list_buddy_unit)

struct __list_buddy * binit_mem(uint8_t *pr,uint32_t len)
{
	uint32_t i=0;
	uint32_t remain=0;
	uint32_t tmp=0;
	uint32_t npow=0;
	uint32_t maxMemUniot=0;
	uint8_t *pr_start=pr;	/*区间是左闭右开-- [ pr_start,pr_end ) */
	uint8_t *pr_end=pr+len;
	struct __list_buddy *pr_list=NULL;
	struct __list_buddy *head_list=NULL;
	struct __list_buddy *tail_list=NULL;
	struct __list_buddy_unit *pr_unit=NULL;
	/*
采用新的分配策略,如下:
根据当前的len,求出最大的(2的n幂-1),其对应的是二进制的1111这样的数值.
对其做减法,得到remain,再求出remain的二进制数值.
按照最大的(2的n幂-1)这个数值的最大位数,其就是垂直链表的个数.
然后垂直链表的每个单元都添加一个水平链表,其指向的是该单元对应的二进制的位,所表示的2的n幂大小.如,第0位:2的0幂.第1位:2的1幂....
在将上述初始化完成后.
在将remain代表的二进制的数值,其对应的垂直链表的单元,所指向的水平链表中,添加新的一个指向该大小内存的单元.

由于需要考虑地址对齐问题,因此需要先将pr的地址按照设计的最大的内存块进行地址对齐.
然后再进行上述的运算.
由于地址对齐在空闲的内存空间,可以按照 "次级最大内存块" 进行对齐.再放入一次.
循环,直到按照2进行对齐后,将剩余的内存再按照1放入一次即可.

根据上述思想:求(最大的2的n幂),其实就是找出允许的最大的地址对齐.
	*/
	
	maxMemUniot=1;
	do
	{
		maxMemUniot*=2;
		tmp=prToUint32(pr_end)-prToUint32(pr_end)%maxMemUniot-maxMemUniot;	
	}while(tmp>=prToUint32(pr_start));
	
	maxMemUniot/=2;
	
	prToUint32(pr_end)=prToUint32(pr_end)-prToUint32(pr_end)%maxMemUniot;
	prToUint32(pr_start)=prToUint32(pr_end)-maxMemUniot;
	
	printf("pr=%p\n",pr);
	printf("len=%d maxMemUniot=%d\n",len,maxMemUniot);
	printf("maxMemUniot=%x\n pr_start=%p\n pr_end=%p\n",maxMemUniot,pr_start,pr_end);
	
	/*将对齐后的地址,初始化到链表中*/
	//获取垂直链表的单元的数量,用于开辟空间 
	tmp=maxMemUniot;
	for( ; tmp >0 ; tmp /=2)
	{
		i++;
	}
	printf("all = %d \n",i);
	head_list=malloc(i*SizeBuddy); 

	pr_unit=malloc(SizeBuddyUnit);
	pr_unit->state=MEM_FREEING;
	pr_unit->pr_mem=pr_start+0;
	init_link_list(&(pr_unit->link));

	head_list[0].Npower=1;
	head_list[0].pr_list_buddy=pr_unit;
	init_link_list(&(head_list[0].link));	
	
	pr_unit=malloc(SizeBuddyUnit);
	pr_unit->state=MEM_FREEING;
	pr_unit->pr_mem=pr_start+head_list[0].Npower;
	list_add_before(&(pr_unit->link),&(head_list[0].pr_list_buddy->link));
	
	for(npow=1;npow<i;npow++)
	{
		pr_unit=malloc(SizeBuddyUnit);

		head_list[npow].Npower=head_list[npow-1].Npower*2;
		head_list[npow].pr_list_buddy=pr_unit;
		list_add_before(&(head_list[npow].link),&(head_list[0].link));	
		
		pr_unit->state=MEM_FREEING;
		pr_unit->pr_mem=pr_start+head_list[npow].Npower;
		init_link_list(&(pr_unit->link));
	}
	
	/*将 [ pr, pr_start )的内存,左闭右开 ,添加到链表中*/ 
	/*只要确保maxMemUniot比[ pr, pr_start )区间大,则下面的推算过程就能变为寻找一个数据的二进制*/

	printf("\n[ pr=%p , pr_start=%p )\n",pr,pr_start);
	tmp=maxMemUniot;
	while(tmp>=1)
	{
		do
		{
			tmp/=2;
		
		}//while(prToUint32(pr_start)-tmp<prToUint32(pr));
		while((prToUint32(pr_start)-prToUint32(pr))<tmp);
			
		if(tmp!=0)
		{
			
			i=getExp2(tmp);
			pr_unit=malloc(SizeBuddyUnit);
			pr_unit->state=MEM_FREEING;
			pr_unit->pr_mem=(pr_start-=tmp);

			list_add_before(&(pr_unit->link),&(head_list[i].pr_list_buddy->link));
		
			printf("pow=%d  %x mem=%p\n",tmp,tmp,pr_unit->pr_mem);
			
		}
		else
		{
			//证明[ pr, pr_start )区间无null 
			printf("\nnull\n");
		}
	
	}
	
	
	/*将 [pr_end , pr+len )的内存,区间:左闭右开 , 添加到链表中*/ 
	printf("\n[pr_end=%p , pr+len=%p )\n",pr_end,pr+len);
	tmp=maxMemUniot; 
	while(tmp>=1)
	{
		do
		{
			tmp/=2;
		
		}//while((prToUint32(pr_end)+tmp)>(prToUint32(pr)+len));
		while((prToUint32(pr)+len-prToUint32(pr_end))<tmp);
		if(tmp!=0)
		{
			i=getExp2(tmp);
			pr_unit=malloc(SizeBuddyUnit);
			pr_unit->state=MEM_FREEING;
			pr_unit->pr_mem=(pr_end+=tmp);
			list_add_before(&(pr_unit->link),&(head_list[i].pr_list_buddy->link));
			
			printf("pow=%d  %x mem=%p\n",tmp,tmp,pr_unit->pr_mem);
		}
		else
		{
			//证明[ pr, pr_start )区间无null 
			printf("\nnull\n");
		}
	
	}


	return head_list;
}




static char s_mbuf[1023];
int main(void)
{
	uint32_t i=0;
	struct __list_buddy *pr_head=NULL;
	struct __list_buddy *pr_list=NULL;
	struct __list_buddy_unit *pr_unit=NULL;
	struct __list_buddy_unit *head_unit=NULL;
	pr_head=binit_mem(s_mbuf,sizeof(s_mbuf));
	
	for(i=0;i<10;i++)
	{
		head_unit=pr_head[i].pr_list_buddy;
		pr_unit=head_unit;
		do
		{
			printf("%d-npow=%d pr_mem=%p\n",i,pr_head[i].Npower,pr_unit->pr_mem);
			pr_unit=getNextBuddyUnit(pr_unit);
		}while(pr_unit!=head_unit);
	}
	
	return 0;
}