#include <stdio.h>

int main()
{
    int a = 3;
    int b = 4;
    int c;

    printf("a > b: %d\n", a > b);

    c = a > b;
    printf("c: %d\n", c);
    
    c = a <= b;//把关系运算符运算后的结果看成一个值（0：假 1：真）
    printf("c: %d\n", c);

    c = a == b;
    printf("c: %d\n", c);

    return 0;
}
