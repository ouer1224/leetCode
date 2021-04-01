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

//��ʱ�趨�����ڴ����1024*8,Ϊ�˴�����ֱ����

#define min_block_size 128

/*������ֱ����
ˮƽ����ʹ���ڴ���ǰ�漸���ֽ�,���õ���������
*/

struct VmemCB
{
	uint32_t mulriple;//����ʵ��СΪmulriple*min_block_size
	struct memCB *Hmem;
	struct VmemCB *pre;
	struct VmemCB *next;
};

#define max_level_vmem	(6+1)	//�����Ҫ2��exp6,buf����Ҫ��7��Ԫ��.��Ϊ�Ǵ�0��ʼ�����.
struct VmemCB s_list_vmem[max_level_vmem];	
struct VmemCB *s_pr_list_vmem=s_list_vmem;
//���sizeΪ128*2exp(6)=128*64;
//��СsizeΪ128*2exp(0)=128*1


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

//��ʼ����ֱ����
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
//��ʼ�����


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

//����bit[]����ֵ,���������ڴ��,���ڴ����ص���ֱ������
//!ע��:��ֱ�����е�size��min_block_size��N��,�ض���2��M��.��bit[i]�е�iҲ��min_block_size��2��i��--�мǽ���ת��.
	for(i=0;i<max_level_vmem;i++)
	{
		//����ˮƽ�������ʵĴ�ֱ������.��ֱ������ڴ����ȡֵ
		//ˮƽ�������лػ�����
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
			else//!���µĽڵ���뵽��ǰ��.//��ɾ��ʱ,Ҳ�ӵ�һ���ڵ㿪ʼɾ��.�����ٽ�����ѵ������.
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


/*��ȡsize��С���ڴ�*/
//!size�ض���min_block_size����,���������,���ں����ڲ�����Ϊ������ϵ.
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

	//���ҵ����ʵ��ڴ��
	printf("addr=%p size=%d\n",prH,prH->size);
	//��ʣ��Ŀռ���뵽Vlist��
	leftsize=prH->size-size;
	printf("leftsize=%d  %x\n",leftsize,leftsize/min_block_size);
	leftsize=leftsize/min_block_size;
	//��leftsize��min_block_size�ı���,�ҵ����������,Ȼ����ݶ�������,��ʣ����ڴ����뵽Vlist��
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