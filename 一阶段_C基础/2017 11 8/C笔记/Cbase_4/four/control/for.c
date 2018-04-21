#include <stdio.h>

int main()
{
    int a;

    for(a = 1; a < 4; a ++)
    {
        printf("a: %d\n", a);
    }
    
    printf("a: %d\n", a);//4

    a = 2;

    //for(;;)
    //for(; a < 4; a = a + 3)
    //for(a = 2; ; a = a + 2)
    for(a = 2; a < 4; )
    {
        printf("*");
    }

    return 0;
}
