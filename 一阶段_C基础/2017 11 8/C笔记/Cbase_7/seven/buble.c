#include <stdio.h>

int main()
{
    int i, j, k;
    int a[] = {5, 10, 26, 90, 1};
    int *p = a;
    
    for(i = 0; i < 5; i ++)
    {
        printf("%d ", *p ++);
    }
    putchar('\n');

    for(i = 0; i < 5 - 1; i ++)
    {
        for(j = 0; j < 5 - i - 1; j ++)
        {
            if(a[j] > a[j + 1])
            {
                a[j] ^= a[j + 1];
                a[j + 1] ^= a[j];
                a[j] ^= a[j + 1];
            }
            
            //printf
            printf("i: %d, j: %d == ", i, j);
            p = a;
            for(k = 0; k < 5; k ++)
            {
                printf("%d ", *p ++);
            }
            putchar('\n');
        }
    }
    
    p = a;
    for(i = 0; i < 5; i ++)
    {
        printf("%d ", *p ++);
    }
    putchar('\n');

    return 0;
}
