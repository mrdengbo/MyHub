#include <stdio.h>

int main()
{
    char a = 20;
    int b = 1000;

    b = a;

    printf("b: %d\n", b);

    b = 1000;
    //a = b;
    a = (char)b;
 
    //1110 1000 : ����
    //24��1 1110 1000������
    //1 23��0 0001 1000��ԭ�� -- -24

    printf("a: %d\n", a);

    return 0;
}
