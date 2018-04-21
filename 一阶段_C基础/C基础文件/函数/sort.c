#include <stdio.h>
#include <stdlib.h>
int sort( int* a, int num)
{
  
    int i = 0;
    int j = 0;
  

    for(i = 0; i < num - 1; i ++)
    {
        for(j = 0; j < num -i- 1 ; j ++)
        {
            if(a[j]  <  a[j + 1]) 
            {
                
               a[j ] ^= a[j + 1] ;
               a[j+1] ^= a[j];
               a[j] ^= a[j + 1];
           
            }
                   
        }
     
    
    }
    return 0;

}


int main()
{
    int a[10] ;
    int i = 0;
  

    printf("Please input ten num:");
    for(i = 0; i < 10; i ++)
    {
        scanf("%d",&a[i]);
        printf("i : %d\n", i);
    
    }

    for(i = 0; i < 10; i ++)
    {
        printf("a[%d] : %d\n", i, a[i]);
    
    } 
    
    sort(a , 10);

    for(i = 0; i < 10; i ++)
    {
         printf("a[%d] : %d\n", i, a[i]);
    
    
    }

   
    return 0;


}
