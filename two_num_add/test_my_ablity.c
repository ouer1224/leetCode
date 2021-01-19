


int stack_init(int deep,int size,infor_stack *stack);
int stack_push(infor_stack *stack,void *dat,int size);
int stack_pop(infor_stack *stack,void *dat,int size);
int stack_deep(infor_stack *stack);


#define init_stack()	
#define push_stack(l,r)
#define pop_stack()
#define deep_stack()


int binSort(int *buf,int num)
{
	int left=0,right=num;
	int pre_l=0,pre_r=0;
	int pit=0;
	int pos='r';
	init_stack();
	push_stack(left,right);//初始条件
	
	while(deep_stack()!=0)
	{
		left,right=pop_stack();//取出需要排序的区间
		pre_l=left;
		pre_r=right;
		pit=buf[left];
		pos='r';
		while(left<right)//区间为左闭右开,因此第一次时,right的数据是不可用的,必须要移动.
						//由于left的数据被赋值给了pit,也变成了(坑),因此在对left的数据进行判断时,left也得移动.
						//由于left的数据被挖了坑,因此第一次比较时,要使用right的数据进行比较,是为了将right的数据放入到坑中.
		{
			if(pos=='r')
			{
				right--;
				if(buf[right]<pit)
				{
					buf[left]=buf[right];
					pos='l'
				}
			
			}
			else
			{
				left++;
				if(buf[left]>pit)
				{
					buf[right]=buf[left];
					pos='r';
				}	
			}
		}
		if(left>pre_l)
		{
			push_stack(pre_l,left);		
		}
		if(pre_r>(right+1))
		{
			push_stack(right+1,pre-r);
		}

		
		
	}
	

	
}