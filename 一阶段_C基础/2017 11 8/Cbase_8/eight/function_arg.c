#include <stdio.h>

void test_arg(int a);

void test_arg(int a)//int a = 5;//形參
{
    printf("&a: %p\n", &a);
    printf("a: %d\n", a);

    return;
}

void add(int b, int c)
{
    int d = b + c;
    printf("d: %d\n", d);

    return;
    printf("=====\n");
}

int main()
{
    //printf("5: %d\n", 5);
    //test_arg(5);

    int a = 5;//实參
    printf("&a: %p\n", &a);
    test_arg(a); 
    
    int b = 3;
    int c = 4;
    
    //1.顺序
    //2.明确目的
    add(b, c);

    return 0;
}
