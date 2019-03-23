#ifndef __SEQLIST_H__
#define __SEQLIST_H__

#ifdef __cplusplus 
extern "C" {
#endif

/*外部顺序表结构数据类型定义*/
typedef void TagList;
/*顺序表节点数据类型定义*/
typedef void SeqListNode;

typedef int (*Length)();
typedef int (*Capacity)();
typedef int (*Insert)(SeqListNode *, int );
typedef SeqListNode *(*Get)(int );
typedef SeqListNode *(*Remove)(int );
typedef void (*Clear)();
typedef void (*Destory)();

/*顺序表对象定义*/
typedef struct _SeqList {
  struct _SeqList *_this;   /*自身结构体this指针*/
  TagList  *_taglist;       /*顺序表结构指针*/
  Length   length;          /*顺序表的长度*/
  Capacity capacity;        /*顺序表的容量*/
  Insert   insert;          /*插入指定数据节点*/
  Get      get;             /*获取指定数据节点*/
  Remove   remove;          /*移除指定数据节点*/
  Clear    clear;           /*清空顺序表*/
  Destory  destory;         /*销毁顺序表*/
} SeqList;

/*构造函数*/
SeqList *SeqList_Construction(int count);
/*析构函数*/
void SeqList_Destruction(SeqList *list);

#ifdef __cplusplus
}
#endif

#endif /*__SEQLIST_H__*/
