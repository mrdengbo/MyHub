#include <stdio.h>

int main()
{
    int a;
    //int i = 0;
    int i = 31;
    float b;

    scanf("%d", &a);
    //scanf("%f", &b);

    //while(i < 32)
    while(i >= 0)
    {
        //if((a & (1 << i)) == (1 << i))
        if(a & (1 << i))
        //if(b & (1 << i))
        {
            putchar('1');
        }
        else
        {
            putchar('0');
        }

        //i ++;
        i --;
    }
    putchar('\n');

    return 0;
}
