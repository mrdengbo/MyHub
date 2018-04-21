#include <stdio.h>

int main()
{
    char array[] = {'a', 'b', 'c'};
    int array_1[] = {'a', 'b'};

    char array_2[] = "abc";
    char array_2_1[] = {"abc"};//  {'a', 'b', 'c', '\0'}
    int i;
    
    for(i = 0; i < sizeof(array_2_1) / sizeof(array_2_1[0]); i ++)
    {
        printf("%c ", array_2_1[i]);
    }
    putchar('\n');

    printf("array_2_1: %s\n", array_2_1);

    return 0;
}
