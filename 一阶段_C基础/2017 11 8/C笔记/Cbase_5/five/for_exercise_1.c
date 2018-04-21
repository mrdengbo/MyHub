#include <stdio.h>

int main()
{
    int n;//100 - 999: 999 - 100 + 1 = 900
    int a, b, c;


    for(n = 100; n <= 999; n ++)
    {
        a = n % 10;
        b = n / 10 % 10;
        c = n / 100;

        if(a * a * a + b * b * b + c * c * c == n)
        {
            printf("flower: %d\n", n);
        }
    }

    return 0;
}
