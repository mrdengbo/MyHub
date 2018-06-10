//#include <stdio.h>
#include <stdlib.h>
#include "seqlist.h"

/*ѡ��unsigned int�������͵�ԭ���Ǹ��������͵��ֽڴ�С��ָ����ֽڴ�С��ͬ,������ָ������,���ڴ洢��ַ����*/
typedef unsigned int pArray;

/*�ڲ�˳���ṹ��������*/
typedef struct _TagSeqList {
    int _length;        /*��Ч���ݳ���*/
    int _capacity;      /*������С*/
    pArray *_node;      /*�ڵ�˳���̬����*/
}TagSeqList;

TagSeqList *g_taglist = NULL;
SeqList *g_list = NULL;

/********************************************
 *�������ƣ�SeqList_Length
 *������������ȡ˳���ǰ����
 *������Σ�void
 *�������Σ���
 *��������ֵ��int
 *���ߣ�xiaoma
 *����ʱ�䣺2017��11��29��
 ********************************************/
int SeqList_Length()
{
	g_taglist = g_list->_this->_taglist;
  if (NULL == g_taglist) {
    return -1;
  }
  return g_taglist->_length;
}

/********************************************
 *�������ƣ�SeqList_Capacity
 *������������ȡ˳�������
 *������Σ�void
 *�������Σ���
 *��������ֵ��int
 *���ߣ�xiaoma
 *����ʱ�䣺2017��11��29��
 ********************************************/
int SeqList_Capacity()
{
	g_taglist = g_list->_this->_taglist;
  if (NULL == g_taglist) {
    return -1;
  }
  return g_taglist->_capacity;
}

/********************************************
 *�������ƣ�SeqList_Insert
 *��������������ָ��λ�����ݽڵ�
 *������Σ�node   //�ڵ�ָ��
            pos    //����λ��
 *�������Σ���
 *��������ֵ��int  //�ɹ�����0,ʧ�ܷ��ط�0
 *���ߣ�xiaoma
 *����ʱ�䣺2017��11��29��
 ********************************************/
int SeqList_Insert(SeqListNode *node, int pos)
{
  int i;
  g_taglist = g_list->_this->_taglist;
  if (NULL == g_taglist || NULL == node ) {
    return -1;
  }
  if (pos < 0 || pos >= g_taglist->_capacity) {
    return -2;
  }
  /*�ж����������Ƿ�����*/
  if (g_taglist->_length >= g_taglist->_capacity) {
    return -3;
  }
  /*�������λ�ô��ڵ�ǰԪ�ظ�������Ĭ�ϲ����ڵ�ǰĩβԪ�غ���*/
  if (pos > g_taglist->_length) {
    pos = g_taglist->_length;
  }
  /*�Ӻ���ǰ��������ǰ�ڵ�Ԫ�ظ�ֵ����ڵ�Ԫ��*/
  for (i = g_taglist->_length; i > pos; i--) {
    g_taglist->_node[i] = g_taglist->_node[i - 1];
  }
  /*node��ΪԪ�ص�ֵַ�洢��������*/
  g_taglist->_node[pos] = (pArray)node;
  g_taglist->_length++;
  return 0;
}

/********************************************
 *�������ƣ�SeqList_Get
 *������������ȡָ��λ�����ݽڵ�
 *������Σ�pos    //����λ��
 *�������Σ���
 *��������ֵ��SeqListNode  //�ڵ�ָ��
 *���ߣ�xiaoma
 *����ʱ�䣺2017��11��29��
 ********************************************/
SeqListNode *SeqList_Get(int pos)
{
	g_taglist = g_list->_this->_taglist;
	/*Ĭ�ϴ�0��ʼ��¼λ��*/
  if (NULL == g_taglist || pos < 0 || pos >= g_taglist->_length) {
    return NULL;
  }
  return (SeqListNode *)g_taglist->_node[pos];
}

