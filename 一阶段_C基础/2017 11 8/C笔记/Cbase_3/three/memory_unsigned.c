#include <stdio.h>

int main()
{
    //[0, 255]
    //unsigned char a = 0;

    //1000 0001 : 人的角度原码
    //1111 1110 : 反码
    //1111 1111 : 补码
    //1111 1111 : 补码存内存（无符号了）

    //不能当成有符号来处理了(错误的推到)
    //24个1 1111 1111 : 补码
    //1 23个1 1111 1111 : 补码
    //1 23个0 0000 0000 : 反码
    //1 23个0 0000 0001 : 原码 -- -1
    unsigned char a = -1;

    //unsigned char b = 255;
    
    //1 0000 0000 : 人的角度原码
    //0 1 0000 0000 : 256的原码
    //0000 0000 : 补码存内存
    //0000 0000 : 原码
    unsigned char b = 256;

    printf("a: %d, b: %d\n", a, b);//%d: signed int
    
    int c = -1;
    //1 23个0 0000 0001 : 人的角度原码
    //1 23个1 1111 1110 : 反码
    //1 23个1 1111 1111 : 补码
    printf("c: %d\n", c);
    printf("c: %#x\n", c);

    return 0;
}
