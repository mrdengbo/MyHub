#include <stdio.h>

int main()
{
    int a = 1, b = 2;
    int array[3];
    char array_1[3] = {a, b, 3};
    double array_2[] = {1.25, 1.23, 2.25};
    
    //read
    printf("array[0]: %d\n", array[0]);
    printf("array[1]: %d\n", array[1]);
    printf("array[2]: %d\n", array[2]);
    printf("array[3]: %d\n", array[3]);
    printf("array[100000]: %d\n", array[100000]);

    //write
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    //array[2] = 1.25;
    //array[2] = "abc";
    array[3] = 4;
    array[100000] = 5;
    
    //read
    printf("========================\n");
    printf("array[0]: %d\n", array[0]);
    printf("array[1]: %d\n", array[1]);
    printf("array[2]: %d\n", array[2]);
    printf("array[3]: %d\n", array[3]);
    printf("array[100000]: %d\n", array[100000]);

    return 0;
}
