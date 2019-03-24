#include "head.h"

/**
 *@author:dengbo
 *@time:2018/4/20
 *@description:�������ʵ��,���,����
 */

/*��������*/
LinkQueue * createLinkQueue()
{
	//Ϊ��������ڴ�
	LinkNode * head = (LinkNode *)malloc(sizeof(LinkNode));
	if (NULL == head) {
		printf("createLinkQueue:malloc head failed\n");
		return NULL;
	}
	memset(head, 0, sizeof(LinkNode));  //��ʼ��ֵ

	//Ϊ���з����ڴ�
	LinkQueue * linkQueue = (LinkQueue *)malloc(sizeof(LinkQueue));
	if (NULL == linkQueue) {
		printf("createLinkQueue:malloc linkQueue failed\n");
		return NULL;
	}

	linkQueue->front = head;
	linkQueue->rear = head;

	return linkQueue;
}

/*�ж϶����Ƿ�Ϊ��*/
int isLinkQueueEmpty(LinkQueue * linkQueue)
{
	CHECKPARMNULL(linkQueue);
	return linkQueue->front == linkQueue->rear ? TRUE : FALSE;
}

/*���������*/
int enQueue(LinkQueue * linkQueue, DaTaType data)
{
	CHECKPARMNULL(linkQueue);

	LinkNode * pTmp = (LinkNode *)malloc(sizeof(LinkNode));
	CHECKPARMNULL(pTmp);

	pTmp->data = data;
	pTmp->next = NULL;
	linkQueue->rear->next = pTmp;
	linkQueue->rear = pTmp;  //���¶�β�ڵ�ĵ�ַ

	return OK;
}

/*���ݳ���*/
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