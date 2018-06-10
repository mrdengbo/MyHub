#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include "socket_api.h"

/*
 * select返回值三态
 * 1 若timeout时间到(超时),没有检测到读事件 ret返回0
 * 2 若ret返回<0 &&  errno == EINTR 说明select的过程中被别的信号中断（可中断睡眠原理,）若返回-1，select出错
 * 3 若ret返回值>0 表示有read事件发生，返回事件发生的个数
 * */

#define handle_error(msg) do \
				{ fprintf(stderr, "%s, %s: %s\n", __FUNCTION__, msg, strerror(errno)); \
					exit(EXIT_FAILURE); } while (0) \

#define MAX_SOCKETS 1024  /*定义客户端最大连接个数为1024*/
#define CONN_IP_LEN 32    /*定义服务器连接客户端的IP地址长度*/
typedef int SocketType;   /*socket描述符变量类型*/

/*链式队列节点数据结构类型*/
typedef struct _LinkNode {
	SocketType sock_fd;     /*socket描述符*/
  struct _LinkNode *next; /*指向下一节点指针*/
}	LinkNode;

/*链式队列的前后节点指针数据结构类型*/
typedef struct _LinkQueue {
  struct _LinkNode *front; /*队头节点指针*/
  struct _LinkNode *rear;  /*队尾节点指针*/
}	LinkQueue;

/*定义客户端处理流程的句柄结构体*/
typedef struct _SockClient_Handle {
	LinkQueue *sock_queue;  /*存储连接套接字个数的队列结构*/
	Time conn_time;         /*设置连接超时时间*/
	Time send_time;					/*设置发送超时时间*/
	Time recv_time;					/*设置接收超时时间*/
} SockClient_Handle;

/*定义服务器处理流程的句柄结构体*/
typedef struct _SockServer_Handle {
	int listen_sock;        /*监听套接字*/
	Time accept_time;       /*设置接受超时时间*/
	Time send_time;					/*设置发送超时时间*/
	Time recv_time;					/*设置接收超时时间*/
} SockServer_Handle;

/*定义客户端结构句柄全局指针变量*/
SockClient_Handle *g_client = NULL;
/*定义服务器结构句柄全局指针变量*/
SockServer_Handle *g_server = NULL;

/********************************************
 *函数名称：LinkQueue_create
 *函数描述：创建链式队列
 *函数入参：无
 *函数出参：无
 *函数返回值：LinkQueue  //链式队列结构句柄指针
 												 //成功返回有效句柄指针
 												 //失败返回NULL
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
static LinkQueue *LinkQueue_create()
{
	/*定义队列句柄结构指针*/
  LinkQueue *ret = NULL;
  ret = (LinkQueue *)malloc(sizeof(LinkQueue));
  if (NULL == ret)
      return NULL;
  /*定义队列空节点结构指针*/
  LinkNode *node = NULL;
  node = (LinkNode *)malloc(sizeof(LinkNode));
  if (NULL == node)
       return NULL;
  node->next    = NULL;
  node->sock_fd = 0;
  ret->front    = node;
  ret->rear     = node;
  return ret;
}

/********************************************
 *函数名称：LinkQueue_isEmpty
 *函数描述：判断链式队列是否空
 *函数入参：link_queue   //链式队列结构句柄指针
 *函数出参：无
 *函数返回值：int  //若为空返回1,否则返回0
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
static int LinkQueue_isEmpty(LinkQueue *link_queue)
{
    return link_queue->front == link_queue->rear ? 1 : 0;
}

/********************************************
 *函数名称：LinkQueue_push
 *函数描述：队尾插入节点
 *函数入参：link_queue   //链式队列结构句柄指针
 						sock         //socket描述符
 *函数出参：无
 *函数返回值：int  //成功返回0,否则返回-1
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
static int LinkQueue_push(LinkQueue *link_queue, SocketType sock)
{
    LinkNode *tmp = NULL;
    tmp = (LinkNode *)malloc(sizeof(LinkNode));
    if (NULL ==tmp)
    	return -1;
    tmp->sock_fd = sock;
    tmp->next = NULL;
    link_queue->rear->next = tmp;
    /*尾节点指针后移*/
    link_queue->rear = tmp;
    return 0;
}

/********************************************
 *函数名称：LinkQueue_pop
 *函数描述：队头取出节点
 *函数入参：link_queue   //链式队列结构句柄指针
 *函数出参：无
 *函数返回值：SocketType  //返回对应socket描述符
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
static SocketType LinkQueue_pop(LinkQueue *link_queue)
{
    SocketType tmp_data = 0;
    LinkNode *tmp_node = NULL;
    tmp_data = link_queue->front->next->sock_fd;
    tmp_node = link_queue->front->next;
    link_queue->front->next = tmp_node->next;
    free(tmp_node);
    
    /*当队头节点遍历为NULL时，表明队列已为空*/
    if (link_queue->front->next == NULL)
    {
        link_queue->rear = link_queue->front;
    }
    return tmp_data;
}

