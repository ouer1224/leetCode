


#include "stdio.h"
#include "stdlib.h"

#include "link_list.h"



typedef int int32_t;
typedef char int8_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t; 


struct __list_buddy_unit
{
	uint8_t *pr_mem;
	uint8_t state;
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

/***********************************************
*fun     :使用buddy的算法,获取内存块
*name    :
*var     :
*return  :
************************************************/

int bmalloc(void)
{
	return NULL;
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
*fun     :初始化buddy
*name    :
*var     :
*return  :
*message :	初始化时,设定最大的内存块的大小.
			部分变量直接使用pr开头的内存空间.后续的空间才分配给user使用.
************************************************/

#define __getMemFromPr(pr,size)    ((1:(pr=pr+size)*0:0)?0:pr-size)       //由于__1st_kmalloc返回0,因此必定返回pr-size.


struct __list_buddy * binit_mem(uint8_t *pr,uint32_t len)
{
	uint32_t i=0;
	uint32_t maxMemUniot=0;
	uint8_t *pr_noused=pr;
	struct __list_buddy *pr_list=NULL;
	struct __list_buddy *head_list=NULL;
	struct __list_buddy *tail_list=NULL;
	struct __list_buddy_unit *pr_unit=NULL;
	
	#ifdef MAX_MEM_UNIOT_BUDDY
	maxMemUniot=MAX_MEM_UNIOT_BUDDY;
	#else
	maxMemUniot=1;
	while(maxMemUniot<len/2)	/*找出pr中能开辟出的1+1+2+4+8....的最大空间*/
	{
		maxMemUniot*=2;
	}
	maxMemUniot/=2; /*最大的单体空间大小*/
	#endif
	
	/*todo:
	如果变量使用pr的内部空间,是否会出现划分出的内存块不是2的幂的连续方式
	需要经过测试
	*/
#if 0	/*变量占用的空间暂时使用malloc获取,后续改善此处*/
	pr_noused=pr_noused-(pr_noused%sizeof(struct __list_buddy))+sizeof(struct __list_buddy);/*地址空间对齐*/
	head_list=__getMemFromPr(pr_noused,sizeof(struct __list_buddy));
#else
	head_list=malloc(sizeof(struct __list_buddy));
#endif	
	/*head存放2的0幂,后续依次为1幂...n幂*/
	head_list->Npower=1;/*2的0幂*/
	pr_unit=malloc(sizeof(struct __list_buddy_unit));
	pr_unit->pr_mem = pr_noused+0;
	pr_unit->state = MEM_FREEING;
	head_list->pr_list_buddy=pr_unit;
	init_link_list(&(head_list->link));	/*链表是自回环的形式*/
	/*取后续的n幂*/

	i=1;/*创建2的N幂的链表*/
	while(i<=maxMemUniot)
	{
		pr_list=malloc(sizeof(struct __list_buddy));
		pr_list->Npower=(i=i*2);
		printf("pr[%d]=%p\n",i,pr_list);
		pr_unit=malloc(sizeof(struct __list_buddy_unit));
		pr_unit->pr_mem = pr_noused+i;
		pr_unit->state = MEM_FREEING;
		pr_list->pr_list_buddy=pr_unit;
		list_add_before(&(pr_list->link),&(head_list->link));	
	}
	/*创建每个幂指向的地址空间的链表---水平方向的链表*/
/*	
	todo:规定-链表形式为二维.垂直方向的链表(__list_buddy)为索引,每个单元为2的n幂.指向水平方向的链表(__list_buddy_unit).
		水平方向的链表(__list_buddy_unit)是指向具体的内存块,相同高度的水平链表的单元指向的都是其对应的垂直链表表示的2的N幂的空间内存块.
		
		创建的指向地址空间的链表cell如何获取空间.其与pr_noused的关系如何处理
		垂直链表单元数量不会增加,但是每个垂直链表单元指向的横向链表,其单元数量是会不停变化.每次进行一次内存块的申请释放都伴随一次变化.
		那么对于这种情况,如何给横向链表申请空间.
	--现在想到的解决办法是采用堆和栈的占用方式,链表单元的占用内存都从栈取(pr的最高端),而待分配的空间,都在堆中(pr的最低端).
	--但是这样必须空出一部分的pr空间,用于堆和栈的间隔,保证栈不会溢出
*/

	return head_list;
}




static s_mbuf[8*1024+4];
int main(void)
{
	uint32_t i=0;
	struct __list_buddy *pr_head=NULL;
	struct __list_buddy *pr_list=NULL;
	struct __list_buddy_unit *pr_unit=NULL;
	pr_head=binit_mem(s_mbuf,sizeof(s_mbuf));
	
	printf("\n--------------------------\n");
	printf("s_mbuf_add=%p\n",s_mbuf);
	printf("\n--------------------------\n");
	printf("pr_head=%p\n",pr_head);
	pr_list=container_of((pr_head->link.next),struct __list_buddy,link);
	printf("pr_list=%p\n",pr_list);
	#if 1
	i=1;
	while(pr_list!=pr_head)
	{
		pr_list=container_of((pr_list->link.next),struct __list_buddy,link);
		printf("pr_list=%p pow=%d\n",pr_list,pr_list->Npower);
		pr_unit=pr_list->pr_list_buddy;
		printf("buf_addr=%d\n",(size_t)(pr_unit->pr_mem)-(size_t)(s_mbuf));
		printf("i=%d\n",i);
		i++;
		if(i>20)
		{
			break;
		}
	}
	#endif
	

	return 0;
}