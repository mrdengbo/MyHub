#include <stdio.h>
inline void eatline()
	{
		while(getchar() != '#')
			continue;
		printf("bye!\n");
	}
int main(void)
{
	eatline();
	return 0;
}
