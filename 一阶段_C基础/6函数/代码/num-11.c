#include <stdio.h>
  double Factorial(int n);        //递归函数说明
int main(void)
  {
	double r=Factorial(5);  //调用函数Factorial并将返回值 赋给r
	printf("5!=%f\n",r);
  }
  double Factorial(int n)        //定义求n!函数，要求n大于等于零
  {
	if(n<1)  return(1);        //递归函数终止条件
	return( n*Factorial(n-1) );  // Factorial函数的函数体中调用了Factorial函数
  }

