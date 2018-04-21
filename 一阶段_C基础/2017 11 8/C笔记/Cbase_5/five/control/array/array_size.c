#include <stdio.h>

int main()
{
    int array[3];

    //array[0], array[1], array[2]
    printf("array[0]: %d\n", sizeof(array[0]));
    printf("array[1]: %d\n", sizeof(array[1]));
    printf("array[2]: %d\n", sizeof(array[2]));
    
    //array
    printf("array[3]: %d\n", sizeof(array[3]));// ?
    //printf("array[]: %d\n", sizeof(array[])); //error
    printf("array: %d\n", sizeof(array));

    //int a;
    //a type: int
    //int array[3]:
    //array type: int [3]

    //error define 
    //int [3] array;

    printf("array: %d\n", sizeof(int [3]));

    return 0;
}
