#include <stdio.h>

int main()
{
    char *s;
    char string[5];

    //gets
    //gets(s);
    //string: char *
    gets(string);

    s = string;

    //puts
    //puts(s);
    puts(string);
    puts(s);

    return 0;
}
