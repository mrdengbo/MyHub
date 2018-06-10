#ifndef __SEQLIST_H__
#define __SEQLIST_H__

#ifdef __cplusplus 
extern "C" {
#endif

/*�ⲿ˳���ṹ�������Ͷ���*/
typedef void TagList;
/*˳���ڵ��������Ͷ���*/
typedef void SeqListNode;

typedef int (*Length)();
typedef int (*Capacity)();
typedef int (*Insert)(SeqListNode *, int );
typedef SeqListNode *(*Get)(int );
typedef SeqListNode *(*Remove)(int );
typedef void (*Clear)();
typedef void (*Destory)();

/*˳��������*/
typedef struct _SeqList {
  struct _SeqList *_this;   /*����ṹ��thisָ��*/
  TagList  *_taglist;       /*˳���ṹָ��*/
  Length   length;          /*˳���ĳ���*/
  Capacity capacity;        /*˳��������*/
  Insert   insert;          /*����ָ�����ݽڵ�*/
  Get      get;             /*��ȡָ�����ݽڵ�*/
  Remove   remove;          /*�Ƴ�ָ�����ݽڵ�*/
  Clear    clear;           /*���˳���*/
  Destory  destory;         /*����˳���*/
} SeqList;

/*���캯��*/
SeqList *SeqList_Construction(int count);
/*��������*/
void SeqList_Destruction(SeqList *list);

#ifdef __cplusplus
}
#endif

#endif /*__SEQLIST_H__*/
