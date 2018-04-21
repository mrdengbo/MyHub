#include <stdio.h>

int  search(int * pa, int n,int *pmax, int* pflag)
{
    
    int i = 0;

    for(i = 0; i < n; i ++)
    {
      if(*pmax < *pa)  
        {
            pmax = pa;
            *pflag = i;  
            
        }

        pa ++;
    }
    return 0;

}


int main()
{
    int  a[10];
    int  i = 0;
    int  *max = &a[0];
    int  index = 0; 

    for(i = 0; i < 10; i ++)
    {
        scanf("%d", &a[i]);
    
    
    }
    
    for(i = 0; i < 10; i ++)
    {
        printf("%d ", a[i]);
    
    }
    
    putchar('\n');
    search(a, 10, max, &index);

    printf("max : %d, index : %d", *max, index);

    return 0;
}
