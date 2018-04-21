#include <stdio.h>

void test()
{
    int a = 5;

    printf("a: %d\n", a);
    //error
    //printf("b: %d\n", b);

    a ++;

    return;
}

int main()
{
    int b = 10;
    int i = 0;

    test();

    while(i < 3)
    {
        test();
        i ++;
    }
    
    //error
    //printf("a: %d\n", a);

    return 0;
}
