#include <stdio.h>

void add(int a, int b)
{
    int c = a + b;
    printf("%d + %d = c: %d\n", a, b, c);

    return;
}

void sub(int a, int b)
{
    int c = a - b;
    printf("- c: %d\n", c);

    printf("sub of add: ");
    add(a, b);
    
    sub(a, b);

#if 0
    void add(int a, int b)
    {
        int c = a + b;
        printf("add define of sub: ");
        printf("%d + %d = c: %d\n", a, b, c);

        return;
    }

    add(a, b);
#endif

    return;
}

void mul(int a, int b)
{
    int c = a * b;
    printf("* c: %d\n", c);

    return;
}
void div(int a, int b)
{
    int c = a / b;
    printf("/ c: %d\n", c);

    return;
}
void mod(int a, int b)
{
    int c = a % b;
    printf("%% c: %d\n", c);

    return;
}

int main()
{
    int a = 3;
    int b = 4;

    //1.add
    add(a, b);

    //2.sub
    sub(a, b);

    //3.mul
    mul(a, b);

    //4.div
    div(a, b);

    //5.mod
    mod(a, b);

    return 0;
}
