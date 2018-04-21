#include <stdio.h>

int main()
{
    printf("char         : %d\n", sizeof(char));
    //error
    //printf("char int     : %d\n", sizeof(char int));
    printf("short        : %d\n", sizeof(short));
    printf("short int    : %d\n", sizeof(short int));
    printf("int          : %d\n", sizeof(int));
    printf("long         : %d\n", sizeof(long));
    printf("long int     : %d\n", sizeof(long int));
    printf("long long    : %d\n", sizeof(long long));
    printf("long long int: %d\n", sizeof(long long int));
    printf("float        : %d\n", sizeof(float));
    printf("double       : %d\n", sizeof(double));

    return 0;
}
