#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERRRETVAL -1

int main()
{
	char *p = (char *)malloc(100 * sizeof(char));
	if (NULL == p)
	{
		printf("malloc p is failed\n");
		return ERRRETVAL;
	}

	fgets(p,10,stdin);

	printf("hello git\n");
	printf("%s\n",p);
	jfdjsajfjsajfjj;
	jfdjsjafjl;
	dengbo;
	
	return 0;
}


