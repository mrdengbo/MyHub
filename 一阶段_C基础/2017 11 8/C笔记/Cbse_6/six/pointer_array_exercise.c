#include <stdio.h>

int main()
{
    int a[5];
    int i;
    int *p;
    int *p_max, *p_min;

    p = a;

    for(i = 0; i < 5; i ++)
    {
                           //&p[i]
        //scanf("%d", p + i);//p[i] <=> *(p + i)
        scanf("%d", p ++);//p = p + 1;
    }
    
    p = a;
    for(i = 0; i < 5; i ++)
    {
        printf("%d ", *p ++);
    }
    putchar('\n');

    p_max = p_min = &a[0];

    p = a;
    for(i = 0; i < 5; i ++)
    {
        //if(*p_max <= a[i]) 
        //if(*p_max <= *(a + i))
        if(*p_max <= *p ++)
        {
            p_max = p - 1;
        }
        
        if(*p_min >= a[i])
        {
            p_min = &a[i];
        }
    }

    printf("p_max: %d, p_min: %d\n", *p_max, *p_min);

    return 0;
}
