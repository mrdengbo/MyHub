#include <stdio.h>

int main()
{
    int array[5];
    int i;
    int max, min;
    int max_index, min_index;

    for(i = 0; i < 5; i ++)
    {
        scanf("%d", &array[i]);
    }
    
    for(i = 0; i < 5; i ++)
    {
        printf("%d ", array[i]);
    }
    putchar('\n');

    //max = array[0];
    //min = array[0]
    max = min = array[0];

    for(i = 0; i < 5; i ++)
    {
        if(max <= array[i])
        {
            max = array[i];
            max_index = i;
        }
        
        if(min >= array[i])
        {
            min = array[i];
            min_index = i;
        }
    }

    printf("max_index: %d, max: %d; min_index: %d, min: %d\n", max_index, max, min_index, min);

    return 0;
}
