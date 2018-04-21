#include <stdio.h>

int main()
{
    int *pointer = (int *)0x12345678;
    int a = 10;
    int b;

    printf("&a      : %p\n", &a);
    printf("&pointer: %p\n", &pointer);

    //write
    pointer = &a;
    printf("pointer: %d, %#x\n", (int)pointer, (int)pointer);

    //read
    //*&a
    b = *pointer;
    printf("b: %d\n", b);

    printf("a: %d\n", a);
    *pointer = 20;
    printf("a: %d\n", a);

    return 0;
}
