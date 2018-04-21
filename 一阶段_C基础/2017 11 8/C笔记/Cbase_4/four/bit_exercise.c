#include <stdio.h>

int main()
{
    int a = 125;
    int b;

    b = a | (0x1 << 1);
    printf("b: %d\n", b);

    b = a & ~(0x3 << 1) | (3 << 1);
    printf("b: %d\n", b);

    b = a & ~(0xf << 3) | (0xa << 3);
    printf("b: %d\n", b);

    return 0;
}
