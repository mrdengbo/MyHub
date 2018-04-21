#include <stdio.h>

void test(int b)
{
    int a;

    printf("sizeof(a)  : %d\n", sizeof(a));
    printf("sizeof(int): %d\n", sizeof(int));
    printf("sizeof a   : %d\n", sizeof a);
    //error
    //printf("sizeof int : %d\n", sizeof int);

    return;
}

int main()
{
    test(a);

    //error
    //test a;

    return 0;
}
