#include <stdio.h>
  
int main()
{
    //2      : type int
    //pointer: type int *
    //write
    int *pointer = (int *)0x12345678;
    int a = 5;
    //b: type int
    //&b: type int *
    int b;

    printf("pointer: %d\n", (int)pointer);
    printf("pointer: %p\n", pointer);

    //read
    //printf();
    //b = *(int *)0x12345678;
    printf("b: %d\n", b);

    //&a
    printf("&a: %p\n", &a);
    //b = *(int *)0xbfa37744;
    //b = *(int *)&a;
    b = *&a;
    printf("b: %d\n", b);

    return 0;
}
