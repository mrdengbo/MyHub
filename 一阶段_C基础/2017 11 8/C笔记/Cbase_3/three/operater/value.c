#include <stdio.h>

int main()
{
    int a = 3;
    int b = 4;
    int c = 0;
    float d;
    
    c = a + b;
    printf("c: %d\n", c); //c: 7

    c = a / b;
    printf("c: %d\n", c); //c: 0
    printf("c: %f\n", c); 

    d = a / b;
    printf("d: %f\n", d);

    c = a % b;
    printf("%% c: %d\n", c);
    
    //error
    //c = 3.0 % b;
    //c = a % 4.0;
    //c = 3.0 % 4.0;

    return 0;
}
