#include <stdio.h>

int main()
{
    int ret;
    int result;

    //int getchar(void);
    ret = getchar();
    
    //int putchar(int c);
    result = putchar(ret);

    putchar('\n');

    putchar(result);

    printf("result: %d\n", result);

    return 0;
}
