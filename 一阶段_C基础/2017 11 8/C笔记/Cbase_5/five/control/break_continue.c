#include <stdio.h>

int main()
{
    int i = 0;
    int j = 0;

    for(i = 0; i < 3; i ++)
    {
        printf("i: %d\n", i);

        while(j < 2)
        {
            printf("j; %d\n", j);

            sleep(1);
            
            //break;
            continue;
            
            j ++;
        }
    }

    if(i < 100)
    {
        //error
        //break;
        //continue;
    }

    return 0;
}
