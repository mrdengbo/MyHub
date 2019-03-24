#ifndef __BINARY_TREE__
#define __BINARY_TREE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//相关宏定义
#define ERROR -1
#define TRUE 1
#define FALSE 0
#define OK 0

//二叉树相关结构设计
typedef char DataType_c;

typedef struct bnode {
	DataType_c data; //二叉树存储的数据
	struct bnode *lchild;  //二叉树的左孩子
	struct bnode *rchild;  //二叉树的右孩子
}BiTree;

//队列相关结构设计
typedef BiTree * DaTaType; //队列存储的数据类型

typedef struct Node{
	DaTaType data;
	struct Node * next;   //指向链表的下一个元素
}LinkNode;

//队列头设计
typedef struct {
	LinkNode * front;  //保存队头元素的地址
	LinkNode * rear;   //保存队尾元素的地址
}LinkQueue;

//指针空值检测
#define CHECKPARMNULL(PARM)\
do{\
	if (NULL == PARM){\
		printf("%s:%d:%s", __FUNCTION__, __LINE__, "check parameter is NULL");\
		return ERROR;\
	}\
}\
while(0);

//队列相关函数的声明
extern LinkQueue * createLinkQueue();
extern int isLinkQueueEmpty(LinkQueue * linkQueue);
extern int enQueue(LinkQueue * linkQueue, DaTaType data);
extern int popQueue(LinkQueue * linkQueue, DaTaType * outData);

//二叉树相关函数声明
extern BiTree * createBitreeNode(DataType_c data);
extern BiTree * createBiTree(char *pBuf, int num, int sum);
extern void preOrder(BiTree * root);
extern void inOrder(BiTree * root);
extern void posOrder(BiTree * root);
extern void leverTraval(BiTree * root);

#endif
