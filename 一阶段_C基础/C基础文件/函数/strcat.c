#include <stdio.h>

char * MyStrcat(char * str1, char * str2)
{
    char *p = str1;
   

    while(*str1++);
    str1 -- ;
    while (*str1++ = *str2++);
          
   return p;


}
int main()
{
    char a[1024];
    char b[1024];
    char *c = a;

    printf("Please input one String:");
    scanf("%s", a);
    printf("a:%s\n", a);
    printf("Please input one String:");
    scanf("%s", b);
    printf("b:%s\n", b); 
    printf("%s\n", MyStrcat(a, b));

    return 0;

}
