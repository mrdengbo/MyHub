#include <stdio.h>

int main()
{
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;

    //scanf();
    //scanf();
    //scanf("%d%d%d...");
    for(i = 0; i < 10; i ++)
    {
        scanf("%d", &array[i]);
    }

#if 0
    array[0] = 1;
    array[1] = 2;
    .
    .
    .
    array[9] = 10;
    for(i = 0; i < 10; i ++)
    {
        array[i] = i + 1;
    }
#endif

    for(i = 0; i < 10; i ++)
    {
        printf("array[%d]: %d\n", i, array[i]);
    }

    return 0;
}
