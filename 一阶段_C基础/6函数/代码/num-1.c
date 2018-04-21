#include <stdio.h>

double  Power(double x,  int  n)     
		{                                  
		    if(  n<0  ) 
     		    {  
         	    printf("error\n"); 
        	    return(-111);   //如果是负整数，返回-111作为出错的状态值 
       	   	    }
               printf("hello,ok\n");
		int i;
        	double  r=1; //定义一个double型变量用以存放结果,并初始化为1
        	for( i=1; i<=n; i++)
                	r *=  x;
         	return( r ) ;        //要求返回double型结果值为r
     		}
int main(void)
{
	double x,y,z;
	int n;
      while (scanf("%lf%d",&x,&n)==2)
	{
	y=Power(x,n);
	printf("%lf\n",y);
 	}
	printf("Bye\n");
	
	return 0;
}

