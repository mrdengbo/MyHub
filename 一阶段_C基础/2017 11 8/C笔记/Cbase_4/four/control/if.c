#include <stdio.h>

int main()
{
    //int if;
    int a = 5;
    int b;
    
    b = a > 1;

    //if(a > 1)
    //if(b)
    //if(1000)
    if(a > 1 && a < 10)
    {
        printf("a > 1: %d\n", a);
    }

    if(a < 1)
    {
        printf("a < 1\n");
    }
    else
    {
        printf("a >= 1\n");
    }

    if(a > 1 && a < 2)
    {
        printf("1 < a < 2\n");
    }
    else if(a >= 2 && a <= 4)
    {
        printf("2 =< a <= 4\n");
    }
    else if(a <= 1 || a > 4)
    {
        printf("a <= 1 || a > 4\n");
    }
    else
    {
        printf("nothing\n");
    }

    return 0;
}
