/*
Ŀǰ��Ҫ��̬���ٵı����ռ�ʹ��malloc
������취ʹ�������ķ�ʽ.
����:
�Ƿ����ֱ�ӿ��ٳ��㹻�ı���buf,Ȼ��init��mallocʹ��.���÷���free�����жԱ����ռ���ͷ�.
*/
/*
2021��2��20�� 15ʱ14��
���ڱ�д����int����.
�����дmalloc�Ĳ���
*/
/*
time:2021-02-25   14:25:44
���������һ�������ݵ�ê��. 
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
#define ANCHOR			0xff

#define MAX_MEM_UNIOT_BUDDY		1024


struct __list_buddy *s_phead_buddy=NULL;


#define __getMemFromPr(pr,size)    ((1:(pr=pr+size)*0:0)?0:pr-size)       //����__1st_kmalloc����0,��˱ض�����pr-size.

#define prToUint32(pr)		((uint32_t)(pr))
#define SizeBuddy			sizeof(struct __list_buddy)
#define SizeBuddyUnit		sizeof(struct __list_buddy_unit)


#define getNextBuddy(pr_list)		container_of((pr_list->link.next),struct __list_buddy,link);
#define getNextBuddyUnit(pr_list)		container_of((pr_list->link.next),struct __list_buddy_unit,link);


/***********************************************
*fun     :ʹ��buddy���㷨,��ȡ�ڴ��
*name    :
*var     :
*return  :
*attention:ˮƽ����������ê���,ê�㲻Я���κ�����,�������ӺͶ�λ 
:��initʱ,��С�ڴ�Ϊ1.������malloc��free����С�ڴ�Ҳ��1.
todo:�����޸�Ϊ2��m�� 
************************************************/

void * bmalloc(uint32_t size)
{
	void *pr=NULL;
	struct __list_buddy *pr_list=NULL;
	struct __list_buddy_unit *anchor_unit=NULL;
	struct __list_buddy_unit *pr_unit=NULL;
	uint32_t mem_remain=0;
	
	printf("ram_size=%d\n",size);
	pr_list=s_phead_buddy;
	do
	{
		if(pr_list->Npower>=size)/*��ȡ�����ʵ��ڴ�Ĵ�ֱ����*/
		{
			mem_remain=pr_list->Npower;
			anchor_unit=pr_list->pr_list_buddy;
			pr_unit=getNextBuddyUnit(anchor_unit);	
			if(pr_unit==anchor_unit)
			{
				pr=NULL;
			}
			else
			{
				pr=pr_unit->pr_mem;
				list_del(&(pr_unit->link));
				free(pr_unit);
				break;
			}

		}
		pr_list=getNextBuddy(pr_list);
	}
	while(pr_list!=s_phead_buddy);
	if(pr_list==s_phead_buddy)
	{
		if(pr_list->Npower<size) //except 
		{
			return NULL;	
		}
	}	
	printf("satisfy_N=%d\n",pr_list->Npower);
	
	/*��ʣ����ڴ�ռ���ص���ֱ������*/
	/*����ʣ��ռ�Ķ��ǰ��յ�ַ�����,���ʣ��Ŀռ���԰��ն�����ֱ��ӳ�䵽��ֱ������*/
	mem_remain-=size;
	pr_list=s_phead_buddy;
	while(mem_remain>0)
	{
		if((mem_remain&0x01)==1)
		{
			pr_unit=malloc(SizeBuddyUnit);
			pr_unit->state=MEM_FREEING;
			pr_unit->pr_mem=pr+size;
			list_add_before(&(pr_unit->link),&(pr_list->pr_list_buddy->link));
			size+=pr_list->Npower;
		}
		else
		{
		
		}
	
		mem_remain=mem_remain>>1;
		pr_list=getNextBuddy(pr_list);
		if(pr_list==s_phead_buddy)	//todo:��̫���ܳ������������,���ǳ���ʱ,��ʾ����err�� 
		{
			break;
		}
	}
	

		
	return pr;
}

