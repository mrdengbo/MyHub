#include <stdio.h>

//总结：
//1.如果希望主函数中的指针改变指向，传指针的地址。
void add(int a, int b, int **e)//int **e = &e;
{
    //int c = a + b;
    static int c;
    c = a + b;

    //**e = c;
    //*e = &c;

    int *x = &c;
    *e = x;

    return;
}

int main()
{
    int a = 3; 
    int b = 4;
    int d;
    int *e = &d;
    int **f = &e;

    //add(a, b, &e);
    //printf("d: %d\n", d);
    //printf("*e: %d\n", *e);
    
    add(a, b, f);
    printf("d: %d\n", d);
    printf("*e: %d\n", *e);
    printf("**f: %d\n", **f);

    return 0;
}
