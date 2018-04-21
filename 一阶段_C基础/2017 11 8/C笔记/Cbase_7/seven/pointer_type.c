#include <stdio.h>

int main()
{
    int n = 10;
    char *p;
    short *q;
    int *x;
    double *y;

    //printf("sizeof(p): %d, sizeof(char *) : %d\n", sizeof(p), sizeof(char *));
    //printf("sizeof(q): %d, sizeof(short *): %d\n", sizeof(q), sizeof(short *));
    //printf("sizeof(x): %d, sizeof(int *)  : %d\n", sizeof(x), sizeof(int *));

    printf("n: %d, &n: %p\n", n, &n);
    printf("p: %p, &p: %p\n", p, &p);
    printf("q: %p, &q: %p\n", q, &q);
    printf("x: %p, &x: %p\n", x, &x);

    p = (char *)&n;
    q = (short *)&n;
    x = &n;
    y = (double *)&n;

    printf("======================\n");
    printf("n: %d, &n: %p\n", n, &n);
    printf("p: %p, &p: %p\n", p, &p);
    printf("q: %p, &q: %p\n", q, &q);
    printf("x: %p, &x: %p\n", x, &x);
    printf("y: %p, &y: %p\n", y, &y);

    p ++;//p = p + 1;
    q ++;
    x ++;
    y ++;
    
    printf("======================\n");
    printf("n: %d, &n: %p\n", n, &n);
    printf("p: %p, &p: %p\n", p, &p);
    printf("q: %p, &q: %p\n", q, &q);
    printf("x: %p, &x: %p\n", x, &x);
    printf("y: %p, &y: %p\n", y, &y);

    return 0;
}
