#include <stdio.h>

int main()
{
    int a = 10;
    int *p = &a;
    int **q = &p;
    int ***x = &q;

    printf("*p  : %d\n", *p);
    printf("**q : %d\n", **q);
    printf("***x: %d\n", ***x);
    printf("sizeof(int **): %d\n", sizeof(int **));
    printf("sizeof(char *****************): %d\n", sizeof(char ***************));

    printf("==================\n");
    int **y = (int **)&a;
    printf("*y: %d\n", (int)*y);
    //segmentation fault
    //printf("**y: %d\n", **y);//*((int *)10)
    //error
    //printf("***y: %d\n", ***y);

    int ***m = (int ***)&p;
    printf("**m: %d\n", (int)**m);

    int ****n = (int ****)&q;
    printf("***n: %d\n", (int)***n);

    int *i = &a;
    int *h = &i;
    //error
    //printf("**h: %d\n", **h);
    printf("&a: %p\n", &a);
    printf("&i: %p\n", &i);
    printf("*h: %d, %#x\n", *h, *h);

    return 0;
}
