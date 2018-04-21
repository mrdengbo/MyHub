#include <stdio.h>

int main()
{
    int score;

    printf("Please input one score: ");
    scanf("%d", &score);

    if(score < 0 || score > 100)
    {
        printf("score is error, will exit...........\n");
        return 0;
    }

    if(score >= 0 && score < 60)
    {
        printf("D");
    }
    else
    {
        if(score >= 60 && score < 70)
        {
            putchar('C');
        }
        else
        {
            if(score >= 70 && score < 90)
            {
                putchar('B');
            }
            else
            {
                if(score >= 90 && score <= 100)
                {
                    putchar('A');
                }
            }
        }
    }
    putchar('\n');

    return 0;
}