/***********************************************
*fun    :�ж�����n���ڴ���Ƿ���Ժϲ���2n�Ĵ��ڴ�� 
*name   :
*var    :
*return :
************************************************/ 
void * incorporate_mem(void *prx,void *pry,uint32_t size)
{
	uint32_t tmp=0;
	
	tmp=prToUint32(prx)-prToUint32(pry);
	if(tmp>0)
	{
	
	}
	else
	{
		tmp=0-tmp;
		swap(prx,pry);	/*ȷ��x����y*/
	}
	if(tmp!=size)
	{
		goto err_exit;
	}
	if((prx%(2*size))!=0)	/*����2n�ռ�,���ַ������*/
	{
		goto err_exit;	
	}
	
ok_exit:	
	return prx;	

err_exit:
	return NULL;

}

/***********************************************
*fun     :�ͷ�buddy���ڴ��
*name    :
*var     :
*return  :
todo:�ϲ�ʱ,�Ƿ���Ҫ��ע��ַ����?? 
************************************************/

int bfree(void *pr)
{

	/*�ͷ�prʱ,��pr���ص���ֱ�����е���ȷ�ڵ���,�����ҵ�������pr��ͬsize��С��free�ڴ��,Ȼ��bin����
	�����,��ʹ��һ��ѭ��,�����Ƿ��ܺϲ����ڴ��*/	
	//˫����������������,����ɾ���ڵ��,ֱ�Ӳ������λ�ü���.
		 


	/*�����ܺϲ����ڴ���,��ȥ��������ͬsize��С���ڴ��,Ȼ���ظ�����Ĳ���*/ 


	return NULL;
}

/***********************************************
*fun     :��һ����ֵ��2�ĵ� 
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
*fun     :��ʼ��buddy
*name    :
*var     :
*return  :
*message :	��ʼ��ʱ,�趨�����ڴ��Ĵ�С.
			���ֱ���ֱ��ʹ��pr��ͷ���ڴ�ռ�.�����Ŀռ�ŷ����userʹ��.
************************************************/



struct __list_buddy * binit_mem(uint8_t *pr,uint32_t len)
{
	uint32_t i=0;
	uint32_t remain=0;
	uint32_t tmp=0;
	uint32_t npow=0;
	uint32_t maxMemUniot=0;
	uint8_t *pr_start=pr;	/*����������ҿ�-- [ pr_start,pr_end ) */
	uint8_t *pr_end=pr+len;
	struct __list_buddy *pr_list=NULL;
	struct __list_buddy *head_list=NULL;
	struct __list_buddy *tail_list=NULL;
	struct __list_buddy_unit *pr_unit=NULL;
	struct __list_buddy_unit *anchor_unit=NULL; 
	/*
�����µķ������,����:
���ݵ�ǰ��len,�������(2��n��-1),���Ӧ���Ƕ����Ƶ�1111��������ֵ.
����������,�õ�remain,�����remain�Ķ�������ֵ.
��������(2��n��-1)�����ֵ�����λ��,����Ǵ�ֱ����ĸ���.
Ȼ��ֱ�����ÿ����Ԫ�����һ��ˮƽ����,��ָ����Ǹõ�Ԫ��Ӧ�Ķ����Ƶ�λ,����ʾ��2��n�ݴ�С.��,��0λ:2��0��.��1λ:2��1��....
�ڽ�������ʼ����ɺ�.
�ڽ�remain����Ķ����Ƶ���ֵ,���Ӧ�Ĵ�ֱ����ĵ�Ԫ,��ָ���ˮƽ������,����µ�һ��ָ��ô�С�ڴ�ĵ�Ԫ.

������Ҫ���ǵ�ַ��������,�����Ҫ�Ƚ�pr�ĵ�ַ������Ƶ������ڴ����е�ַ����.
Ȼ���ٽ�������������.
���ڵ�ַ�����ڿ��е��ڴ�ռ�,���԰��� "�μ�����ڴ��" ���ж���.�ٷ���һ��.
ѭ��,ֱ������2���ж����,��ʣ����ڴ��ٰ���1����һ�μ���.

��������˼��:��(����2��n��),��ʵ�����ҳ���������ĵ�ַ����.
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
	
	/*�������ĵ�ַ,��ʼ����������*/
	//��ȡ��ֱ����ĵ�Ԫ������,���ڿ��ٿռ� 
	tmp=maxMemUniot;
	for( ; tmp >0 ; tmp /=2)
	{
		i++;
	}
	printf("all = %d \n",i);
	head_list=malloc(i*SizeBuddy); 

