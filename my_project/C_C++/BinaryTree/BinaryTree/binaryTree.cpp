#include "head.h"

/**
 *@author:dengbo
 *@time:2018/4/20
 *@descrition:二叉树的相关操作,创建,前序遍历,中序遍历,后序遍历,层次遍历
*/

/*
 *@data:传入的数据
 *@return:二叉树的节点
*/
BiTree * createBitreeNode(DataType_c data)
{
	if (NULL == data) {
		printf("initBiTree:input pBuf is NULL\n");
		return NULL;
	}

	BiTree * bTree = (BiTree *)malloc(sizeof(BiTree));
	if (NULL == bTree) {
		printf("initBiTree:malloc bTree is NULL\n");
		return NULL;
	}

	bTree->data = data;
	bTree->lchild = NULL;
	bTree->rchild = NULL;

	return bTree;
}

/**
 *@descrition:构建二叉树,当左孩子存在(2*num <= sum),创建左孩子节点
 *当右孩子存在(2 * num + 1 <= sum),创建右孩子节点
 *@pBuf:传入的参数
 *@num:二叉树节点所在的位置
 *@sum:节点的总个数
 */
BiTree * createBiTree(char *pBuf, int num, int sum)
{
	if (NULL == pBuf)
	{
		printf("createBiTree:input param pBuf is NULL");
	}

	BiTree * root = createBitreeNode(pBuf[num]);

	//2 * num <= sum,二叉树左孩子存在
	if (2 * num <= sum)
	{
		root->lchild = createBiTree(pBuf, 2 * num, sum);
	}

	if (2 * num + 1 <= sum)
	{
		root->rchild = createBiTree(pBuf, 2 * num + 1, sum);
	}

	return root;
}

/**
 *二叉树的前序遍历,根左右
 */
void preOrder(BiTree * root)
{
	if (NULL == root)
	{
		return;
	}
	printf("%c ", root->data);
	preOrder(root->lchild);
	preOrder(root->rchild);
}

/**
*二叉树的中序遍历,左根右
*/
void inOrder(BiTree * root)
{
	if (NULL == root)
	{
		return;
	}

	inOrder(root->lchild);
	printf("%c ", root->data);
	inOrder(root->rchild);
}

/**
*二叉树的后序遍历,左右根
*/
void posOrder(BiTree * root)
{
	if (NULL == root)
	{
		return;
	}

	posOrder(root->lchild);
	posOrder(root->rchild);
	printf("%c ", root->data);
}

/**
 *二叉树层次遍历,通过队列实现,先将根节点入队
 *然后判断队列是否为空,不为空从队列取出数据并输出
 *判断取出的该节点左孩子是否存在,存在将左孩子入队
 *判断取出的该节点右孩子是否存在,存在将右孩子入队
 */
void leverTraval(BiTree * root)
{
	if (NULL == root)
	{
		return;
	}

	DaTaType outData;

	LinkQueue *q = createLinkQueue();
	//先将根节点入队
	enQueue(q,root);

	while (!isLinkQueueEmpty(q))
	{
		//出队数据
		popQueue(q, &outData);

		printf("%c ", outData->data);

		//当左孩子存在,入队
		if (NULL != outData->lchild)
		{
			enQueue(q, outData->lchild);
		}
		
		//当右孩子存在,入队
		if (NULL != outData->rchild)
		{
			enQueue(q, outData->rchild);
		}
	}

	//循环结束,释放相关节点
	free(q->front);  //此处释放头节点即可,因为队列为空时,尾节点和头结点指向相同的内存
	free(q);
	q = NULL;
}