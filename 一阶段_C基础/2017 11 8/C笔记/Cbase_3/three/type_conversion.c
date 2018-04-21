#include <stdio.h>

int main()
{
    char a = 20;
    int b = 1000;

    b = a;

    printf("b: %d\n", b);

    b = 1000;
    //a = b;
    a = (char)b;
 
    //1110 1000 : ²¹Âë
    //24¸ö1 1110 1000£º²¹Âë
    //1 23¸ö0 0001 1000£ºÔ­Âë -- -24

    printf("a: %d\n", a);

    return 0;
}
