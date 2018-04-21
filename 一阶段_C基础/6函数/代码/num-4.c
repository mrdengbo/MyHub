#include <stdio.h> 
       void Swap(double *x, double *y) ; // Swap函数声明
       int main(void)
       {
          double x1, x2;
          x1=21.56;
          x2=65.12;
	
	  printf("x1=%.2f,x2=%.2f\n",x1,x2);
          Swap(&x1, &x2);//调用Swap函数，交换x1和x2的值
          printf("x1=%.2f,x2=%.2f\n",x1,x2);
	  
       }

void Swap(double *x, double *y)        //实现x和y指向的目标
    {
         double temp;
          temp= *x;     *x = *y;       *y=temp;
     }



