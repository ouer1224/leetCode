#include "stdio.h"
#include "string.h"
#include "stdlib.h"

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef char int8_t;

#define fun_ok 		1
#define fun_err 	0xffffffff
#define fun_none	0

struct memCB
{
	uint32_t size;
	struct memCB *pre;
	struct memCB *next;
};//占用3个int的空间.

struct memCB s_free_mem_header={0,NULL,NULL};
struct memCB s_free_mem_tail;
struct memCB *s_pr_mem_list=NULL;


struct memCB s_used_mem_header={0,NULL,NULL};
struct memCB s_used_mem_tail;












/*初始化整个内存块为最小单位为1,然后已2的幂递增的内存块*/
//!将内存块的大小设定为2进制数值,找到最高位的1,然后该位设置为0,低位全部设置为1.思路是找到一个最大的0b11111形式的数字.
//然后按照其进行内存块的划分.
//然后在使用size-bxxxx,这个余数的位1与bxxxx进行相加,但是1+1=2,不能进行进位.此处的操作使用char[]内放入每个位.再进行相加
//按照char[]的数值,对内存进行1-2-4这样的形式,进行内存块的划分

//todo:为了测试方便,暂时将内存空间的最小单位为128字节.

//!要求mempool是地址对齐的.

//暂时设定最大的内存块是1024*8,为了创建垂直链表

#define min_block_size 128

/*创建垂直链表
水平链表使用内存块的前面几个字节,不用单独创建了
*/

struct VmemCB
{
	uint32_t mulriple;//其真实大小为mulriple*min_block_size
	struct memCB *Hmem;
	struct VmemCB *pre;
	struct VmemCB *next;
};

#define max_level_vmem	(6+1)	//如果想要2的exp6,buf就需要有7个元素.因为是从0开始计算的.
struct VmemCB s_list_vmem[max_level_vmem];	
struct VmemCB *s_pr_list_vmem=s_list_vmem;
//最大size为128*2exp(6)=128*64;
//最小size为128*2exp(0)=128*1


#define pushListToHeader(pr_inser,vlist)	do{\
	if(vlist->Hmem!=NULL)\
	{\
		pr_inser->pre=NULL;\
		pr_inser->next=vlist->Hmem;\
		vlist->Hmem->pre=pr_inser;\
		vlist->Hmem=pr_inser;\
	}\
	else\
	{\
		pr_inser->pre=NULL;\
		pr_inser->next=NULL;\
		vlist->Hmem=pr_inser;\
	}\
}while(0);

#define popListFromHeader(pr_inser,vlist)	do{\
	if(vlist->Hmem==NULL)\
	{\
		pr_inser=NULL;\
	}\
	else\
	{\
		pr_inser=vlist->Hmem;\
		vlist->Hmem=pr_inser->next;\
		if(vlist->Hmem!=NULL)\		
		{\
			vlist->Hmem->pre=NULL;\
		}\
	}\
}while(0);




uint32_t init_mem(uint8_t *mempool,uint32_t size)
{
	uint32_t _size=size/min_block_size;
	uint32_t tmp=0;
	uint32_t i=0,j=0;
	uint8_t bit[32];
	struct memCB *pr_mem=NULL;

	s_free_mem_header.next=&s_free_mem_tail;
	s_free_mem_header.pre=NULL;
	s_free_mem_header.size=0;

	s_free_mem_tail.pre=&s_free_mem_header;
	s_free_mem_tail.next=NULL;
	s_free_mem_tail.size=0;

	s_used_mem_header.next=&s_used_mem_tail;
	s_used_mem_header.pre=NULL;
	s_used_mem_header.size=0;

	s_used_mem_tail.pre=&s_used_mem_header;
	s_used_mem_tail.next=NULL;
	s_used_mem_tail.size=0;

	s_pr_mem_list=&s_free_mem_header;

//初始化垂直链表
	s_pr_list_vmem=s_list_vmem;
	for(i=0;i<(max_level_vmem);i++)
	{
		s_pr_list_vmem->mulriple=0x01<<i;
		s_pr_list_vmem->pre=s_pr_list_vmem-1;
		s_pr_list_vmem->next=s_pr_list_vmem+1;
		s_pr_list_vmem++;
	}

	s_pr_list_vmem=s_list_vmem;
	s_pr_list_vmem->mulriple=0x01<<0;
	s_pr_list_vmem->pre=NULL;
	s_pr_list_vmem->next=s_pr_list_vmem+1;


	s_pr_list_vmem=s_list_vmem+max_level_vmem;
	s_pr_list_vmem->next=NULL;
//初始化完毕


	if(((uint32_t)mempool&0x03)!=0)
	{
		return fun_err;
	}

	for(i=0;i<32;i++)
	{
		if(_size!=0)
		{
			_size=_size>>1;
		}
		else
		{
			break;
		}
		
	}

	if(i==32)
	{
		return fun_err;
	}
	i=i-1;
	//求出最高位的bit1是在第i位上
	//因此0bxxx是(i-1)位,也就是(0x01<<i)-1;
	_size=size/min_block_size;
	tmp=(0x01<<i)-1;
	memset(bit,0,32);
	
	printf("_size=%x i=%d\n tmp=%x\n",_size,i,tmp);

	for(j=0;j<i;j++)
	{
		bit[j]=1;
	}
	for(i=0;i<8;i++)
	{
		printf("bit[%d]=%d\n",i,bit[i]);
	}
	tmp=_size-tmp;
	for(j=0;j<i+1;j++)
	{
		if(tmp&0x01==1)
		{
			bit[j]++;
		}
		tmp=tmp>>1;
	}
	for(i=0;i<8;i++)
	{
		printf("bit[%d]=%d\n",i,bit[i]);
	}
	printf("I1\n");

//根据bit[]中数值,及其代表的内存块,将内存块挂载到垂直链表中
//!注意:垂直链表中的size是min_block_size的N倍,必定是2的M幂.而bit[i]中的i也是min_block_size的2的i幂--切记进行转换.
	for(i=0;i<max_level_vmem;i++)
	{
		//挂载水平链表到合适的垂直链表中.垂直链表从内存块中取值
		//水平链表不进行回环处理
		s_pr_list_vmem=s_list_vmem+i;

		s_pr_list_vmem->Hmem=NULL;

		while(bit[i]>0)
		{	
			bit[i]--;
			pr_mem=mempool;
			mempool=mempool+min_block_size*(0x01<<i);

			pr_mem->size=(0x01<<i)*min_block_size;
			pr_mem->pre=NULL;
			pr_mem->next=NULL;
#if 0
			if(s_pr_list_vmem->Hmem==NULL)
			{
				s_pr_list_vmem->Hmem=pr_mem;
			}
			else//!将新的节点插入到最前面.//在删除时,也从第一个节点开始删除.不用再进行轮训链表了.
			{
				pr_mem->next=s_pr_list_vmem->Hmem;
				s_pr_list_vmem->Hmem->pre=pr_mem;
				s_pr_list_vmem->Hmem=pr_mem;
			}
#else
			pushListToHeader(pr_mem,s_pr_list_vmem);
#endif
		}

	}


#if 0
	for(i=0;i<32;i++)
	{
		while(bit[i]>0)
		{
			printf("bit[%d]=%d\n",i,bit[i]);
			bit[i]--;
			pr_mem=mempool;
			mempool=mempool+min_block_size*(0x01<<i);

			s_pr_mem_list->next=pr_mem;
			pr_mem->pre=s_pr_mem_list;
			pr_mem->size=(0x01<<i)*min_block_size;
			pr_mem->next=NULL;
			s_pr_mem_list=pr_mem;
		}

	}

	s_pr_mem_list->next=&s_free_mem_tail;
	s_free_mem_tail.next=NULL;
	s_free_mem_tail.pre=s_pr_mem_list;
	s_free_mem_tail.size=0;	
#endif	

	return fun_ok;
}


