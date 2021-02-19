
#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__



#ifndef NULL
	#define NULL ((void*)0)
#endif

#define os_true 			1
#define os_false			0
#define os_null_pr			0xffffffff


struct __link_list{
	struct __link_list  *pre;
	struct __link_list  *next;
};
#if 0
#define __swap(a,b)	do{if(sizeof(a)!=sizeof(b))\
					{while(1);}a=a^b;b=a^b;a=a^b;}while(0)
#else
#define __swap(a,b)     do{a=a^b;b=a^b;a=a^b;}while(0)
#define swap(a,b)   \
do{\
    int i_qwer=0;\
    if(sizeof(a)!=sizeof(b))\
    {\
        while(1);\
    }\
    for(i_qwer=0;i_qwer<sizeof(a);i_qwer++)\
    {\
        __swap((*(((char *)((char *)(&a))+i_qwer))),(*(((char *)((char *)(&b))+i_qwer))));\
    }\
}while(0)

#endif
#define __offsetof(type,member)	(size_t)(&(((type *)0)->member))

#define container_of(pr_list,type,member)	__container_of(pr_list,__offsetof(type,member))



int list_add_behind(struct __link_list *new_list,struct __link_list *current);
void * __container_of(void *cur,int offset);


int list_del(struct __link_list *current);




#endif

