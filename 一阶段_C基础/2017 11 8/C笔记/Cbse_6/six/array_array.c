#include <stdio.h>

int main()
{
    int a[2][2];
    int i, j;

    int a_1[2][2] = {1, 2, 3, 4};
    int a_2[][3] = {{1, 2, 3},
                     {4, 5},
                    };

    printf("a[0][0]: %p\n", &a[0][0]);
    printf("a[0][1]: %p\n", &a[0][1]);
    printf("a[1][0]: %p\n", &a[1][0]);
    printf("a[1][1]: %p\n", &a[1][1]);

    for(i = 0; i < sizeof(a_2) / sizeof(a_2[0]); i ++)
    {
        for(j = 0; j < sizeof(a_2[0]) / sizeof(a_2[0][0]); j ++)
        {
            printf("a_2[%d][%d]: %d\n", i, j, a_2[i][j]);
        }
    }

    return 0;
}
