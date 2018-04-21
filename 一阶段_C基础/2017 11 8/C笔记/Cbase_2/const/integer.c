#include <stdio.h>

int main()
{
    printf("hello World!\n");

    printf("integer: %d\n", 2000);
    printf("integer: %d, %d, %d\n", 2000, 100, -19);
    
    printf("Dec: %d\n", 56);
    //printf("Binary: %b\n", 56);

    printf("Oct: %o, %#o\n", 56, 56);
    //printf("Oct: %O\n", 56);
    
    printf("Hex: %x, %#x\n", 56, 56);

    printf("-56: Oct: %o, %#o\n", -56, -56);
    printf("-56: Hex: %x, %#x\n", -56, -56);

    return 0;
}
