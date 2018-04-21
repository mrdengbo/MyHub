#include <stdio.h>

int main()
{
    char a[] = "string";//{'s', 't', 'r', 'i', 'n', 'g', '\0'}
    char b[] = {"string"};
    char *s = "string";

    printf("a: %p, &a: %p\n", a, &a);
    printf("b: %p, &b: %p\n", b, &b);
    printf("s: %p, &s: %p\n", s, &s);

    printf("a[3]: %c\n", a[3]);
    printf("s[3]: %c\n", s[3]);
    printf("*(s + 3): %c\n", *(s + 3));
    printf("s + 3: %p\n", s + 3);

    //error
    //s[3] = 'm';

    a[3] = 'm';
    printf("a: %s\n", a);

    return 0;
}