/********************************************
 *函数名称：LinkQueue_destroy
 *函数描述：销毁链式队列资源
 *函数入参：link_queue   //链式队列结构句柄指针
 *函数出参：无
 *函数返回值：void
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
static void LinkQueue_destroy(LinkQueue *link_queue)
{
    if (link_queue->front != NULL)
    {
        free(link_queue->front);
    }
    if (link_queue != NULL)
    {
        free(link_queue);
    }
}

/********************************************
 *函数名称：setNonblock
 *函数描述：设置文件描述符为非阻塞模式
 *函数入参：fd      //文件(socket)描述符
 *函数出参：无
 *函数返回值：void
 *作者：xiaoma
 *创建时间：2017年12月25日
 ********************************************/
static void setNonblock(int fd)
{
	/*fcntl函数功能为获取或设置文件描述符属性*/
  int flg = fcntl(fd, F_GETFL);
  if (flg == -1)
		handle_error("get fcntl failed\n");
  flg |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flg) == -1)
    handle_error("set fcntl failed\n");
  return ;
}

/********************************************
 *函数名称：setBlock
 *函数描述：设置文件描述符为阻塞模式
 *函数入参：fd      //文件(socket)描述符
 *函数出参：无
 *函数返回值：void
 *作者：xiaoma
 *创建时间：2017年12月25日
 ********************************************/
static void setBlock(int fd)
{
    int flg = fcntl(fd, F_GETFL);
    if (flg == -1)
        handle_error("get fcntl failed\n");
    flg &= ~O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flg) == -1)
        handle_error("set fcntl failed\n");
    return ;
}

/********************************************
 *函数名称：connectTimeout
 *函数描述：连接超时处理
 *函数入参：sockfd      //等待连接处理的socket描述符
 						addr        //连接指定IP和端口的结构体
 						wait_time   //等待时间大小
 *函数出参：无
 *函数返回值：int       //成功返回0, 失败返回-1
 *作者：xiaoma
 *创建时间：2017年12月25日
 ********************************************/
static int connectTimeout(int sockfd, struct sockaddr_in *addr, Time wait_time)
{
	int ret = 0;
	/*若等待时间大于0,则将socket描述符设置为非阻塞模式,准备做连接超时处理流程*/
	if (wait_time > 0) {
		setNonblock(sockfd);
	}
	ret = connect(sockfd, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
	/* #define	EINPROGRESS	115	 Operation now in progress  */
	if (ret == -1 && errno == EINPROGRESS) {
		/*
		 * void FD_CLR(int fd, fd_set *set);
		 * int  FD_ISSET(int fd, fd_set *set);
		 * void FD_SET(int fd, fd_set *set);
		 * void FD_ZERO(fd_set *set);
		 * */ 
		/*定义存储文件描述符集合表*/
		fd_set conn_fdset;
		/*清空文件描述符集合表*/
		FD_ZERO(&conn_fdset);
    /*添加连接socket描述符到集合表中*/
    FD_SET(sockfd, &conn_fdset);
		/*
		 * struct timeval {
		 *     long    tv_sec;    // seconds       秒
		 *     long    tv_usec;   // microseconds  微秒
		 *     };
		 * */
		/*定义时间结构体变量*/
		struct timeval timeout;
    timeout.tv_sec  = wait_time;
    timeout.tv_usec = 0;
    do {
    	/*
       * int select(int nfds, fd_set *readfds, fd_set *writefds,
       *           fd_set *exceptfds, struct timeval *timeout);
       * */
      /*将连接conn_fdset集合表设置到select事件管理中的写入集合,等待写入事件响应*/
    	ret = select(sockfd + 1, NULL, &conn_fdset, NULL, &timeout);
    } while (ret == -1 && errno == EINTR); /*若在设置的超时时间内,响应事件错误,且错误原因为被信号中断,则继续循环*/
    if (ret == 0) { /*select超时*/
    	/* #define	ETIMEDOUT	110	 Connection timed out  */
    	/*将错误码传递给主调函数,让其判断,并执行连接超时的相应处理*/
			errno = ETIMEDOUT;
			ret = -1;
    } /*if (ret == 0)*/
    else if (ret == -1) { /*select出错*/
    	ret = -1;
    } /*if (ret == -1)*/
    else if (ret > 0) {   /*select响应写入事件*/
    	/*
    	 * 当select机制监控到conn_fdset可读，并不能代表连接是可用的。
	     * 还需要做进一步的异常判断处理
			 * 造成可读有两种结果：1 连接建立成功, 2 连接建立失败,失败会回写失败信息,造成可读。
			 * 此时错误信息不会保存至errno变量中，因此，需要调用getsockopt来获取。
			 */
			 /* 获取socket属性
			  *	int getsockopt(int sockfd, int level, int optname,
        *                void *optval, socklen_t *optlen);
			  */
			 int conn_err;
			 socklen_t sock_len = sizeof(conn_err);
			 if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &conn_err, &sock_len) == -1) { /*getsockopt出错*/
			 	 ret = -1;
			 } 
			 else { /*getsockopt成功获取socket属性*/
			 	 if (conn_err == 0) { /*当conn_err为0,表示建立连接成功*/
			 	 	 ret = 0; /*设置为0返回,表示成功*/
			 	 } /*if (conn_err == 0)*/
			 	 else { /*当conn_err不为0,表示建立连接失败*/
			 	 	 errno = conn_err;
			 	 	 ret = -1;
			 	 }
			 } 
    } /*if (ret > 0)*/
	} /*if (ret == -1 && errno == EINPROGRESS)*/
	/*连接超时函数处理完后恢复socket套接字的阻塞模式*/
	if (wait_time > 0) {
		setBlock(sockfd);
	}
	return ret;
}

