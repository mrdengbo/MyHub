#include <stdio.h>

int main()
{
    //int a[4] ;
    int a[] = {1, 2, 3, 4};

    int *p = a;
    int (*y)[4] = (int (*)[4])a;
    //error
    //int [4] *q = &a;
    int (*q)[4] = &a;
    int *x      = (int *)&a;
    //0x12345678
    //     +   1
    //-----------
    //0x12345679
    int b       = (int)&a;

    printf("a : %p\n", a);
    printf("&a: %p\n", &a);
    printf("q : %p\n", q);
    printf("y : %p\n", y);

    printf("===================\n");
    //a: type int *
    printf("a + 1 : %p\n", a + 1);
   
    //int a[4];
    //a : type int [4]
    //&a: type int [4] * //int (*)[4]
    printf("&a + 1: %p\n", &a + 1);
    printf("q + 1 : %p\n", q + 1);
    printf("y + 1 : %p\n", y + 1);

    return 0;
}