	anchor_unit=malloc(SizeBuddyUnit);
	anchor_unit->state=ANCHOR;
	anchor_unit->pr_mem=NULL;
	init_link_list(&(anchor_unit->link));
	head_list[0].Npower=1;
	head_list[0].pr_list_buddy=anchor_unit;
	init_link_list(&(head_list[0].link));
	
	pr_unit=malloc(SizeBuddyUnit);
	pr_unit->state=MEM_FREEING;
	pr_unit->pr_mem=pr_start+0;
	list_add_before(&(pr_unit->link),&(head_list[0].pr_list_buddy->link));

	
	pr_unit=malloc(SizeBuddyUnit);
	pr_unit->state=MEM_FREEING;
	pr_unit->pr_mem=pr_start+head_list[0].Npower;
	list_add_before(&(pr_unit->link),&(head_list[0].pr_list_buddy->link));
	
	for(npow=1;npow<i;npow++)
	{
		anchor_unit=malloc(SizeBuddyUnit);/*��ʼ��ê��*/
		anchor_unit->state=ANCHOR;
		anchor_unit->pr_mem=NULL;
		init_link_list(&(anchor_unit->link));

		head_list[npow].Npower=head_list[npow-1].Npower*2;/*��ʼ����ֱ����*/ 
		head_list[npow].pr_list_buddy=anchor_unit;
		list_add_before(&(head_list[npow].link),&(head_list[0].link));	
		
		pr_unit=malloc(SizeBuddyUnit);/*���ڴ��������ê��ҹ�����*/
		pr_unit->state=MEM_FREEING;
		pr_unit->pr_mem=pr_start+head_list[npow].Npower;
		list_add_before(&(pr_unit->link),&(head_list[npow].pr_list_buddy->link));
	}
	
	/*�� [ pr, pr_start )���ڴ�,����ҿ� ,��ӵ�������*/ 
	/*ֻҪȷ��maxMemUniot��[ pr, pr_start )�����,�������������̾��ܱ�ΪѰ��һ�����ݵĶ�����*/

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
			//֤��[ pr, pr_start )������null 
			printf("\nnull\n");
		}
	
	}
	
	
	/*�� [pr_end , pr+len )���ڴ�,����:����ҿ� , ��ӵ�������*/ 
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
			//֤��[ pr, pr_start )������null 
			printf("\nnull\n");
		}
	
	}

	s_phead_buddy=head_list;

	return head_list;
}

void show_mem(struct __list_buddy * pr_head)
{
	uint32_t i=0;
	uint32_t j=0;
	struct __list_buddy *pr_list=NULL;
	struct __list_buddy_unit *pr_unit=NULL;
	struct __list_buddy_unit *anchor_unit=NULL;

	for(i=0;i<10;i++)
	{
		anchor_unit=pr_head[i].pr_list_buddy;
		pr_unit=getNextBuddyUnit(anchor_unit);
		j=0;
		while(pr_unit!=anchor_unit)
		{
			j++;
			printf("%d-npow=%d pr_mem=%p \n",i,pr_head[i].Npower,pr_unit->pr_mem);
			pr_unit=getNextBuddyUnit(pr_unit);
		}
		printf("				--%d pow==%d count\n",pr_head[i].Npower,j);
	}

}

static char s_mbuf[1023];
int main(void)
{
	uint32_t i=0;
	struct __list_buddy *pr_head=NULL;
	struct __list_buddy *pr_list=NULL;
	struct __list_buddy_unit *pr_unit=NULL;
	struct __list_buddy_unit *anchor_unit=NULL;
	void *pr=NULL;
	pr_head=binit_mem(s_mbuf,sizeof(s_mbuf));
	
	printf("\n--------------0 st-------------------\n");
	show_mem(pr_head);
	for(i=0;i<10;i++)
	{
		pr=bmalloc(3);
		printf("malloc_mem=%p\n",pr);
		printf("\n--------------%d-------------------\n",i+1);
		show_mem(pr_head);
	}
	
	
	
	return 0;
}
