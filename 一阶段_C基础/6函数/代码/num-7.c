#include <stdio.h>
double TestArray(double *pa, int Size);  //函数说明
int main(void)
{
      	double a[5]={1.1, 2.1, 3.1, 4.1, 5.1}, r;
	r=0;
        r=TestArray(a,5);              //将数组传递给函数  
      	printf("%.2f\n",r);
	
	return 0;
}

double TestArray(double *pa, int Size)  //形参为一个指针
        {
	 int i; 	
	 double s=0;
      	 for(i=0; i<Size; i++)         //注意：这里的pa[i]就是a[i]
	  	 s += pa[i];
	  	 return(s);
        }                                        
