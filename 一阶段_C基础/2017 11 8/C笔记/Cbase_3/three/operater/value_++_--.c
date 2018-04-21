#include <stdio.h>

int main()
{
    int a = 3;
    int b = 0;

    a ++;//a = a + 1; 
    printf("a: %d\n", a);

    ++ a;//a = a + 1;
    printf("a: %d\n", a);

    a = 3;
    b = a ++; //b = a; a = a + 1;
    printf("a: %d, b: %d\n", a, b); //4, 3
    
    a = 3;
    b = ++ a; //a = a + 1; b = a;
    printf("a: %d, b: %d\n", a, b);//4, 4

    int x = 5;
    int y = 6;
    int z = 0;

    z = x ++ + y ++;//z = x + y = 5 + 6 = 11; x = x + 1; y = y + 1;
    printf("x: %d, y: %d, z: %d\n", x, y, z);//6, 7, 11

    x = 5;
    y = 6;
    z = ++ x + y ++;//x = x + 1 = 5 + 1 = 6; z = x + y = 6 + 6 = 12; y = y + 1
    printf("x: %d, y: %d, z: %d\n", x, y, z);//6, 7, 12
    
    x = 5;
    y = 6;
    z = ++ x + ++ y;//x = x + 1 = 6; y = y + 1 = 7; z = x + y = 6 + 7 = 13
    printf("x: %d, y: %d, z: %d\n", x, y, z);//6, 7, 13

    return 0;
}
