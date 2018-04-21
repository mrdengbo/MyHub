#include <stdio.h>
#include <stdlib.h>

int sum_array(int (*p)[5] , int row,int column)
{
    int j; 
    int sum_row = 0;

    for(j = 0; j < column; j ++)
    {
        sum_row = sum_row + *((int *)(p + row) + j) ;

    }


    return sum_row ;


}

int main()
{
    int a[3][5] ={{1,2,3,4,5},{6,7,8,9,10}
        ,{11,12,13,14,15}};
    int i;
    int j;
    int ret ;

#if 0
    for(i = 0; i < 3; i ++)
    {
        for(j = 0; j < 5; j ++)
        {
            printf("a[%d][%d] : %d\n", i, j,a[i][j]);

        }


    }
#endif
    for(i = 0; i < 3; i ++)
    {
        ret = sum_array(a, i, 5);
        printf("%d :%d\n", i, ret  );

    }

    return 0;


}
