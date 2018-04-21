#include <stdio.h>

int main()
{
    //int a = 3;
    //int b = 4;
    //int c;
    int a = 3, b = 4, c;

    c = a + b, a - b;
    printf("c: %d\n", c); //7

    c = (a + b, a - b);
    printf("c: %d\n", c); //-1

    return 0;
}
