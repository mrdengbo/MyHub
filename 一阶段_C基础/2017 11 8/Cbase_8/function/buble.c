#include <stdio.h>

//printf
void MyPrintf(int *p)
{
    int i;

    for(i = 0; i < 5; i ++)
    {
        printf("%d ", *p ++);
    }
    putchar('\n');
    
    return;
}

//swap
void swap(int *a, int *b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;

    return;
}

//sort
void sort(int a[])
{
    int i, j;

    for(i = 0; i < 5 - 1; i ++)
    {
        for(j = 0; j < 5 - i - 1; j ++)
        {
            if(a[j] > a[j + 1])
            {
                //swap
                swap(&a[j], &a[j + 1]);
            }
            
            //MyPrintf
            printf("i: %d, j: %d == ", i, j);
            MyPrintf(a);
        }
    }

    return;
}

int main()
{
    int i, j, k;
    int a[] = {5, 10, 26, 90, 1};
    int *p = a;
    
    //MyPrintf
    MyPrintf(p);

    //sort
    sort(p);
    
    //MyPrintf
    MyPrintf(p);

    return 0;
}
