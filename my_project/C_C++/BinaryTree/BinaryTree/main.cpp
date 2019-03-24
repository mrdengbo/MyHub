#include "head.h"

/*������*/
int main(int argc, const char *argv[])
{
	//�˴���һ������(-1)����,Ϊ�˺Ͷ������ı�Ŷ�Ӧ
	DataType_c buf[] = { -1, 'A', 'B', 'C', 'D','E','F' };
	DaTaType root = NULL;
	int iDataCount = (sizeof(buf) / sizeof(buf[0])) - 1;

	root = createBiTree(buf, 1, iDataCount);

	if (NULL == root)
	{
		printf("create Bitree failed\n");
		return ERROR;
	}

	printf("create Bitree success\n");

	//ǰ�����
	printf("preOrder:");
	preOrder(root);
	putchar('\n');

	//�������
	printf("inOrder:");
	inOrder(root);
	putchar('\n');

	//�������
	printf("postorder:");
	posOrder(root);
	putchar('\n');

	//��α���
	printf("leverTraver:");
	leverTraval(root);
	putchar('\n');

	return OK;
}