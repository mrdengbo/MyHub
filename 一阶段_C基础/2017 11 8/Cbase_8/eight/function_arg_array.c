#include <stdio.h>

//void test(int *a)
//void test(int a[])
void test(int a[10000], int l)
{
    int i = 0;

    printf("sizeof(a): %d\n", sizeof(a));

    for(i = 0; i < l; i ++)
    {
        printf("%d ", a[i]);
    }
    putchar('\n');

    return;
}

//void test_1(int *a)
//void test_1(int (*a)[3])
void test_1(int a[][3])
{
    int i, j;
    int *b = (int *)a;
    
    printf("== sizeof(a): %d\n", sizeof(a));
#if 1
    for(i = 0; i < 6; i ++)
    {
        //printf("%d ", *a ++);
        printf("%d ", *b ++);
    }
#endif

#if 0
    for(i = 0; i < 2; i ++)
    {
        for(j = 0; j < 3; j ++)
        {
            printf("%d ", a[i][j]);
        }
    }
#endif

    putchar('\n');

    return;
}

int main()
{
    int a[] = {1, 2, 3, 4};
    int b[2][3] = {1, 2, 3, 4, 5, 6};

    test(a, sizeof(a) / sizeof(a[0]));
    //test_1((int *)b);
    test_1(b);

    return 0;
}
