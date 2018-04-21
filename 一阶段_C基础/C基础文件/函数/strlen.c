#include <stdio.h>

int TotalStrlen(char *p)
{
    char *t = p ;
    int count = 0;
    while(*t != '\0')
    {
        count ++;
        t++ ;
    }
    return count ;
}

int main()
{
    char a[1024];
    int  ret = 0;
    printf("Please input a String:");
    scanf("%s", a);
    printf("%s\n", a);

    ret =  TotalStrlen(a);
    printf("%d\n", ret);
}
