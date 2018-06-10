//#include <stdio.h>
#include <stdlib.h>
#include "seqlist.h"

/*选择unsigned int数据类型的原因是该数据类型的字节大小与指针的字节大小相同,类似与指针数组,用于存储地址数据*/
typedef unsigned int pArray;

/*内部顺序表结构数据类型*/
typedef struct _TagSeqList {
    int _length;        /*有效数据长度*/
    int _capacity;      /*容量大小*/
    pArray *_node;      /*节点顺序表动态数组*/
}TagSeqList;

TagSeqList *g_taglist = NULL;
SeqList *g_list = NULL;

/********************************************
 *函数名称：SeqList_Length
 *函数描述：获取顺序表当前长度
 *函数入参：void
 *函数出参：无
 *函数返回值：int
 *作者：xiaoma
 *创建时间：2017年11月29日
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
 *函数名称：SeqList_Capacity
 *函数描述：获取顺序表容量
 *函数入参：void
 *函数出参：无
 *函数返回值：int
 *作者：xiaoma
 *创建时间：2017年11月29日
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
 *函数名称：SeqList_Insert
 *函数描述：插入指定位置数据节点
 *函数入参：node   //节点指针
            pos    //插入位置
 *函数出参：无
 *函数返回值：int  //成功返回0,失败返回非0
 *作者：xiaoma
 *创建时间：2017年11月29日
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
  /*判断数组容量是否已满*/
  if (g_taglist->_length >= g_taglist->_capacity) {
    return -3;
  }
  /*如果插入位置大于当前元素个数，则默认插入在当前末尾元素后面*/
  if (pos > g_taglist->_length) {
    pos = g_taglist->_length;
  }
  /*从后向前遍历，将前节点元素赋值给后节点元素*/
  for (i = g_taglist->_length; i > pos; i--) {
    g_taglist->_node[i] = g_taglist->_node[i - 1];
  }
  /*node作为元素地址值存储在数组中*/
  g_taglist->_node[pos] = (pArray)node;
  g_taglist->_length++;
  return 0;
}

/********************************************
 *函数名称：SeqList_Get
 *函数描述：获取指定位置数据节点
 *函数入参：pos    //插入位置
 *函数出参：无
 *函数返回值：SeqListNode  //节点指针
 *作者：xiaoma
 *创建时间：2017年11月29日
 ********************************************/
SeqListNode *SeqList_Get(int pos)
{
	g_taglist = g_list->_this->_taglist;
	/*默认从0开始记录位置*/
  if (NULL == g_taglist || pos < 0 || pos >= g_taglist->_length) {
    return NULL;
  }
  return (SeqListNode *)g_taglist->_node[pos];
}

/********************************************
 *函数名称：SeqList_Remove
 *函数描述：移除指定位置数据节点
 *函数入参：pos    //插入位置
 *函数出参：无
 *函数返回值：SeqListNode  //节点指针
 *作者：xiaoma
 *创建时间：2017年11月29日
 ********************************************/
SeqListNode *SeqList_Remove(int pos)
{
	g_taglist = g_list->_this->_taglist;
  if (NULL == g_taglist || pos < 0 || pos >= g_taglist->_length) {
    return NULL;
  }
  int i;
  SeqListNode *ret = (SeqListNode *)g_taglist->_node[pos];
  /*移除节点的后面所有节点向前移动一位*/
  for (i = pos + 1; i < g_taglist->_length; i++) {
    g_taglist->_node[i - 1] = g_taglist->_node[i];
  }
  g_taglist->_length--;
  return  ret;
}

/********************************************
 *函数名称：SeqList_Clear
 *函数描述：清空顺序表
 *函数入参：void
 *函数出参：无
 *函数返回值：void
 *作者：xiaoma
 *创建时间：2017年11月29日
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
 *函数名称：SeqList_Destory
 *函数描述：销毁顺序表
 *函数入参：void
 *函数出参：无
 *函数返回值：void
 *作者：xiaoma
 *创建时间：2017年11月29日
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
 *函数名称：SeqList_Construction
 *函数描述：顺序表对象的构造函数
 *函数入参：count   //创建顺序表的容量大小
 *函数出参：无
 *函数返回值：SeqList  //顺序表对象指针
 *作者：xiaoma
 *创建时间：2017年11月29日
 ********************************************/
SeqList *SeqList_Construction(int count)
{
  g_list = (SeqList *)malloc(sizeof(SeqList));
  if (NULL == g_list) {
    return NULL;
  }
  /*结构体与结构体成员变量共同申请一块连续的内存，节省步骤及方便资源释放操作*/
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
  /*ret结构体类型指针偏移一个结构体大小，指针指向动态数组（成员变量）内存首地址*/
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
 *函数名称：SeqList_Destruction
 *函数描述：顺序表对象的析构函数
 *函数入参：void
 *函数出参：无
 *函数返回值：void
 *作者：xiaoma
 *创建时间：2017年11月29日
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
