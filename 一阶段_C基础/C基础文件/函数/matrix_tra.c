#include <stdio.h>

int swap(int *m, int *n)
{

    *m ^= *n;
    *n ^= *m;
    *m ^= *n;

    return 0;
}



/*求一个3X3矩阵的转置
 *
 *
 *
 */
int matrix_tra(int (*a)[3], int num)
{
  int i;
  int j;


  for(i = 0; i < num; i ++)
  {

      for(j = i + 1; j < num; j ++)
      {
          printf("%d ,%d\n", a[i][j], a[j][i]);

          swap(&a[i][j],&a[j][i]);
      }
  
  }

}
void disp(int (*a)[3], int num)
{
    int i, j;
    for(i = 0; i < num; i ++)
    {
        for(j = 0; j < num; j ++)
        {
            printf("a[%d][%d] : %d\n", i, j, a[i][j]);
        
        }
    
    }

}


int main()
{
    int a[3][3] = {{0,1,2},{3,4,5},{6,7,8}};
    disp(a,3);
    matrix_tra(a,3);
    disp(a,3);

    return 0;
}
