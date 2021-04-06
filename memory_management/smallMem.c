

#include "stdlib.h"
#include "stdio.h"





typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef char int8_t;

#ifndef null
#define null ((void *)0)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif


#define code_ok 		1
#define code_err 		0xffffffff
#define code_prnone		0


#define pradd(a,b)	(((uint32_t)(a)+(uint32_t)(b)))
#define prsub(a,b)	((uint32_t)(a)-(uint32_t)(b))
#define prxor(a,b)	((uint32_t)(a)^(uint32_t)(b))



#define fun_prv		static
#define fun_glo		extern 

#define min_block_size	4


//!建议使用xor的链表,可以节约1个指针的空间.减少1/3的浪费
struct memCB
{
	uint32_t size;
	struct memCB *xor;//!xor=pre^next.锚点的pre为0,而最后1个节点的next为0.
};

//!header不存储任何信息,仅仅是作为链表的锚点.其后面的节点才用于存储信息.
//链表采用xor去实现双向链表,但是不做回环处理.
//!因此不会出现空节点,最少是有1个header.
static struct memCB s_header_noused={0,null};
static struct memCB * pr_header_noused=&s_header_noused;



#define getNextNode(cur,pre)	((void *)prxor((cur)->xor,(pre)))
#define getPreNode(cur,next)	((void *)prxor((cur)->xor,(next)))



fun_prv uint32_t os_memcpy(uint8_t *dest,uint8_t *src,uint32_t len)
{
	if((dest==NULL)||(src==NULL)||(len==0))
	{
		return code_prnone;
	}

	while (len!=0)
	{
		len--;
		*dest=*src;
		dest++;
		src++;
	}
	
	return code_ok;
}




/***********************************************
 *fun     :以小内存的管理方式,初始化mem,最小block为min_block_size(4)
			只创建1个noused的链表
 *name    :
 *var     :
 *return  :
 ************************************************/
fun_glo uint32_t init_mem(void* pool,uint32_t size)
{
	struct memCB *prmem=null;

	if(pool==NULL)
	{
		return code_prnone;
	}

	prmem=pool;
	prmem->size=size;
	prmem->xor=(void*)prxor(pr_header_noused,NULL);

	s_header_noused.size=0;
	s_header_noused.xor=(void *)prxor(NULL,prmem);

	return code_ok;
}

fun_prv void * __os_malloc(uint32_t size)
{
	struct memCB *prmem=NULL;
	uint32_t leftsize=0;

	struct memCB *pre=NULL;
	struct memCB *next =NULL;
	struct memCB *cur=NULL;
	
	size=size/min_block_size*min_block_size+(((size%min_block_size)==0)?0:min_block_size);

	cur=pr_header_noused;
	pre=NULL;
	next=NULL;
	prmem=getNextNode(cur,pre);
	pre=pr_header_noused;
	cur=prmem;
	while(cur!=NULL)
	{

		leftsize=cur->size;
		if(leftsize>=size)
		{
			if(leftsize<(size+sizeof(struct memCB)+4))
			{
				prmem=cur;
				//将其从noused链表中删除
				next=getNextNode(cur,pre);
				if(next!=NULL)
				{
					//pre->xor=pre->xor^cur^next;
					//next->xor=next->xor^cur^pre;
					pre->xor=(void*)prxor(pre->xor,cur);
					pre->xor=(void*)prxor(pre->xor,next);

					next->xor=(void*)prxor(next->xor,cur);
					next->xor=(void*)prxor(next->xor,pre);
				}
				else
				{
					pre->xor=(void*)prxor(pre->xor,cur);
					pre->xor=(void*)prxor(pre->xor,next);
				}

			}
			else//!从高地址取block,提供给user
			{
				prmem=(void *)pradd(cur,leftsize);
				prmem=(void *)prsub(prmem,size+sizeof(struct memCB));

				prmem->size=size;
				prmem->xor=prmem;

				cur->size=leftsize-size-sizeof(struct memCB);
			}

			break;
		}
		prmem=getNextNode(cur,pre);
		pre=cur;
		cur=prmem;

	}

	return prmem;
}


fun_glo os_malloc(uint32_t size)
{
	void *pr=NULL;

	pr=__os_malloc(size);
	
	pr=(void *)prsub(pr,sizeof(struct memCB));

	return pr;
}

/***********************************************
 *fun     :将内存块释放回去.并挂载noused上,挂载后需要保证内存块按照大小,从小到大进行排序.
 *name    :
 *var     :
 *return  :
 ************************************************/
fun_prv uint32_t __os_free(struct memCB *prmem)
{
	struct memCB *cur=NULL,*pre=NULL,*next=NULL;	
	uint32_t size=0;
	
	if(prmem==NULL)
	{
		return code_prnone;
	}


	cur=pr_header_noused;
	pre=NULL;
	next=getNextNode(cur,pre);

	if(next==NULL)
	{
		cur->xor=(void*)prxor(NULL,prmem);
		prmem->xor=(void *)prxor(cur,NULL);
	}

	while(next!=NULL)
	{
		pre=cur;
		cur=next;
		next=getNextNode(cur,pre);

		printf("cur=%p pre=%p next=%p\n",cur,pre,next);
		printf("mem.size=%d cur.size=%d\n",prmem->size,cur->size);
		size=cur->size;
		if(prmem->size<=size)//!将prmem插入到cur的前面
		{
			pre->xor=(void*)prxor(pre->xor,cur);
			pre->xor=(void*)prxor(pre->xor,prmem);
			prmem->xor=(void*)prxor(pre,cur);

			cur->xor=(void*)prxor(prmem,next);
			printf("inser---cur=%p pre=%p next=%p\n",cur,pre,next);
			printf("inser--xor=%p\n",prmem->xor);
			printf("inser--pre.xor=%p\n",pre->xor);

			#if 0
			if(next!=NULL)
			{
				next->xor=(void*)prxor(next->xor,cur);
				next->xor=(void*)prxor(next->xor,prmem);
			}
			#endif

			break;
		}
	}

	return code_ok;
}





void readMemMap(void)
{
	struct memCB *cur=NULL,*pre=NULL,*next=NULL;
	struct memCB *pr=NULL;

	cur=pr_header_noused;
	next=getNextNode(cur,pre);
	while(next!=NULL)
	{
		pre=cur;
		cur=next;
		next=getNextNode(cur,pre);
		printf("cur size=%d addr=%p\n",cur->size,cur);
		printf("next=%p\n",next);
	}
}





#define buf_size	1024
static uint8_t s_buf[buf_size];

void main(void)
{
	struct memCB *cur=NULL,*pre=NULL,*next=NULL;
	struct memCB *pr[48];

	uint32_t i=0;
	uint32_t st=code_err;


	st=init_mem(s_buf,buf_size);
	printf("buf st=%x startaddr = %p ,endaddr=%p\n",st,s_buf,s_buf+buf_size);


	readMemMap();

	for(i=1;i<4;i++)
	{
		printf("\n-------i=%d--------------\n",i);
		pr[i]=__os_malloc(i);
		printf("ml add=%p,size=%d\n",pr[i],pr[i]->size);
		readMemMap();
		printf("\n~~~~~~~~~~~~~~~~~~~~~\n");
	}


	printf("\n\n----------free----------------\n\n");
	for(i=1;i<4;i++)
	{
		printf("\n-------free i=%d--------------\n",i);
		printf("free %p size=%x\n",pr[i],pr[i]->size);
		__os_free(pr[i]);
		readMemMap();
		printf("\n~~~~~~~~~~~~~~~~~~~~~\n");
	}









	printf("\n========================\n");
}