/********************************************
 *函数名称：writeTimeout
 *函数描述：写入超时处理
 *函数入参：sockfd      //等待连接处理的socket描述符
 						wait_time   //等待时间大小
 *函数出参：无
 *函数返回值：int       //成功返回0, 失败返回-1
 *作者：xiaoma
 *创建时间：2017年12月26日
 ********************************************/
static int writeTimeout(int sockfd, Time wait_time)
{
	int ret = 0;
	if (wait_time > 0) {
		/*定义存储文件描述符集合表*/
		fd_set write_fdset;
		/*清空文件描述符集合表*/
		FD_ZERO(&write_fdset);
    /*添加连接socket描述符到集合表中*/
    FD_SET(sockfd, &write_fdset);
		/*定义时间结构体变量*/
		struct timeval timeout;
    timeout.tv_sec  = wait_time;
    timeout.tv_usec = 0;
    do {
      /*将write_fdset集合表设置到select事件管理中的写入集合,等待写入事件响应*/
    	ret = select(sockfd + 1, NULL, &write_fdset, NULL, &timeout);
    } while (ret == -1 && errno == EINTR); /*若在设置的超时时间内,响应事件错误,且错误原因为被信号中断,则继续循环*/
    if (ret == 0) { /*select超时*/
    	/* #define	ETIMEDOUT	110	 Connection timed out  */
    	/*将错误码传递给主调函数,让其判断,并执行连接超时的相应处理*/
			errno = ETIMEDOUT;
			ret = -1;
    } 
    else if (ret == -1) { /*select出错*/
    	ret = -1;
    } 
    else if (ret > 0) { /*select成功,响应写入事件*/
    	ret = 0;  /*置为0,表示函数执行成功*/
    }
	} /*if (wait_time > 0)*/
	return ret;
}

/********************************************
 *函数名称：readTimeout
 *函数描述：读取超时处理
 *函数入参：sockfd      //等待连接处理的socket描述符
 						wait_time   //等待时间大小
 *函数出参：无
 *函数返回值：int       //成功返回0, 失败返回-1
 *作者：xiaoma
 *创建时间：2017年12月26日
 ********************************************/
static int readTimeout(int sockfd, Time wait_time)
{
	int ret = 0;
	if (wait_time > 0) {
		/*定义存储文件描述符集合表*/
		fd_set read_fdset;
		/*清空文件描述符集合表*/
		FD_ZERO(&read_fdset);
    /*添加连接socket描述符到集合表中*/
    FD_SET(sockfd, &read_fdset);
		/*定义时间结构体变量*/
		struct timeval timeout;
    timeout.tv_sec  = wait_time;
    timeout.tv_usec = 0;
    do {
      /*将read_fdset集合表设置到select事件管理中的读取集合,等待读取事件响应*/
    	ret = select(sockfd + 1, &read_fdset, NULL, NULL, &timeout);
    } while (ret == -1 && errno == EINTR); /*若在设置的超时时间内,响应事件错误,且错误原因为被信号中断,则继续循环*/
    if (ret == 0) { /*select超时*/
			errno = ETIMEDOUT;
			ret = -1;
    } 
    else if (ret == -1) { /*select出错*/
    	ret = -1;
    } 
    else if (ret > 0) { /*select成功,响应读取事件*/
    	ret = 0;  /*置为0,表示函数执行成功*/
    }
	} /*if (wait_time > 0)*/
	return ret;
} 

/********************************************
 *函数名称：writeByCount
 *函数描述：根据指定个数写入数据
 *函数入参：fd     //文件(socket)描述符
 						buf    //数据缓冲首地址
 						count  //数据缓冲字节大小
 *函数出参：无
 *函数返回值：int  //成功返回的写入数据字节数, 失败返回-1
 *作者：xiaoma
 *创建时间：2017年12月26日
 ********************************************/
static int writeByCount(int fd, const void *buf, size_t count)
{
	int ret = count;
	/*定义返回写入文件数据的字节数*/
	ssize_t ret_val;
	/*定义char* 类型的辅助字符指针并强转输入参数buf的类型*/
	char *tmp = (char *)buf;
	size_t tmp_len = count;
	while (tmp_len > 0) {
		/* ssize_t write(int fd, const void *buf, size_t count); */
		ret_val = write(fd, (const void *)tmp, tmp_len);
		/* 
     * RETURN VALUE
     *  On  success,  the  number  of bytes written is returned (zero indicates
     *  nothing was written).  On error, -1  is  returned,  and  errno  is  set
     *  appropriately.
     *
     *  If  count  is  zero  and  fd refers to a regular file, then write() may
     *  return a failure status if one of the errors below is detected.  If  no
     *  errors  are  detected,  0  will  be  returned without causing any other
     *  effect.  If count is zero and fd refers to a file other than a  regular
     *  file, the results are not specified.
     */
    if (ret_val == -1) {
    	if (errno == EINTR) { /*若写入出错,并且错误原因为被信号所中断,则继续循环写入*/
    		continue;
    	} 
    	else { /*若写入出错,并且错误原因为其它,则设置错误返回值并结束循环*/
    		ret = -1;
    		break; 
    	}
    } /*if (ret_val == -1)*/
    else if (ret_val == 0) { /*表示没数据可写*/
    	continue;
    } /*if (ret_val == 0)*/
    /*字符指针偏移已经写入成功大小的字节数*/
 		tmp += ret_val;
 		/*更新剩余待写入字节数*/
 		tmp_len -= ret_val;
	} /*while (tmp_len > 0)*/
	return ret;
}

