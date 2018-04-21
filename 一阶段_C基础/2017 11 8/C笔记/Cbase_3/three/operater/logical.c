#include <stdio.h>

int main()
{
    int a = 3;
    int b = 4;
    int c;

    c = a < 10 && a > 1;//  1 < a < 10
    printf("-c: %d\n", c);
    
    c = (a < 2 && a > 1);//  1 < a < 2
    printf("+c: %d\n", c);
    
    c = (a < b && a > 1);//  1 < a < 4
    printf("*c: %d\n", c);
    
    c = (a < 2 || a > 1);//  1 < a < 2
    printf("$c: %d\n", c);//1

    int d = 1;
    int e = 1;

    c = -- d && e ++;//-- d: d = d - 1 = 0
    printf("c: %d, d: %d, e: %d\n", c, d, e);//0, 0, 1

    d = 0;
    e = 1;
    c = d ++ || e ++;// 0 || 1 : 1; d = d + 1; e = e + 1
    printf("c: %d, d: %d, e: %d\n", c, d, e);//1, 1, 2
    
    d = 0;
    e = 1;
    c = ++ d || e ++;//d = d + 1 = 1; 1 || 1 : 1; 
    printf("c: %d, d: %d, e: %d\n", c, d, e);//1, 1, 1
    
    d = 0;
    e = 1;
    c = !(++ d || e ++);//d = d + 1 = 1; 1 || 1 : 1; 
    printf("c: %d, d: %d, e: %d\n", c, d, e);//0, 1, 1

    int x = 0;
    c = !x;
    printf("c: %d\n", c);

    //不建议这样写
    c = 0;
    c = 1 < a < 10;
    printf("=c: %d\n", c);

    return 0;
}
