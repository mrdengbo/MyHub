#include <stdio.h>

int main()
{
    int n;
    int i;
    int count;
    int sum;

    while(1)
    {
        //scanf
        scanf("%d", &n);

        i = 0;
        count = 0;
        //第一个非0位
        while(i < 32)
        {
            //68
            //24个0 0100 0100：人的角度原码
            //24个0 0100 0100：补码存内存

            //1
            //24个0 0000 0001：人的角度原码
            //24个0 0000 0001：补码存内存
            //1 << 2
            //24个0 0000 0100：左移后的

            //24个0 0100 0100
            //24个0 0000 0100 &
            //---------------------
            //24个0 0000 0100 ---- 4
            //if(n & (1 << i) == (1 << i))
            //if(n & (1 << i))
            //if(!(n & (1 << i)))
            if(n & (1 << i))
            {
                //count ++;
                break;
            }

            count ++;
            i ++;
        }
        printf("count : %d\n", count);

        sum = 0;
        for(i = count + 1; i < 32; i ++)
        {
            if(!(n & (1 << i)))
            {
                sum ++;
            }
        }
        printf("sum: %d\n", sum);
    }
    return 0;
}
