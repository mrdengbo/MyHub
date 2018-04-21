#include <stdio.h>

int main()
{
    char ch;
    char result;

    scanf("%c", &ch);

    //'A' <= ch <= 'Z' => ch + 32
    //'z' <= ch <= 'z' => ch - 32
    
    printf("ch: %d\n", ch);
    result = ch >= 'A' && ch <= 'Z' ? ch + 32 : ch - 32;
    printf("result: %c, %d\n", result, result);
    
    ch >= 'A' && ch <= 'Z' ? printf("=%c\n", ch + 32) : ch - 32;

    5 - 3;

    return 0;
}
