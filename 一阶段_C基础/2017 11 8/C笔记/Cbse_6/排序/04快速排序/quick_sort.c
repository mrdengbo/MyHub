#include <stdio.h>
#include <stdlib.h>

void quick_sort(int a[],int low,int high)
{
	int i,j,key;
//	printf("sizeof(a) = %d.\n",sizeof(a));
	
	i = low;
	j = high;
	key = a[low];

	while(i < j && a[j] > key) j --;
	if(i < j) a[i] = a[j];

	while(i < j && a[i] < key) i ++;
	if(i < j) a[j] = a[i];

	a[i] = key;

	if(j-1 > low)
		quick_sort(a,low,j-1);

	if(i + 1 < high)
		quick_sort(a,i+1,high);

	return;
}

int main()
{
	int a[] = {12,2,34,100,23,56,8,45};
	int low,high;
	int i;

	low = 0;
	high = sizeof(a)/sizeof(int) - 1;

	quick_sort(a,low,high);

	for(i = 0;i < sizeof(a)/sizeof(int);i ++)
	{
		printf("%d ",a[i]);
	}

	printf("\n");

	return 0;
}
