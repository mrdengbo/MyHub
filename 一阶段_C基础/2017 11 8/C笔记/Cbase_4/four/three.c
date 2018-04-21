#include <stdio.h>

int main()
{
    int a = 3;
    int b;
    
    b = a > 1 ? a + 2 : a - 2;
    printf("b: %d, a: %d\n", b, a);
 
    a = 3;
    b = a > 1 && a <= 5 ? a ++ : -- a ;//b = a ++; //b = a; a = a + 1;
    printf("b: %d, a: %d\n", b, a); //3, 4
    
    a = 3;
    b = a > 1 && a <= 2 ? a ++ : -- a ;// b = -- a; 
    printf("b: %d, a: %d\n", b, a); //2, 2

    a = 3;
    int c = 1;
    b = c -- ? a ++ : -- a ;//b = a ++;
    printf("b: %d, a: %d, c: %d\n", b, a, c); //3, 4, 0
    
    a = 3;
    int c = 1;
    b = -- c ? a ++ : -- a ;//b = -- a;
    printf("b: %d, a: %d, c: %d\n", b, a, c); //2, 2, 0

    return 0;
}
