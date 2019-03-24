#ifndef __BINARY_TREE__
#define __BINARY_TREE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//��غ궨��
#define ERROR -1
#define TRUE 1
#define FALSE 0
#define OK 0

//��������ؽṹ���
typedef char DataType_c;

typedef struct bnode {
	DataType_c data; //�������洢������
	struct bnode *lchild;  //������������
	struct bnode *rchild;  //���������Һ���
}BiTree;

//������ؽṹ���
typedef BiTree * DaTaType; //���д洢����������

typedef struct Node{
	DaTaType data;
	struct Node * next;   //ָ���������һ��Ԫ��
}LinkNode;

//����ͷ���
typedef struct {
	LinkNode * front;  //�����ͷԪ�صĵ�ַ
	LinkNode * rear;   //�����βԪ�صĵ�ַ
}LinkQueue;

//ָ���ֵ���
#define CHECKPARMNULL(PARM)\
do{\
	if (NULL == PARM){\
		printf("%s:%d:%s", __FUNCTION__, __LINE__, "check parameter is NULL");\
		return ERROR;\
	}\
}\
while(0);

//������غ���������
extern LinkQueue * createLinkQueue();
extern int isLinkQueueEmpty(LinkQueue * linkQueue);
extern int enQueue(LinkQueue * linkQueue, DaTaType data);
extern int popQueue(LinkQueue * linkQueue, DaTaType * outData);

//��������غ�������
extern BiTree * createBitreeNode(DataType_c data);
extern BiTree * createBiTree(char *pBuf, int num, int sum);
extern void preOrder(BiTree * root);
extern void inOrder(BiTree * root);
extern void posOrder(BiTree * root);
extern void leverTraval(BiTree * root);

#endif
