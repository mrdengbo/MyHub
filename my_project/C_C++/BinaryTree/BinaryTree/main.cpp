#include "head.h"

/*主程序*/
int main(int argc, const char *argv[])
{
	//此处第一个变量(-1)不用,为了和二叉树的编号对应
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

	//前序遍历
	printf("preOrder:");
	preOrder(root);
	putchar('\n');

	//中序遍历
	printf("inOrder:");
	inOrder(root);
	putchar('\n');

	//后序遍历
	printf("postorder:");
	posOrder(root);
	putchar('\n');

	//层次遍历
	printf("leverTraver:");
	leverTraval(root);
	putchar('\n');

	return OK;
}