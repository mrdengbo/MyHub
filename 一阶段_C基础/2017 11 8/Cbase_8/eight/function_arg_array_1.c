#include <stdio.h>

void test(int **p)
{
    static int a[] = {1, 2, 3, 4};
    
    *p = a; 

    return;
}

int main()
{
    int i;
    int *p;

    test(&p);

    for(i = 0; i < 4; i ++)
    {
        printf("%d ", p[i]);
    }
    putchar('\n');

    return 0;
}
