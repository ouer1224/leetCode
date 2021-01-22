
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

int f[7][4];


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
			break;
		}
	}
	
	printf("f[%d][0]=%d\n",val_1st+delta,f[val_1st+delta][0]);
	loopAll(f[val_1st+delta][i],i,4);	
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
	
	for(i=0;i<4*7;i++)
	{
		pr[i]=-1;
	}
	
	f[0][0]=0;
	
	for(i=0;i<len;i++)//将f2,f3,f6,f7设置为其自身数值.正确其至少有一种实现的方式.其他数值暂时设置为-1.需要后续的计算
	{
		val=buf[i];
		f[val][0]=val;
	}
	
	printf("nums=%d\n",nums);
	loopOut(f[i][0],i,nums);

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