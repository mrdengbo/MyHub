#include <stdio.h>

int main()
{
    int a = 0;

    while(a < 5)
    {
        printf("a: %d\n", a);

        a ++;
    }
    printf("a: %d\n", a);

    a = 0;

    while(a ++)//while(a); a = a + 1
    //while(++ a)
    {
        printf("=a: %d\n", a);
    }
    printf("-a: %d\n", a);

    return 0;
}
