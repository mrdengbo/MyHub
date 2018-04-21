#include <stdio.h>

int main()
{
    int a = 3;
    int b = 4;
    int t;

    printf("a: %d, b: %d\n", a, b);
    t = a;
    a = b;
    b = t;
    printf("a: %d, b: %d\n", a, b);

    a = 3, b = 4;

    //x ^ x = 0;
    //x ^ 0 = x;

    printf("a: %d, b: %d\n", a, b);
    //  0011  3
    //^ 0100  4
    //----------------
    //  0111  7
    a ^= b;// a = a ^ b;
    
    //  0100  4
    //^ 0111  7
    //-------------
    //  0011  3
    b ^= a;// b = b ^ a;
    
    //  0111  7
    //^ 0011  3
    //-----------------
    //  0100  4
    a ^= b;// a = a ^ b;
    printf("a: %d, b: %d\n", a, b);

    return 0;
}