/********************************************
 *�������ƣ�SeqList_Remove
 *�����������Ƴ�ָ��λ�����ݽڵ�
 *������Σ�pos    //����λ��
 *�������Σ���
 *��������ֵ��SeqListNode  //�ڵ�ָ��
 *���ߣ�xiaoma
 *����ʱ�䣺2017��11��29��
 ********************************************/
SeqListNode *SeqList_Remove(int pos)
{
	g_taglist = g_list->_this->_taglist;
  if (NULL == g_taglist || pos < 0 || pos >= g_taglist->_length) {
    return NULL;
  }
  int i;
  SeqListNode *ret = (SeqListNode *)g_taglist->_node[pos];
  /*�Ƴ��ڵ�ĺ������нڵ���ǰ�ƶ�һλ*/
  for (i = pos + 1; i < g_taglist->_length; i++) {
    g_taglist->_node[i - 1] = g_taglist->_node[i];
  }
  g_taglist->_length--;
  return  ret;
}

/********************************************
 *�������ƣ�SeqList_Clear
 *�������������˳���
 *������Σ�void
 *�������Σ���
 *��������ֵ��void
 *���ߣ�xiaoma
 *����ʱ�䣺2017��11��29��
 ********************************************/
void SeqList_Clear()
{
  g_taglist = g_list->_this->_taglist;
  if (NULL == g_taglist) {
    return ;
  }
  g_taglist->_length = 0;
  return ;
}

/********************************************
 *�������ƣ�SeqList_Destory
 *��������������˳���
 *������Σ�void
 *�������Σ���
 *��������ֵ��void
 *���ߣ�xiaoma
 *����ʱ�䣺2017��11��29��
 ********************************************/
void SeqList_Destory()
{
  if (NULL == g_list->_this->_taglist) {
    return ;
  }
  free(g_list->_this->_taglist);
  g_list->_this->_taglist = NULL;
  return ;
}

/********************************************
 *�������ƣ�SeqList_Construction
 *����������˳������Ĺ��캯��
 *������Σ�count   //����˳����������С
 *�������Σ���
 *��������ֵ��SeqList  //˳������ָ��
 *���ߣ�xiaoma
 *����ʱ�䣺2017��11��29��
 ********************************************/
SeqList *SeqList_Construction(int count)
{
  g_list = (SeqList *)malloc(sizeof(SeqList));
  if (NULL == g_list) {
    return NULL;
  }
  /*�ṹ����ṹ���Ա������ͬ����һ���������ڴ棬��ʡ���輰������Դ�ͷŲ���*/
  g_taglist = (TagSeqList *)malloc(sizeof(TagSeqList) + sizeof(pArray) *count);
  if (NULL == g_taglist) {
  	if (NULL != g_list) {
  		free(g_list);
  		g_list = NULL;
  	}
    return NULL;
  }
  g_taglist->_length = 0;
  g_taglist->_capacity = count;
  /*ret�ṹ������ָ��ƫ��һ���ṹ���С��ָ��ָ��̬���飨��Ա�������ڴ��׵�ַ*/
  g_taglist->_node = (pArray *)(g_taglist + 1);
  g_list->_taglist = (TagList *)g_taglist;
  g_list->length   = SeqList_Length;
  g_list->capacity = SeqList_Capacity;
  g_list->insert   = SeqList_Insert;
  g_list->get      = SeqList_Get;
  g_list->remove   = SeqList_Remove;
  g_list->clear    = SeqList_Clear;
  g_list->destory  = SeqList_Destory;
  g_list->_this    = g_list;
  return g_list;
}

/********************************************
 *�������ƣ�SeqList_Destruction
 *����������˳���������������
 *������Σ�void
 *�������Σ���
 *��������ֵ��void
 *���ߣ�xiaoma
 *����ʱ�䣺2017��11��29��
 ********************************************/
void SeqList_Destruction(SeqList *list)
{
	if (list->_taglist != NULL) {
		free(list->_taglist);
		list->_taglist = NULL;
	}
	if (list != NULL) {
		free(list);	
  }
  return ;
}
