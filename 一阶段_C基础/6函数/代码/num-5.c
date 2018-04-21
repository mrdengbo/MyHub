#include  <stdio.h>
int  n ;                      		 //定义一个全局变量n;
double  factorial (void) ; 	 // 说明了一个double型的函数factorial
int main(void)
     {
         double  s=0;    
         n=10 ;                    
         s = factorial(  );
	printf("%e\n",s);          	 //调用factorial函数；
} 
double  factorial (void)        	 //factorial函数功能为计算n! ；  
     {
          double r = 1;
	   int i;
          for(i=1; i<=n; i++) 
                r *= i;
           return r ;
     }

