#include <stdio.h>

//int test()
//char test()
int *test()
//int **test()
{
    //return 10;
    //return 'a';
    //return (int *)10;
#if 1
    static int a;
    a = 10;
    //return &a;
    
    static int *p;
    p = &a;

    //return p;
    //return &p;

    static int b[] = {1, 2, 3, 4};
    return a;
#endif
}

int main()
{
    int ret;
    int *ret_1;
    int **ret_2;
    int i;

    //ret = test();//ret = 10;
    //printf("ret: %d\n", ret);

    //ret_1 = test();
    //printf("=======\n");
    //printf("*ret_1: %d\n", *ret_1);

    //ret_2 = test();
    //printf("------\n");
    //printf("**ret_2: %d\n", **ret_2);

    ret_1 = test();
    for(i = 0; i < 4; i ++)
    {
        printf("%d ", ret_1[i]);
    }
    putchar('\n');

    return 0;
}
