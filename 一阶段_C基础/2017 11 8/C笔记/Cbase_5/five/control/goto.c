#include <stdio.h>

int main()
{
    int i;

label_1:
    printf("label_1: i: %d\n", i);

    for(i = 0; i < 3; i ++)
    {
        printf("for i: %d\n", i);

        if(i == 1)
        {
            //goto label_1;
        }
         
        sleep(1);
        putchar('\n');
    }

label:
    printf("label: i: %d\n", i);

    return 0;
}
