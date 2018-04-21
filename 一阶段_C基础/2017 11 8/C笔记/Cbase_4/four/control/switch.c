#include <stdio.h>

#define M 2

int main()
{
    int a;

    //switch(a = 1)
    //switch(a == 1)//0
    switch(a = 1)
    {
        case 1:
            printf("1 integer\n");
            //break;

        case 3:
            printf("3 integer\n");
            break;
        
        //error
        //case 1:
            //printf("integer\n");
            //break;

        //error
        //case 1.2:
            //printf("float\n");
            //break;

        case 'a': //97
            printf("char\n");
            break;

        //error
        //case "abc":
            //printf("string\n");
            //break;

        case M: //2
            printf("define\n");
            break;

        //error
        //case (a > 1):
            //printf("a > 1\n");
            //break;
        //case a == 2:
            //printf("a == 2\n");
            //break;

        default:
            printf("nothing\n");
            break;
    }

    return 0;
}
