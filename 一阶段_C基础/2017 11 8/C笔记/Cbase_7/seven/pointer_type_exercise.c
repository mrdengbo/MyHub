#include <stdio.h>

int main()
{
    unsigned int x = 0x12345678;
    char *p = (char *)&x;
    short *q = (short  *)&x;
    int i, sum = 0;

    for(i = 0; i < 4; i ++)
    {
        sum = sum + *p;
        
        p ++;
    }
    printf("sum : %d, %#x\n", sum, sum);

    sum = 0;
    for(i = 0; i < 2; i ++)
    {
        printf("*q: %#x\n", *q);
        
        sum += *q;
        
        printf("sum: %d, %#x\n", sum, sum);

        q ++;
    }
    printf("sum: %d, %#x\n", sum, sum);

    return 0;
}
