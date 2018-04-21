#include <stdio.h>

int main()
{
    int num;
    int a;
    int b;
    int c;

    printf("Please input one three number: ");
    scanf("%d", &num);
    
    a = num / 100;
    b = num / 10 % 10;
    c = num % 10;

    printf("a: %d, b: %d, c: %d\n", a, b, c);

    return 0;
}
