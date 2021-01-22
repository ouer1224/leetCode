
#include "stdlib.h"
#include "stdio.h"

#define loopOut(val,i,num)		do{\
	for(i=0;i<num;i++)\
	{\
		printf(" %d ",val);\
	}	\
}while(0)

#define loopAll(val,i,num)		do{\
	for(i=0;i<num;i++)\
	{\
		printf(" %d ",val);\
	}	\
}while(0)

#define msgout(a)	puts(a);


#define head_oneSet		0x80000000
#define tail_oneSet		0x7fffffff

#define init_array(array)	do{array[0]=0;array[1]=tail_oneSet;}while(0)

/*----------------------
规则:
数据的有效数据个数:[0]
array的结束:tail_oneSet

-----------------------*/
int f[8][48];

int array_append(int *dest,int *source)
{
	int tail_dest=0;
	int tail_source=0;
	
	tail_dest=dest[0]+1;
	

	source++;
	while(*source!=tail_oneSet)
	{	
		dest[tail_dest]=*source;
		
		tail_dest++;
		source++;
	}
	
	
	dest[tail_dest]=tail_oneSet;
	dest[0]=tail_dest-1;
	
	return tail_dest;
}
int array_addVal(int *dest,int val)
{
	int tail_dest=0;
	
	tail_dest=dest[0]+1;
	dest[tail_dest]=val;

	dest[tail_dest+1]=tail_oneSet;
	dest[0]++;
	
	return dest[0];	
}







int getFN(int *buf,int len,int tar,int delta)
{
	int *source=NULL;
	int *dest=NULL;
	int val_1st=buf[0];
	int j=0,i=0;
/*-------------------------------------------------*/	
	printf("delta=%d\n",delta);
	for(j=0;j<len;j++)
	{
		if(buf[j]>(val_1st+delta))
		{
			break;
		}
		//f[val_1st+1]=f[buf[j]]+f[val_1st+1-buf[j]];
		if(f[val_1st+delta-buf[j]][0]==0)
		{
			break;
		}
		else
		{
			dest=f[val_1st+delta];
			source=f[buf[j]];
			
			array_append(dest,source);
			
			source=f[val_1st+delta-buf[j]];
			
			array_append(dest,source);
			
	
			break;
		}
	}
	
	printf("f[%d][0]=%d\n",val_1st+delta,f[val_1st+delta][0]);
	loopAll(f[val_1st+delta][i],i,f[val_1st+delta][0]+1);	
	puts("\n");	
	return 1;
}





