#include <stdio.h>

int main()
{
    char a[] = {'a', 'b', 'c'};
    char *p = a;
    int i;

    for(i = 0; i < sizeof(a) / sizeof(a[0]); i ++)
    {
        putchar(*p ++);
        putchar(' ');
    }
    putchar('\n');

    return 0;
}
