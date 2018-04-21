#include <stdio.h>


char * MyStrcpy(char *dir , char* src)
{
    char *t = dir ;
    char *p = src ; 
    int count = 0 ;
    while(*p != '\0')
    {
    
        *dir = *p ;
        dir ++;
        p   ++;

    }
   
    dir = dir + 1;
    *dir = '\0';
     
    return t;
}

int main()
{
    char a[1024];
    char b[1024];
    char *s;

    printf("Please input a String : ");
    scanf("%s", a);
    printf("%s\n", a);
    
    s = MyStrcpy(b, a);
    printf("%s\n", s);

}
