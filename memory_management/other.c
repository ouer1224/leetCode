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
};//ռ��3��int�Ŀռ�.

struct memCB s_free_mem_header={0,NULL,NULL};
struct memCB s_free_mem_tail;
struct memCB *s_pr_mem_list=NULL;


struct memCB s_used_mem_header={0,NULL,NULL};
struct memCB s_used_mem_tail;


/*��ʼ�������ڴ��Ϊ��С��λΪ1,Ȼ����2���ݵ������ڴ��*/
//!���ڴ��Ĵ�С�趨Ϊ2������ֵ,�ҵ����λ��1,Ȼ���λ����Ϊ0,��λȫ������Ϊ1.˼·���ҵ�һ������0b11111��ʽ������.
//Ȼ����������ڴ��Ļ���.
//Ȼ����ʹ��size-bxxxx,���������λ1��bxxxx�������,����1+1=2,���ܽ��н�λ.�˴��Ĳ���ʹ��char[]�ڷ���ÿ��λ.�ٽ������
//����char[]����ֵ,���ڴ����1-2-4��������ʽ,�����ڴ��Ļ���

//todo:Ϊ�˲��Է���,��ʱ���ڴ�ռ����С��λΪ128�ֽ�.

//!Ҫ��mempool�ǵ�ַ�����.

#define min_block_size 128

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
	//������λ��bit1���ڵ�iλ��
	//���0bxxx��(i-1)λ,Ҳ����(0x01<<i)-1;
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
	

	return fun_ok;
}


/*��ȡsize��С���ڴ�*/
//!size�ض���min_block_size����,���������,���ں����ڲ�����Ϊ������ϵ.
void * os_malloc(uint32_t size)
{
	void *pr=NULL;
	uint32_t _size=0;

	size=(size/min_block_size+((size%min_block_size)?1:0))*min_block_size;

	s_pr_mem_list=s_free_mem_header.next;
	while(s_pr_mem_list!=&s_free_mem_tail)
	{
		_size=s_pr_mem_list->size;
		if(_size>=size)
		{
			break;
		}
		s_pr_mem_list=s_pr_mem_list->next;
	}
	if(s_pr_mem_list==&s_free_mem_tail)
	{
		return NULL;
	}

	//���ҵ����ʵ��ڴ��
	printf("addr=%p size=%d\n",s_pr_mem_list,s_pr_mem_list->size);


//����used list
	s_pr_mem_list=&s_used_mem_header;

	return pr;
}

























#define SIZE_MEM_POOLE	(128*78+56)
uint8_t s_mempool[SIZE_MEM_POOLE];

int main(void)
{
	int i=0;
	struct memCB *pr=NULL;
	
	printf("s0\n");
	
	init_mem(s_mempool,SIZE_MEM_POOLE);

	pr=s_free_mem_header.next;

	printf("s1\n")	;
	while(pr!=&s_free_mem_tail)
	{
		printf("size=%d addr=%p\n",pr->size,pr);
		pr=pr->next;
	}


	os_malloc(4095);



	return fun_err;
}