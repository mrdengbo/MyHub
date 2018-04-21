 #include <stdio.h>
void Swap(double x, double y)
    {
         double temp;
          temp=x;
          x=y;
          y=temp;
               
     }
     
int main(void)
     {
         double x1, x2;
         x1=10.56;
         x2=8.5;
         
	 printf("x1=%lf\nx2=%lf\n",x1,x2);
         Swap(x1, x2);
         printf("x1=%lf\nx2=%lf\n",x1,x2);
	
	return 0;
     }

