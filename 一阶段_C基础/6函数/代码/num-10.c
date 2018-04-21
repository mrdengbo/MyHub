#include <stdio.h>
     int  Plus(int a, int b);			//计算加法函数的说明
     int  Minus(int a, int b);             	//计算减法函数的说明
int main(void)
    {
	      	int  (*pFunc[2])(int a,int b); 
		int i;
          	pFunc[0] = Plus; 
	      	pFunc[1] = Minus;
		
	for( i=0; i<2; i++)
		printf("%d\n", (* pFunc[i])(15,85) );
	}
	
int Plus(int a, int b)
  	{
		return(a+b);
  	}
	
int Minus(int a, int b)
  	{
		return(a-b);
  	}

