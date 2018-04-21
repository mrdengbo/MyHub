#include <stdio.h>

//void add(int a, int b, int d)
//void add(int a, int b, int *d)//int *d = &d;//左边的d是形參，右边的d是实參。
void add(int a, int b, int *e)//int *e = e = &d;//左边的e是形參，右边的e是实參，再右边的d是实參。
{
    int c = a + b;
    //printf("c: %d\n", c);
    
    //c: int
    //d: int *
    //d = c;

    //(int *)7
    //d = (int *)c;
    //printf("d: %p\n", d);

    //printf("add: d: %p\n", d);
    //printf("add: &d: %p\n", &d);
    //*d = c;

    //printf("e: %p\n", e);
    //*e = c;

    e = &c;

    return;
}

int main()
{
    int a = 3;
    int b = 4;
    int d;
    int *e = &d;

    //add(a, b);
    //printf("c: %d\n", c);

    //add(a, b, d);
    //printf("d: %d\n", d);

    //d: type int
    //&d: type int *

    printf("&d: %p\n", &d);
    //add(a, b, &d);
    //printf("d: %d\n", d);

    add(a, b, e);
    printf("d: %d\n", d);
    printf("*e: %d\n", *e);

    return 0;
}
