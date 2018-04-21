#include <stdio.h>

int main()
{
    int array[3];

    //place
    printf("&array[0]: %p\n", &array[0]);
    printf("&array[1]: %p\n", &array[1]);
    printf("&array[2]: %p\n", &array[2]);
    printf("&array[3]: %p\n", &array[3]);

    printf("&array[10]: %p\n", &array[10]);
    
    return 0;
}
