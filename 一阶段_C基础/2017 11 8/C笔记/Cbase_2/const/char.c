#include <stdio.h>

int main()
{
    printf("char: %c\n", 'A');
    printf("char: %d\n", 'A');
    printf("char: %#o\n", 'A');
    printf("char: %#x\n", 'A');

    printf("char: %c\n", 'a' - 'A');//97 - 65 = 32 space
    
    printf("char: %c\n", 'A' + 32);//97 - 65 = 32 space
    printf("char: %c\n", 'A' + ' ');//97 - 65 = 32 space
    
    return 0;
}
