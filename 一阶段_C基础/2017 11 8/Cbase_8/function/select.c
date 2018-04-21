#include <stdio.h>

//MyPrintf
void MyPrintf(int *a)
{
    int i;

    for(i = 0; i < 5; i ++)
    {
        printf("%d ", *(a + i));
    }
    putchar('\n');

    return;
}

void sort(int *a)
{
    int i, j, k;

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
            MyPrintf(a);
        }
        
        //论证
        if(k != i)
        {
            a[k] ^= a[i];
            a[i] ^= a[k];
            a[k] ^= a[i];
        }
    }
    return;
}

int main()
{
    int a[] = {5, 4, 3, 2, 1};

    MyPrintf(a);
    
    sort(a);
    
    MyPrintf(a);

    return 0;
}
