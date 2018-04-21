#include <stdio.h>

int main()
{
    int i, j, k, m;
    int a[] = {5, 4, 3, 2, 1};

    for(i = 0; i < 5; i ++)
    {
        printf("%d ", *(a + i));
    }
    putchar('\n');

    for(i = 0; i < 4; i ++)
    {
        k = i;//假设第一个数是最大的

        //找到最小值的下标
        for(j = i + 1; j < 5; j ++)
        {   
            if(a[k] > a[j]) 
            {
                k = j;//k保存最小值的下标
            }
            
            //printf
            printf("i: %d, j: %d, k: %d == ", i, j, k);
            for(m = 0; m < 5; m ++)
            {
                printf("%d ", *(a + m));
            }
            putchar('\n');
        }
        
        //论证
        if(k != i)
        {
            a[k] ^= a[i];
            a[i] ^= a[k];
            a[k] ^= a[i];
        }
    }
    
    for(i = 0; i < 5; i ++)
    {
        printf("%d ", *(a + i));
    }
    putchar('\n');

    return 0;
}