/********************************************
 *函数名称：readByCount
 *函数描述：根据指定个数读取数据
 *函数入参：fd     //文件(socket)描述符
 						buf    //数据缓冲首地址
 						count  //数据缓冲字节大小
 *函数出参：无
 *函数返回值：int  //成功返回读取的数据字节数, 失败返回-1
 *作者：xiaoma
 *创建时间：2017年12月26日
 ********************************************/
static int readByCount(int fd, void *buf, size_t count)
{
	int ret = count;
	/*定义返回读取文件数据的字节数*/
	ssize_t ret_val;
	/*定义char* 类型的辅助字符指针并强转输入参数buf的类型*/
	char *tmp = (char *)buf;
	size_t tmp_len = count;
	while (tmp_len > 0) {
		/* ssize_t read(int fd, void *buf, size_t count); */
		ret_val = read(fd, (void *)tmp, tmp_len);
		/* 
     * RETURN VALUE
     * On success, the number of bytes read is returned (zero indicates end of
     * file), and the file position is advanced by this number.  It is not  an
     * error  if  this  number  is smaller than the number of bytes requested;
     * this may happen for example because fewer bytes are actually  available
     * right  now  (maybe  because we were close to end-of-file, or because we
     * are reading from a pipe, or from a terminal),  or  because  read()  was
     * interrupted  by  a  signal.  On error, -1 is returned, and errno is set
     * appropriately.  In this case it is left unspecified  whether  the  file
     * position (if any) changes.
     */
    if (ret_val == -1) {
    	if (errno == EINTR) { /*若写入出错,并且错误原因为被信号所中断,则继续循环写入*/
    		continue;
    	} 
    	else { /*若写入出错,并且错误原因为其它,则设置错误返回值并结束循环*/
    		ret = -1;
    		break; 
    	}
    } /*if (ret_val == -1)*/
    else if (ret_val == 0) { /*表示已经读取到文件末尾*/
    	/*所需要读取的数据字节数count大于0,但此时文件已读取到末尾*/
    	ret = count - tmp_len;
    	break;
    } /*if (ret_val == 0)*/
    /*字符指针偏移已经写入成功大小的字节数*/
 		tmp += ret_val;
 		/*更新剩余待写入字节数*/
 		tmp_len -= ret_val;
	} /*while (tmp_len > 0)*/
	return ret;
}

#if 0
/********************************************
 *函数名称：getCountByRecvBuffer
 *函数描述：获取接收缓冲区中的有效数据字节数
 *函数入参：sockfd     //socket描述符
 						buf        //数据缓冲区首地址
 						buf_len    //数据缓冲区字节数
 *函数出参：无
 *函数返回值：ssize_t  //成功返回获取的数据字节数, 失败返回-1
 *作者：xiaoma
 *创建时间：2017年12月26日
 ********************************************/
