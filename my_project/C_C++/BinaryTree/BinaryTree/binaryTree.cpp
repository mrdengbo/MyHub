#include "head.h"

/**
 *@author:dengbo
 *@time:2018/4/20
 *@descrition:����������ز���,����,ǰ�����,�������,�������,��α���
*/

/*
 *@data:���������
 *@return:�������Ľڵ�
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
 *@descrition:����������,�����Ӵ���(2*num <= sum),�������ӽڵ�
 *���Һ��Ӵ���(2 * num + 1 <= sum),�����Һ��ӽڵ�
 *@pBuf:����Ĳ���
 *@num:�������ڵ����ڵ�λ��
 *@sum:�ڵ���ܸ���
 */
BiTree * createBiTree(char *pBuf, int num, int sum)
{
	if (NULL == pBuf)
	{
		printf("createBiTree:input param pBuf is NULL");
	}

	BiTree * root = createBitreeNode(pBuf[num]);

	//2 * num <= sum,���������Ӵ���
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
 *��������ǰ�����,������
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
*���������������,�����
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
*�������ĺ������,���Ҹ�
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
 *��������α���,ͨ������ʵ��,�Ƚ����ڵ����
 *Ȼ���ж϶����Ƿ�Ϊ��,��Ϊ�մӶ���ȡ�����ݲ����
 *�ж�ȡ���ĸýڵ������Ƿ����,���ڽ��������
 *�ж�ȡ���ĸýڵ��Һ����Ƿ����,���ڽ��Һ������
 */
void leverTraval(BiTree * root)
{
	if (NULL == root)
	{
		return;
	}

	DaTaType outData;

	LinkQueue *q = createLinkQueue();
	//�Ƚ����ڵ����
	enQueue(q,root);

	while (!isLinkQueueEmpty(q))
	{
		//��������
		popQueue(q, &outData);

		printf("%c ", outData->data);

		//�����Ӵ���,���
		if (NULL != outData->lchild)
		{
			enQueue(q, outData->lchild);
		}
		
		//���Һ��Ӵ���,���
		if (NULL != outData->rchild)
		{
			enQueue(q, outData->rchild);
		}
	}

	//ѭ������,�ͷ���ؽڵ�
	free(q->front);  //�˴��ͷ�ͷ�ڵ㼴��,��Ϊ����Ϊ��ʱ,β�ڵ��ͷ���ָ����ͬ���ڴ�
	free(q);
	q = NULL;
}