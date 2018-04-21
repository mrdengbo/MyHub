#include <stdio.h>
     double TestArray(double b[],int Size);
     int main(void)
       {
      	int i;
		double a[5]={1.1, 2.1, 3.1, 4.1, 5.1}, r=0;
            	r=TestArray(a,5);             
     		printf("%f\n",r);
     		r=0;        //在主函数中求出a数组的元素之和
     		for(i=0; i<5; i++)
		r += a[i];
     		printf("%lf\n",r);
		printf("%p\n",a);
      }  
double TestArray(double b[], int Size)
{
	printf("%p\n",b);
	double s=0;
	b++;
	int i;
    for( i=0; i<Size; i++)
		s += b[i-1];
	b[3]=0;
	return(s);
}  