/*获取size大小的内存*/
//!size必定是min_block_size倍数,如果不满足,就在函数内部处理为倍数关系.
void * os_malloc(uint32_t size)
{
	void *pr=NULL;
	uint32_t _size=0;
	uint32_t leftsize=0;
	uint32_t i=0;
	uint8_t *pr_mem=NULL;
	struct memCB *prH=NULL;
	struct VmemCB *prV=NULL;
	size=(size/min_block_size+((size%min_block_size)?1:0))*min_block_size;
printf("insize=%d\n",size);
	prV=s_list_vmem;
printf("m0\n");
	while(prV!=NULL) 
	{
		
		prH=prV->Hmem;
		if(prH!=NULL)
		{
			_size=prH->size;
			if(_size>=size)
			{
				popListFromHeader(prH,prV);
				pr=(void *)prH+sizeof(struct VmemCB);
				break;
			}

		}
		else
		{

		}

		prV=prV->next;
	}
	if(prV==NULL)
	{
		return NULL;
	}

	//已找到合适的内存块
	printf("addr=%p size=%d\n",prH,prH->size);
	//将剩余的空间加入到Vlist中
	leftsize=prH->size-size;
	printf("leftsize=%d  %x\n",leftsize,leftsize/min_block_size);
	leftsize=leftsize/min_block_size;
	//将leftsize的min_block_size的倍数,找到其二进制码,然后根据二进制码,将剩余的内存块放入到Vlist中
	prV=s_list_vmem;
	(uint32_t)pr_mem=(uint32_t)prH+size;
	while(leftsize!=0)
	{
		printf("V-size=%d\n",prV->mulriple);
		if((leftsize&0x01)==0)
		{

		}
		else
		{
			prH=pr_mem;
			prH->size=prV->mulriple*min_block_size;
			prH->pre=NULL;
			prH->next=NULL;
			pushListToHeader(prH,prV);
			printf("push mem ok-size=%d addr=%p\n",prH->size,prH);

			(uint32_t)pr_mem=(uint32_t)pr_mem+prH->size;
		}

		leftsize=leftsize>>1;
		prV=prV->next;
	}

	return pr;
}

uint8_t readMemMap(void)
{
	int i=0;
	struct VmemCB *pr=NULL;
	struct memCB *pr_H=NULL;

	pr=s_list_vmem;
	i=0;
	while(pr!=NULL)
	{
		
		pr_H=pr->Hmem;
		while(pr_H!=NULL)
		{
			printf("i=%d mul=%d  size=%d add=%p\n",i,pr->mulriple,pr_H->size,pr_H);
			pr_H=pr_H->next;
		}
		i++;
		pr=pr->next;
	}
	return 1;
}




#define SIZE_MEM_POOLE	(128*78+56)
uint8_t s_mempool[SIZE_MEM_POOLE];

int main(void)
{
	int i=0;
	struct VmemCB *pr=NULL;
	struct memCB *pr_H=NULL;
	printf("s0\n");
	
	init_mem(s_mempool,SIZE_MEM_POOLE);

	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();	
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();	
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();
	printf("--mem=%p\n",os_malloc(300));
	readMemMap();	
				
	return fun_err;
}