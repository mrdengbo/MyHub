#include <stdio.h>

int main()
{
    int a;
    int b;
    int c;
    char d;
    int ret;

    //scanf
    //int scanf(const char *format, ...);
    //int printf(const char *format, ...);
  
    printf("a: %d\n", a);
    printf("&a: %p\n", &a);
    
    //不建议这么写
    //scanf("a: %d\n", &a);//&：取地址符号(shift + 7)
    //printf("=a: %d\n", a);

    //建议这样写
    //scanf("%d", &a);
    //scanf("%d", &b);
    //scanf("%d", &c);
    //scanf("%d%d%d", &a, &b, &c);

    ret = scanf("%d%d%c", &a, &b, &d);
    printf("a: %d\n", a);
    printf("b: %d\n", b);
    printf("d: %d\n", d);
    
    //error
    //printf("a: %#x\n", a);
    //scanf("a: %d\n", a);
    //printf("-a: %d\n", a);

    //return value
    printf("ret: %d\n", ret);

    return 0;
}
