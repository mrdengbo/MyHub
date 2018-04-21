#include <stdio.h>

int main()
{
    int a = 5;
    int b = 3;
    int c;
    
    c = a & b;// a &= b;// a = a & b;
    printf("& c: %d\n", c);

    c = a | b;
    printf("| c: %d\n", c);

    c = a ^ b;
    printf("^ c: %d\n", c);
    
    //5
    //28个0 0101 ：人的角度原码
    //28个1 1010 ：取反 c
    //1 27个1 1010 ：补码
    //1 27个0 0110 ：原码 -6
    c = ~ a;
    printf("~ c: %d\n", c);
    
    a = -5;
    //-5
    //1 27个0 0101：人的角度原码
    //1 27个1 1011：补码存内存
    //0 27个0 0100：整体取反 c 补码
    //0 27个0 0100：原码 -- 4
    c = ~ a;
    printf("~ c: %d\n", c);
    
    return 0;
}
