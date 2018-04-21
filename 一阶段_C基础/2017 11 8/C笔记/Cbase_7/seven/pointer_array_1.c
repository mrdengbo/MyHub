#include <stdio.h>

int main()
{
    int a[] = {0x00100001, 2, 3, 4};
    int b[] = {1, 2, 3, 4};

    printf("*((char *)&a[1] - 2): %#x\n", *((char *)&a[1] - 2));
    printf("                    : %#x\n", ((char *)&a[1])[-2]);
    printf("                    : %#x\n", *((char *)((int *)(&a + 1) - 3)  - 2));
    printf("                    : %#x\n", ((char *)((int *)(&a + 1) - 3))[-2]);

    
    int *ptr1 = (int *)(&b + 1);
    int *ptr2 = (int *)((int)b + 1);

    printf("%#x, %#x\n", ptr1[-1], *ptr2);

    return 0;
}
