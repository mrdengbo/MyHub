#include <stdio.h>

int main()
{
    int score;
    int num;

    printf("Please input one score: ");
    scanf("%d", &score);

    if(score < 0 || score > 100)
    {
        printf("score is error, will exit..............\n");
        return 0;
    }

    num = score / 10;

    switch(num)
    //switch(15)
    //switch(score > 0)//1
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            printf("D");
            break;

        case 6:
            printf("C");
            break;

        //7 | 8
        //0111
        //1000 |
        //----------
        //1111 -- 15
        //case 7 | 8:
        case 7:
        case 8:
            putchar('B');
            break;

        case 9:
        case 10:
            putchar('A');
            break;

        default:
            break;
    }
    putchar('\n');

    return 0;
}