static ssize_t getCountByRecvBuffer(int sockfd, void *buf, size_t buf_len)
{
	ssize_t ret = 0;
	while (1)
	{
		/*
		 * ssize_t recv(int sockfd, void *buf, size_t len, int flags);
		 * sockfd 套接字
		 * len 需要读取的长度
		 * MSG_PEEK只从队列中查看，但不取出。
		 * 返回接受到的字节的长度，失败返回-1，接受到关闭信号返回0；
		 * */
		ret = recv(sockfd, buf, len, MSG_PEEK);
		/* 若被信号中断，则继续*/
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}
#endif

/********************************************
 *函数名称：SocketClient_init
 *函数描述：客户端环境初始化
 *函数入参：handle      //内部client socket结构体句柄
 						conn_time   //连接超时时间
 						send_time   //发送超时时间
 						recv_time   //接收超时时间
 						conn_count  //连接套接字个数
 *函数出参：无
 *函数返回值：int       //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月25日
 ********************************************/
int SocketClient_init(ClientHandle **handle, Time conn_time, Time send_time, Time recv_time, int conn_count)
{
	int ret = 0, i;
	int sockfd;
	//Sock_Handle *tmp = NULL;
	if (NULL == handle || conn_time < 0 || send_time < 0 || recv_time < 0) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	if (conn_count <= 0 || conn_count >= MAX_SOCKETS) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, the argument conn_count is invalid, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_client = (SockClient_Handle *)malloc(sizeof(SockClient_Handle));
	if (NULL == g_client) {
		ret = SOCK_ERR_MALLOC;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	/*创建链式队列,用于存储多个socket套接字*/
	LinkQueue *queue = LinkQueue_create();
	if (NULL == queue) {
		ret = SOCK_ERR_CALL;
		fprintf(stderr, "%s, LinkQueue_create() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	for (i = 0; i < conn_count; i ++) {
		/*  IPPROTO_IP  = 0,		  Dummy protocol for TCP	      	*/
		/*  IPPROTO_TCP = 6,		  Transmission Control Protocol	  */
		sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sockfd == -1) {
			ret = SOCK_ERR_SYSTEM;
			fprintf(stderr, "%s, socket() err %d\n", __FUNCTION__, ret);
		  return ret;
		}
		/*队列尾插入套接字描述符*/
		ret = LinkQueue_push(queue, sockfd);	
		if (ret == -1) {
			ret = SOCK_ERR_CALL;
			fprintf(stderr, "%s, LinkQueue_push() err %d\n", __FUNCTION__, ret);
			return ret;
		}
	}
	g_client->sock_queue = queue;
	g_client->conn_time = conn_time;
	g_client->send_time = send_time;
	g_client->recv_time = recv_time;
	*handle = (SockClient_Handle *)g_client;
	g_client = NULL;
	return ret;
}

/********************************************
 *函数名称：SocketClient_getConnSock
 *函数描述：客户端获取连接socket描述符
 *函数入参：handle    //内部client socket结构体句柄
 						ip_addr   //IP地址
 						port      //端口号
 *函数出参：sockfd    //连接套接字描述符
 *函数返回值：int     //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月25日
 ********************************************/
int SocketClient_getConnSock(ClientHandle *handle, const char *ip_addr, int port, int *sockfd)
{
	int ret = 0;
	int conn_sock;
	if (NULL == handle || NULL == ip_addr || port <= 0 || NULL == sockfd) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_client = (SockClient_Handle *)handle;
	if (!LinkQueue_isEmpty(g_client->sock_queue)) {
		/*从队列头取出有效套接字*/
		conn_sock = LinkQueue_pop(g_client->sock_queue);
	}
	else {
		ret = SOCK_ERR_CALL;
		fprintf(stderr, "%s, current queue has empty, LinkQueue_isEmpty() %d\n", __FUNCTION__, ret);
		return ret;
	}

	if (conn_sock == -1) {
		ret = SOCK_ERR_CALL;
		fprintf(stderr, "%s, LinkQueue_pop() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	/*
   *struct sockaddr_in {
   *	 sa_family_t    sin_family; // address family: AF_INET
   *	 in_port_t      sin_port;   // port in network byte order
   *	 struct in_addr sin_addr;   // internet address
   *};
   */
	struct sockaddr_in addr;
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = PF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip_addr);
	ret = connectTimeout(conn_sock, &addr, g_client->conn_time);
	if (ret == -1) {
		if (errno == ETIMEDOUT) {
  		ret = SOCK_ERR_TIMEOUT;
			fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
			return ret;
  	} else {
  		ret = SOCK_ERR_CALL;
			fprintf(stderr, "%s, connectTimeout() err %d\n", __FUNCTION__, ret);
			return ret;
  	}
	}
  *sockfd = conn_sock;
	return ret;
}

/********************************************
 *函数名称：SocketClient_send
 *函数描述：客户端发送数据报
 *函数入参：handle      //内部client socket结构体句柄
 						sockfd      //socket套接字描述符
 						data        //发送数据报内容地址
 						data_length //发送数据报内容大小
 *函数出参：无
 *函数返回值：int       //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月26日
 ********************************************/
int SocketClient_send(ClientHandle *handle, int sockfd, const Datagram *data, int data_length)
{
	int ret = 0;
	if (NULL == handle || sockfd < 0 || NULL == data || data_length <= 0) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_client = (SockClient_Handle *)handle;
	ret = writeTimeout(sockfd, g_client->send_time);
	if (ret == -1) {
		if (errno == ETIMEDOUT) {
  		ret = SOCK_ERR_TIMEOUT;
			fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
			return ret;
  	} else {
  		ret = SOCK_ERR_CALL;
			fprintf(stderr, "%s, writeTimeout() err %d\n", __FUNCTION__, ret);
			return ret;
  	}
	}
	Datagram *data_buf = (Datagram *)malloc(data_length + sizeof(int));
	if (NULL == data_buf) {
		ret = SOCK_ERR_MALLOC;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	/*将长度数字的int变量内容转换为网络字节序*/
	int net_length = htonl(data_length);
	/*void *memcpy(void *dest, const void *src, size_t n);*/
	/*组装数据报的包头的前4字节存储数据报内容字节数大小*/
	memcpy(data_buf, (const void *)&net_length, sizeof(int));
	/*组装数据报的包体数据内容*/
	memcpy(data_buf + sizeof(int), (const void *)data, data_length);
	ret = writeByCount(sockfd, (const void *)data_buf, data_length + sizeof(int));
	if (ret == -1) {
		if (data_buf != NULL) {
			free(data_buf);
			data_buf = NULL;
		}
		ret = SOCK_ERR_CALL;
		fprintf(stderr, "%s, writeByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	else if (ret < (data_length + sizeof(int))) { /*当实际发送的字节数小于所需发送的数据时,发送数据不完整,表明对方已关闭*/
		ret = SOCK_ERR_CLOSE;
		fprintf(stderr, "%s, writeByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	ret = 0;
	if (data_buf != NULL) {
		free(data_buf);
		data_buf = NULL;
	}
	return ret;
}

/********************************************
 *函数名称：SocketClient_recv
 *函数描述：客户端接收数据报
 *函数入参：handle      //内部client socket结构体句柄
 						sockfd      //socket套接字描述符
 *函数出参：data        //接收数据报内容地址
 						data_length //接收数据报内容大小
 *函数返回值：int       //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月26日
 ********************************************/
int SocketClient_recv(ClientHandle *handle, int sockfd, Datagram *data, int *data_length)
{
	int ret = 0;
	if (NULL == handle || sockfd < 0 || NULL == data || data_length <= 0) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_client = (SockClient_Handle *)handle;
	ret = readTimeout(sockfd, g_client->recv_time);
	if (ret == -1) {
		if (errno == ETIMEDOUT) {
  		ret = SOCK_ERR_TIMEOUT;
			fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
			return ret;
  	} else {
  		ret = SOCK_ERR_CALL;
			fprintf(stderr, "%s, readTimeout() err %d\n", __FUNCTION__, ret);
			return ret;
  	}
	}
	/*定义网络字节序变量*/
	int net_length;
	/*解析数据报包头,获取数据报包体长度*/
	ret = readByCount(sockfd, (void *)&net_length, sizeof(int));
	if (ret == -1) {
		ret = SOCK_ERR_CALL;
		fprintf(stderr, "%s, readByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	/*网络字节序转换为本机字节序*/
	int recv_length = ntohl(net_length);
	/*根据解析的数据报长度来接收(读)数据*/
	ret = readByCount(sockfd, (void *)data, recv_length);
	if (ret == -1) {
		ret = SOCK_ERR_CALL;
		fprintf(stderr, "%s, readByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	} 
	else if (ret < recv_length) { /*当实际接收的字节数小于所需接收的数据时,接收数据不完整,表明对方已关闭*/
		ret = SOCK_ERR_CLOSE;
		fprintf(stderr, "%s, readByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	ret = 0;
	/*将接收的数据报长度结果返回给形参*/
	*data_length = recv_length;
	return ret;
}

/********************************************
 *函数名称：SocketClient_close
 *函数描述：客户端socket描述符关闭
 *函数入参：sockfd      //socket套接字描述符
 *函数出参：无
 *函数返回值：void
 *作者：xiaoma
 *创建时间：2017年12月26日
 ********************************************/
void SocketClient_close(int sockfd)
{
	close(sockfd);
	return ;
}

/********************************************
 *函数名称：SocketClient_destroy
 *函数描述：客户端环境销毁
 *函数入参：handle      //内部client socket结构体句柄
 *函数出参：无
 *函数返回值：int       //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月26日
 ********************************************/
int SocketClient_destroy(ClientHandle **handle)
{
	int ret = 0;
	if (NULL == handle) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_client = (SockClient_Handle *)*handle;
	while (!LinkQueue_isEmpty(g_client->sock_queue)) {
		/*逐个关闭创建成功,但未使用的socket套接字描述符*/
		close(LinkQueue_pop(g_client->sock_queue));
	}
	/*销毁链式队列结构*/
	LinkQueue_destroy(g_client->sock_queue);
	free(*handle);
	/*间接地修改实参的值,以避免野指针*/
	*handle = NULL;
	return ret;
}

/********************************************
 *函数名称：createListenSock
 *函数描述：创建服务器端监听套接字
 *函数入参：port    //端口号
 						ip      //ip地址
 *函数出参：无
 *函数返回值：int       //返回监听套接字
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
static int createListenSock(int port, const char *ip)
{
  int ret_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (ret_sock == -1)
		handle_error("socket failed");
  int on = 1;
  /*设置用户地址可重用*/
	if (setsockopt(ret_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		handle_error("setsockopt failed");
		return 0;
	}
	/* 
	 * struct sockaddr_in {
   * uint8_t  sin_len;         //4
	 * sa_family_t  sin_family;  //4
	 * in_port_t	sin_port;      //2
	 * struct in_addr	sin_addr;  //4
	 * char sin_zero[8];         //8
	 * }; 
	 * */
  struct sockaddr_in addr;
  /*协议簇通信类型为ipv4*/
  addr.sin_family = AF_INET;
  /*端口号的本地字节序转换为网络字节序*/
  addr.sin_port = htons(port);
  if (NULL == ip) { 
		/* 
     * Address to accept any incoming messages. 
		 * #define	INADDR_ANY		((unsigned long int) 0x00000000)
		 */
		/*若输入参数IP为空,则将服务器绑定地址设置为接收任意类型*/
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
  }
  else {
    /*若输入参数IP不为空,则设置为服务器绑定指定IP地址*/
    addr.sin_addr.s_addr = inet_addr(ip);
  }   
  if (bind(ret_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		handle_error("bind failed");
  if (listen(ret_sock, 30) == -1)
		handle_error("listen failed");
  fprintf(stdout, "listen success\n");
	return ret_sock;
}

/********************************************
 *函数名称：acceptTimeout
 *函数描述：服务器接受对方客户端的超时处理
 *函数入参：listen_sock    //端口号
 						peer_addr      //套接字地址结构
 						wait_time      //超时时间
 *函数出参：无
 *函数返回值：int       //成功返回接受对方客户端连接的socket描述符,失败返回-1
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
static int acceptTimeout(int sock_fd, struct sockaddr_in *peer_addr, Time wait_time)
{
	int ret = 0;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	if (wait_time > 0) {
		/*定义存储文件描述符集合表*/
		fd_set accept_fdset;
		/*清空文件描述符集合表*/
		FD_ZERO(&accept_fdset);
    /*添加连接socket描述符到集合表中*/
    FD_SET(sock_fd, &accept_fdset);
		/*定义时间结构体变量*/
		struct timeval timeout;
    timeout.tv_sec  = wait_time;
    timeout.tv_usec = 0;
    do {
    	/*在规定时间内,执行select函数事件管理来监测sock_fd的写入事件*/
    	ret = select(sock_fd + 1, &accept_fdset, NULL, NULL, &timeout);
    } while (ret == -1 && errno == EINTR);
    if (ret == 0) { /*select成功超时*/
    	errno = ETIMEDOUT;
    	ret = -1;
			return ret;
    } 
    else if (ret == -1) { /*select出错*/
    	ret = -1;
			return ret;
    }
	} /*if (wait_time > 0)*/
	/*int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);*/
	if (NULL == peer_addr) { /*若输入参数为NULL,做相应处理*/
		ret = accept(sock_fd, NULL, NULL);
	} 
	else {
		ret = accept(sock_fd, (struct sockaddr *)peer_addr, &addr_len);
	}
	return ret;
} 

/********************************************
 *函数名称：getAddrStringBySockaddr
 *函数描述：通过struct sockaddr_in地址结构获取IP地址字符串
 *函数入参：addr         			//地址结构
 						ip           			//IP地址字符串
 *函数出参：无
 *函数返回值：const char *    //返回IP地址字符串
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
static const char *getAddrStringBySockaddr(const struct sockaddr_in *addr, char *ip_str)
{
	unsigned char *p = (unsigned char *)&(addr->sin_addr.s_addr);
	sprintf(ip_str, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
	return (const char *)ip_str;
}

/********************************************
 *函数名称：SocketServer_init
 *函数描述：服务器环境初始化
 *函数入参：handle        //内部server socket结构体句柄
 						accept_time   //接受超时时间
 						send_time     //发送超时时间
 						recv_time     //接收超时时间
 						port          //端口号
 						ip_addr       //ip地址
 *函数出参：无
 *函数返回值：int         //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
int SocketServer_init(ServerHandle **handle, Time accept_time, Time send_time, Time recv_time, 
											int port, const char *ip_addr)
{
	int ret = 0;
	if (NULL == handle || port <= 0 || accept_time < 0 || send_time < 0 || recv_time < 0) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_server = (SockServer_Handle *)malloc(sizeof(SockServer_Handle));
	if (NULL == g_server) {
		ret = SOCK_ERR_MALLOC;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_server->listen_sock = createListenSock(port, ip_addr);
	g_server->accept_time = accept_time;
	g_server->send_time 	= send_time;
	g_server->recv_time 	= recv_time;
	*handle = (ServerHandle *)g_server;
	g_server = NULL;
	return ret;
}

/********************************************
 *函数名称：SocketServer_accept
 *函数描述：服务器接受对方客户端的连接
 *函数入参：handle        //内部server socket结构体句柄
 						peer_ip       //连接客户端的IP地址字符串
 						peer_ip_len   //连接客户端的IP地址字符串长度
 *函数出参：peer_sock     //连接客户端的socket描述符
 *函数返回值：int         //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
int SocketServer_accept(ServerHandle *handle, char *peer_ip, int peer_ip_len, int *peer_sock)
{
	int ret = 0;
	if (NULL == handle || NULL == peer_sock) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_server = (SockServer_Handle *)handle;
	if (NULL == peer_ip) { /*若输入参数peer_ip为空,则执行相应处理流程*/
		ret = acceptTimeout(g_server->listen_sock, NULL, g_server->accept_time);
	}
	else if (NULL != peer_ip){
		if (peer_ip_len < CONN_IP_LEN) {
			ret = SOCK_ERR_PARA;
			fprintf(stderr, "%s, the argument peer_ip_len is invalid, err %d\n", __FUNCTION__, ret);
			return ret;
		}
		else {
			struct sockaddr_in addr;
			ret = acceptTimeout(g_server->listen_sock, &addr, g_server->accept_time);
			if (ret != -1) {
				/*若输入参数peer_ip不为空,则在接受对方连接成功后,根据addr地址结构获取ip字符串*/
				getAddrStringBySockaddr(&addr, peer_ip);
			}
		}
	} /*if (NULL == peer_ip)*/
	if (ret == -1) {
		if (errno == ETIMEDOUT) {
  		ret = SOCK_ERR_TIMEOUT;
			fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
			return ret;
  	} else {
  		ret = SOCK_ERR_CALL;
			fprintf(stderr, "%s, acceptTimeout() err %d\n", __FUNCTION__, ret);
			return ret;
  	}
	}
	*peer_sock = ret;
	ret = 0;
	return ret;
}

 /********************************************
 *函数名称：SocketServer_send
 *函数描述：服务器发送数据报
 *函数入参：handle        //内部server socket结构体句柄
 						peer_sock     //socket套接字描述符
 						data          //发送数据报内容地址
 						data_length   //发送数据报内容大小
 *函数出参：无				
 *函数返回值：int       //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
int SocketServer_send(ServerHandle *handle, int peer_sock, const Datagram *data, int data_length)
{
	int ret = 0;
	if (NULL == handle || peer_sock < 0 || NULL == data || data_length <= 0) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_server = (SockServer_Handle *)handle;
	ret = writeTimeout(peer_sock, g_server->send_time);
	if (ret == -1) {
		if (errno == ETIMEDOUT) {
  		ret = SOCK_ERR_TIMEOUT;
			fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
			return ret;
  	} else {
  		ret = SOCK_ERR_CALL;
			fprintf(stderr, "%s, writeTimeout() err %d\n", __FUNCTION__, ret);
			return ret;
  	}
	}
	Datagram *data_buf = (Datagram *)malloc(data_length + sizeof(int));
	if (NULL == data_buf) {
		ret = SOCK_ERR_MALLOC;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	/*将长度数字的int变量内容转换为网络字节序*/
	int net_length = htonl(data_length);
	/*void *memcpy(void *dest, const void *src, size_t n);*/
	/*组装数据报的包头的前4字节存储数据报内容字节数大小*/
	memcpy(data_buf, (const void *)&net_length, sizeof(int));
	memcpy(data_buf + sizeof(int), (const void *)data, data_length);
	ret = writeByCount(peer_sock, (const void *)data_buf, data_length + sizeof(int));
	if (ret == -1) {
		if (data_buf != NULL) {
			free(data_buf);
			data_buf = NULL;
		}
		ret = SOCK_ERR_CALL;
		fprintf(stderr, "%s, writeByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	else if (ret < (data_length + sizeof(int))) { /*当实际发送的字节数小于所需发送的数据时,发送数据不完整,表明对方已关闭*/
		ret = SOCK_ERR_CLOSE;
		fprintf(stderr, "%s, writeByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	ret = 0;
	if (data_buf != NULL) {
		free(data_buf);
		data_buf = NULL;
	}
	return ret;
}

 /********************************************
 *函数名称：SocketServer_recv
 *函数描述：服务器接收数据报
 *函数入参：handle        //内部server socket结构体句柄
 						peer_sock     //socket套接字描述符	
 *函数出参：data          //接收数据报内容地址
 						data_length   //接收数据报内容大小			
 *函数返回值：int       //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
int SocketServer_recv(ServerHandle *handle, int peer_sock, Datagram *data, int *data_length)
{
	int ret = 0;
	if (NULL == handle || peer_sock < 0 || NULL == data || data_length <= 0) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_server = (SockServer_Handle *)handle;
	ret = readTimeout(peer_sock, g_server->recv_time);
	if (ret == -1) {
		if (errno == ETIMEDOUT) {
  		ret = SOCK_ERR_TIMEOUT;
			fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
			return ret;
  	} else {
  		ret = SOCK_ERR_CALL;
			fprintf(stderr, "%s, readTimeout() err %d\n", __FUNCTION__, ret);
			return ret;
  	}
	}
	/*定义网络字节序变量*/
	int net_length;
	/*解析数据报,获取数据报长度*/
	ret = readByCount(peer_sock, (void *)&net_length, sizeof(int));
	if (ret == -1) {
		ret = SOCK_ERR_CALL;
		fprintf(stderr, "%s, readByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	/*网络字节序转换为本机字节序*/
	int recv_length = ntohl(net_length);
	/*根据解析的数据报长度来接收(读)数据*/
	ret = readByCount(peer_sock, (void *)data, recv_length);
	if (ret == -1) {
		ret = SOCK_ERR_CALL;
		fprintf(stderr, "%s, readByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	} 
	else if (ret < recv_length) { /*当实际接收的字节数小于所需接收的数据时,接收数据不完整,表明对方已关闭*/
		ret = SOCK_ERR_CLOSE;
		fprintf(stderr, "%s, readByCount() err %d\n", __FUNCTION__, ret);
		return ret;
	}
	ret = 0;
	/*将接收的数据报长度结果返回给形参*/
	*data_length = recv_length;
	return ret;
}

 /********************************************
 *函数名称：SocketServer_closeClient
 *函数描述：服务器的对方客户端socket描述符关闭
 *函数入参：peer_sock     //socket套接字描述符			
 *函数返回值：void
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
void SocketServer_closeClient(int peer_sock)
{
	close(peer_sock);
	return ;
}

 /********************************************
 *函数名称：SocketServer_closeListen
 *函数描述：服务器关闭监听socket描述符
 *函数入参：handle        //内部server socket结构体句柄	
 *函数出参：无		
 *函数返回值：int       //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
int SocketServer_closeListen(ServerHandle *handle)
{
	int ret = 0;
	if (NULL == handle) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	g_server = (SockServer_Handle *)handle;
	close(g_server->listen_sock);
	return ret;
}

 /********************************************
 *函数名称：SocketServer_destroy
 *函数描述：服务器环境销毁
 *函数入参：handle        //内部server socket结构体句柄	
 *函数出参：无		
 *函数返回值：int       //成功返回0, 失败返回错误码
 *作者：xiaoma
 *创建时间：2017年12月27日
 ********************************************/
int SocketServer_destroy(ServerHandle **handle)
{
	int ret = 0;
	if (NULL == handle) {
		ret = SOCK_ERR_PARA;
		fprintf(stderr, "%s, err %d\n", __FUNCTION__, ret);
		return ret;
	}
	free(*handle);
	/*间接地修改实参的值,以避免野指针*/
	*handle = NULL;
	return ret;
}
