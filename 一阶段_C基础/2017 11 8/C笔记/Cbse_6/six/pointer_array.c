#include <stdio.h>

int main()
{
    int array[] = {1, 2, 3, 4};
    int *pointer;
    int i;

    //int a;
    //pointer = &a;

    //array[0]: int
    //&array[0]: int *
    //pointer = &array[0];
    pointer = array;

    //printf("array[0]: %d, *pointer: %d\n", array[0], *pointer);
    printf("pointer[0]: %d, *pointer: %d\n", pointer[0], *pointer);

    //pointer = &array[3];
    //printf("array[3]: %d, *pointer: %d\n", array[3], *pointer);
    
    pointer = &pointer[3];
    //printf("pointer[3]: %d, *pointer: %d\n", pointer[3], *pointer);
    printf("*pointer: %d\n", *pointer);

    pointer = array;
    for(i = 0; i < sizeof(array) / sizeof(array[0]); i ++)
    {
        //pointer = &array[i];
        //printf("array[%d]: %d, *pointer: %d\n", i, array[i], *pointer);
        printf("pointer[%d]: %d\n", i, pointer[i]);
    }

    printf("&array[0]: %p, array: %p\n", &array[0], array);

    return 0;
}
