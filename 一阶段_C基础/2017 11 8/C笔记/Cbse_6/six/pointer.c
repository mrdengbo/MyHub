#include <stdio.h>

int main()
{
    char b;
    //&b: type: char *

    int a = 1;
    //a : type int
    //&a: type int *
    
    int *pointer = 2;
    //pointer: type int *
    //&pointer: int **

    printf("&a      : %p\n", &a);
    printf("&pointer: %p\n", &pointer);
    
    printf("a      : %d, %d, %#x\n", a, &a, &a);
    printf("pointer: %d, %d, %#x\n", pointer, &pointer, &pointer);

    return 0;
}
