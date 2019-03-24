#include "head.h"

/**
 *@author:dengbo
 *@time:2018/4/20
 *@description:队列相关实现,入队,出队
 */

/*创建队列*/
LinkQueue * createLinkQueue()
{
	//为链表分配内存
	LinkNode * head = (LinkNode *)malloc(sizeof(LinkNode));
	if (NULL == head) {
		printf("createLinkQueue:malloc head failed\n");
		return NULL;
	}
	memset(head, 0, sizeof(LinkNode));  //初始化值

	//为队列分配内存
	LinkQueue * linkQueue = (LinkQueue *)malloc(sizeof(LinkQueue));
	if (NULL == linkQueue) {
		printf("createLinkQueue:malloc linkQueue failed\n");
		return NULL;
	}

	linkQueue->front = head;
	linkQueue->rear = head;

	return linkQueue;
}

/*判断队列是否为空*/
int isLinkQueueEmpty(LinkQueue * linkQueue)
{
	CHECKPARMNULL(linkQueue);
	return linkQueue->front == linkQueue->rear ? TRUE : FALSE;
}

/*数据入队列*/
int enQueue(LinkQueue * linkQueue, DaTaType data)
{
	CHECKPARMNULL(linkQueue);

	LinkNode * pTmp = (LinkNode *)malloc(sizeof(LinkNode));
	CHECKPARMNULL(pTmp);

	pTmp->data = data;
	pTmp->next = NULL;
	linkQueue->rear->next = pTmp;
	linkQueue->rear = pTmp;  //更新队尾节点的地址

	return OK;
}

/*数据出队*/
int popQueue(LinkQueue * linkQueue, DaTaType * outData)
{
	CHECKPARMNULL(linkQueue);

	LinkNode * pTmp = (LinkNode *)malloc(sizeof(LinkNode));
	CHECKPARMNULL(pTmp);

	DaTaType data = linkQueue->front->next->data;
	pTmp = linkQueue->front->next;
	linkQueue->front->next = pTmp->next;
	free(pTmp);
	pTmp = NULL;

	if (linkQueue->front->next == NULL)
	{
		linkQueue->rear = linkQueue->front;
	}

	*outData = data;

	return OK;
}