#include <stdio.h>

#define M 15
#define N M + M
#define F (M + M)
#define MYPRINTF printf("N: %d\n", N);

#define MYPRINTF_1(arg) printf("arg: %d\n", arg);
#define MYPRINTF_2(arg...) printf("arg: %d, %d\n", arg, arg);

int main()
{
    printf("M: %d\n", M); //<=> printf("M: %d\n", 15);

    printf("N * N: %d\n", N * N);//N * N = M + M * M + M = 15 + 15 * 15 + 15 = 30 + 225 

    printf("F * F: %d\n", F * F);//F * F = (M + M) * (M + M) = 30 * 30 = 900

    MYPRINTF;//printf("N: %d\n", 15 + 15);;

    MYPRINTF_1(20);

    MYPRINTF_2(100, 200);

    return 0;
}
