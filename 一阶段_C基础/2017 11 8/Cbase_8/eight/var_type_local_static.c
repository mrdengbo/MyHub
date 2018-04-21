#include <stdio.h>

void test()
{
    static int a = 5;
    printf("a: %d\n", a);

    a ++;

    return;
}

int main()
{
    int i = 0;

    while(i < 3)
    {
        test();
        i ++;
    }

    return 0;
}
