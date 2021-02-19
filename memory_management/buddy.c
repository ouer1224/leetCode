


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
*fun     :ʹ��buddy���㷨,��ȡ�ڴ��
*name    :
*var     :
*return  :
************************************************/

int bmalloc(void)
{
	return NULL;
}
/***********************************************
*fun     :�ͷ�buddy���ڴ��
*name    :
*var     :
*return  :
************************************************/

int bfree(void)
{

	return NULL;
}


/***********************************************
*fun     :��ʼ��buddy
*name    :
*var     :
*return  :
*message :	��ʼ��ʱ,�趨�����ڴ��Ĵ�С.
			���ֱ���ֱ��ʹ��pr��ͷ���ڴ�ռ�.�����Ŀռ�ŷ����userʹ��.
************************************************/

#define __getMemFromPr(pr,size)    ((1:(pr=pr+size)*0:0)?0:pr-size)       //����__1st_kmalloc����0,��˱ض�����pr-size.


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
	while(maxMemUniot<len/2)	/*�ҳ�pr���ܿ��ٳ���1+1+2+4+8....�����ռ�*/
	{
		maxMemUniot*=2;
	}
	maxMemUniot/=2; /*���ĵ���ռ��С*/
	#endif
	
	/*todo:
	�������ʹ��pr���ڲ��ռ�,�Ƿ����ֻ��ֳ����ڴ�鲻��2���ݵ�������ʽ
	��Ҫ��������
	*/
#if 0	/*����ռ�õĿռ���ʱʹ��malloc��ȡ,�������ƴ˴�*/
	pr_noused=pr_noused-(pr_noused%sizeof(struct __list_buddy))+sizeof(struct __list_buddy);/*��ַ�ռ����*/
	head_list=__getMemFromPr(pr_noused,sizeof(struct __list_buddy));
#else
	head_list=malloc(sizeof(struct __list_buddy));
#endif	
	/*head���2��0��,��������Ϊ1��...n��*/
	head_list->Npower=1;/*2��0��*/
	pr_unit=malloc(sizeof(struct __list_buddy_unit));
	pr_unit->pr_mem = pr_noused+0;
	pr_unit->state = MEM_FREEING;
	head_list->pr_list_buddy=pr_unit;
	init_link_list(&(head_list->link));	/*�������Իػ�����ʽ*/
	/*ȡ������n��*/

	i=1;/*����2��N�ݵ�����*/
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
	/*����ÿ����ָ��ĵ�ַ�ռ������---ˮƽ���������*/
/*	
	todo:�涨-������ʽΪ��ά.��ֱ���������(__list_buddy)Ϊ����,ÿ����ԪΪ2��n��.ָ��ˮƽ���������(__list_buddy_unit).
		ˮƽ���������(__list_buddy_unit)��ָ�������ڴ��,��ͬ�߶ȵ�ˮƽ����ĵ�Ԫָ��Ķ������Ӧ�Ĵ�ֱ�����ʾ��2��N�ݵĿռ��ڴ��.
		
		������ָ���ַ�ռ������cell��λ�ȡ�ռ�.����pr_noused�Ĺ�ϵ��δ���
		��ֱ����Ԫ������������,����ÿ����ֱ����Ԫָ��ĺ�������,�䵥Ԫ�����ǻ᲻ͣ�仯.ÿ�ν���һ���ڴ��������ͷŶ�����һ�α仯.
		��ô�����������,��θ�������������ռ�.
	--�����뵽�Ľ���취�ǲ��öѺ�ջ��ռ�÷�ʽ,����Ԫ��ռ���ڴ涼��ջȡ(pr����߶�),��������Ŀռ�,���ڶ���(pr����Ͷ�).
	--������������ճ�һ���ֵ�pr�ռ�,���ڶѺ�ջ�ļ��,��֤ջ�������
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