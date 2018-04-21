#include <stdio.h>

int main()
{
    unsigned int x = 0x12345678;
    char *p = (char *)&x;
    int i;

    printf("&x: %p\n", &x);

    for(i = 0; i < 4; i ++)
    {
        //*p;
        //p = p + 1; 
        printf("%#x\n", *p ++);
        printf("p: %p\n", p);
    }

    return 0;
}
