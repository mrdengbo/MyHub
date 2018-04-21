#include <stdio.h>

#define PASSWD 110

int main()
{
    int passwd;
    int i = 0;

    do
    {
        printf("please input one passwd: ");
        //scanf
        scanf("%d", &passwd);
        //getchar
        while(getchar() != '\n');

        if(passwd == PASSWD)
        {
            printf("passwd is right! welcome.............\n");
            break;
        }
        else
        {
            if(i == 2)
            {  
                printf("BYE BYE\n");

                break;
            }
            printf("passwd is error! try again\n");
        }

        i ++;

    }while(i < 3);

    return 0;
}
