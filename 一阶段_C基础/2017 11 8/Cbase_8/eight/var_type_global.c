#include <stdio.h>

int a = 5;
static int b = 6;

static void test()
{
    printf("a: %d\n", a);

    a ++;

    return;
}

//int a = 5;

//error
//printf("a: %d\n", a);

int main()
{
    int i = 0;

    test();

    while(i < 3)
    {
        test();
        printf("main: a: %d\n", a);
        i ++;
    }
    

    return 0;
}
