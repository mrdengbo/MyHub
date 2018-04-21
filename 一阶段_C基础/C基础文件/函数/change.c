#include <stdio.h>

void swap(int *a, int *b)
{
    int t ;
  //  printf("a : %d, b : %d\n", *a , *b);
    t = *b;
    *b = *a;
    *a = t;
  
   // printf("a : %d, b : %d\n", *a , *b);
}

void disp(int *a, int num)
{
    int i = 0;
    
    for(i = 0; i < num; i ++)
    {
        printf("%d  ", a[i] );
    
    }

}


int  Change(int *a, int num)
{
    int *p = a;
    int min = a[0];
    int max = a[0];
    int index_max = 0;
    int index_min = 0;
    int i = 0;

    for(i = 0; i < num; i ++)
    {
        if(max < *a)
        {
            max = *a;
            index_max = i;
        }

        if(min > *a)
        {
            min = *a;
            index_min = i;
        }
        a ++;
    
    }
    //printf("min : %d , max : %d\n",index_min,index_max);
   // printf("min : %d , max : %d\n" ,p[index_min] , p[index_max]);
    swap(&p[index_min],&p[0]);
    swap(&p[index_max],&p[9]);
    disp(p,10);
    return 0;

}

int main()
{
    int a[10];
    int i = 0;
    for(i = 0; i < 10; i ++)
    {
        scanf("%d", &a[i]);
    
    }

    disp(a, 10);
    putchar('\n');
    Change(a,10);
         
   
}
