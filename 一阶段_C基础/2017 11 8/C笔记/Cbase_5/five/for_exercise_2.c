#include <stdio.h>

int main()
{
    int i;
    int sum = 0;

    for(i = 1; i <= 100; i ++)
    {
        sum += i;//sum = sum + i;
    }
    printf("sum: %d\n", sum);

    int temp = 0;
    sum = 0;

    for(i = 0; i < 6; i ++)
    {
        temp = temp * 10 + 6;
        printf("temp: %d\n", temp);

        sum += temp;
    }
    printf("sum: %d\n", sum);
    
    int n;
    int temp_1 = 0;
    sum = 0;

    printf("Please input one number[1-9]: ");
    scanf("%d", &n);

    for(i = 0; i < n; i ++)
    {
        temp_1 = temp_1 * 10 + n;
        printf("temp_1: %d\n", temp_1);

        sum += temp_1;
    }
    printf("sum: %d\n", sum);

    return 0;
}