void combinationSum(int *buf,int len,int tar)
{
	int val_1st=buf[0];
	int val=0;
	int nums=tar+1;
	int i=0,j=0;
	int *pr=NULL;
	int *source=NULL;
	int *dest=NULL;
	int delta=1;
	
	pr=(void *)f;
	
	for(i=0;i<7;i++)
	{
		init_array(f[i]);
	}
	msgout("step1");
	array_addVal(f[0],0);
	msgout("step2");
	for(i=0;i<len;i++)//将f2,f3,f6,f7设置为其自身数值.正确其至少有一种实现的方式.其他数值暂时设置为-1.需要后续的计算
	{
		val=buf[i];
		printf("step3=%d\n",val);
		array_addVal(f[val],val);
		msgout("step3_end");
	}
	msgout("step4");
	printf("nums=%d\n",nums);
	loopOut(f[i][1],i,f[i][0]);

#if 1	//使用迭代方式
	for(i=1;i<=tar-val_1st;i++)
	{
		delta=i;
		getFN(buf,len,tar,delta);
	}


#else	//通过下面的每一步的运算,总结出上面的迭代方式
	
	for(j=0;j<len;j++)
	{
		if(buf[j]>(val_1st+delta))
		{
			break;
		}

		if(f[val_1st+delta-buf[j]][0]==-1)
		{
			break;
		}
		else
		{
			dest=f[val_1st+delta];
			source=f[buf[j]];
			while(*source!=-1)
			{
				*dest=*source;
				printf("%d\n",*source);
				source++;
				dest++;
			}
			source=f[val_1st+delta-buf[j]];
			while(*source!=-1)
			{
				*dest=*source;
				printf("%d\n",*source);
				source++;
				dest++;
			}
			
		}
	}
	
	printf("f[%d][0]=%d\n",val_1st+delta,f[val_1st+delta][0]);
	loopAll(f[val_1st+delta][i],i,4);
/*-------------------------------------------------*/	
	delta=2;

	for(j=0;j<len;j++)
	{
		if(buf[j]>(val_1st+delta))
		{
			break;
		}
		//f[val_1st+1]=f[buf[j]]+f[val_1st+1-buf[j]];
		if(f[val_1st+delta-buf[j]][0]==-1)
		{
			break;
		}
		else
		{
			dest=f[val_1st+delta];
			source=f[buf[j]];
			while(*source!=-1)
			{
				*dest=*source;
				//printf("%d\n",*source);
				source++;
				dest++;
			}
			source=f[val_1st+delta-buf[j]];
			while(*source!=-1)
			{
				*dest=*source;
				//printf("%d\n",*source);
				source++;
				dest++;
			}
		}
	}
	
	printf("\nf[%d][0]=%d\n",val_1st+delta,f[val_1st+delta][0]);
	loopAll(f[val_1st+delta][i],i,4);
	puts("\n");	
/*-------------------------------------------------*/	
	delta=3;

	for(j=0;j<len;j++)
	{
		if(buf[j]>(val_1st+delta))
		{
			break;
		}
		//f[val_1st+1]=f[buf[j]]+f[val_1st+1-buf[j]];
		if(f[val_1st+delta-buf[j]][0]==-1)
		{
			break;
		}
		else
		{
			dest=f[val_1st+delta];
			source=f[buf[j]];
			while(*source!=-1)
			{
				*dest=*source;
				//printf("%d\n",*source);
				source++;
				dest++;
			}
			source=f[val_1st+delta-buf[j]];
			while(*source!=-1)
			{
				*dest=*source;
				//printf("%d\n",*source);
				source++;
				dest++;
			}
		}
	}
	
	printf("f[%d][0]=%d\n",val_1st+delta,f[val_1st+delta][0]);
	loopAll(f[val_1st+delta][i],i,4);	
	puts("\n");	
	msgout("3 end\n");
/*-------------------------------------------------*/	
	delta=4;

	for(j=0;j<len;j++)
	{
		if(buf[j]>(val_1st+delta))
		{
			break;
		}
		msgout("step0");
		//f[val_1st+1]=f[buf[j]]+f[val_1st+1-buf[j]];
		if(f[val_1st+delta-buf[j]][0]==-1)
		{
			break;
		}
		else
		{
			dest=f[val_1st+delta];
			source=f[buf[j]];
			msgout("step1");
			while(*source!=-1)
			{
				*dest=*source;
				//printf("%d\n",*source);
				source++;
				dest++;
			}
			msgout("step2");
			source=f[val_1st+delta-buf[j]];
			while(*source!=-1)
			{
				*dest=*source;
				//printf("%d\n",*source);
				source++;
				dest++;
			}
			msgout("step3");
			break;
		}
	}
	
	printf("f[%d][0]=%d\n",val_1st+delta,f[val_1st+delta][0]);
	loopAll(f[val_1st+delta][i],i,4);	
	puts("\n");	
	
/*-------------------------------------------------*/	
	delta=5;

	for(j=0;j<len;j++)
	{
		if(buf[j]>(val_1st+delta))
		{
			break;
		}
		msgout("step0");
		//f[val_1st+1]=f[buf[j]]+f[val_1st+1-buf[j]];
		if(f[val_1st+delta-buf[j]][0]==-1)
		{
			break;
		}
		else
		{
			dest=f[val_1st+delta];
			source=f[buf[j]];
			msgout("step1");
			while(*source!=-1)
			{
				*dest=*source;
				//printf("%d\n",*source);
				source++;
				dest++;
			}
			msgout("step2");
			source=f[val_1st+delta-buf[j]];
			while(*source!=-1)
			{
				*dest=*source;
				//printf("%d\n",*source);
				source++;
				dest++;
			}
			msgout("step3");
			break;
		}
	}
	
	printf("f[%d][0]=%d\n",val_1st+delta,f[val_1st+delta][0]);
	loopAll(f[val_1st+delta][i],i,4);	
	puts("\n");		
#endif	
	
	
	
	
	
}







int main(void)
{
	int buf[]={2,3,6,7};
	int tar=7;
	
	combinationSum(buf,sizeof(buf)/sizeof(buf[0]),tar);
	
	printf("rt= \n");
	
	return 1;
}