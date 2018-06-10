#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void BubbleSort(int *buf, int BufSize)
{
	int i,j;
	int temp = 0; //中间变量
	
	assert((NULL != buf) && (BufSize > 0));
	
	for (i = 0; i < BufSize -1; i++)
	{
		for (j = 0; j < BufSize - i -1; j++)
		{
			if (buf [j] > buf[j + 1])
			{
				temp = buf[j];
				buf[j] = buf[j + 1];
				buf[j + 1] = temp;
			}
		}
	}
	
	return;
}

int main()
{
	int i;
	
	int buf[]={79,20,40,80,50};
	
	int BufSize = sizeof(buf) / sizeof(int);
	
	printf("before sort:\n");
	for (i = 0; i < BufSize; i++)
	{
		printf("%d ",buf[i]);
	}
	
	putchar('\n');
	
	BubbleSort(buf, BufSize);
	
	printf("After sort\n");
	for (i = 0; i < BufSize; i++)
	{
		printf("%d ",buf[i]);
	}
	putchar('\n');
	
	return 0;
}
