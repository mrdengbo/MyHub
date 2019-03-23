/********************************************************************
* 项目名称:socket_api                                               *
* 项目描述:基于TCP/IP协议的socket客户端/服务器通信接口的封装与设计  *
* 项目开发人员:xiaoma                                        				*
* 项目开发日期:2017-12-25																		 				*
* 项目修改日期:2017-12-27                                    				*
*********************************************************************/

#ifndef __SOCKET_API_H__
#define __SOCKET_API_H__
#ifdef __cplusplus
extern 'C'
{
#endif

typedef void          ClientHandle;   /*客户端对象句柄*/
typedef void          ServerHandle;   /*服务器对象句柄*/
typedef unsigned int  Time;           /*超时时间设定(秒为单位)*/
typedef unsigned char Datagram;       /*数据报传输类型*/

enum Socket_Error {
	SOCK_BASE = 400 ,
	SOCK_ERR_PARA   ,  /*参数错误*/
	SOCK_ERR_TIMEOUT,  /*处理超时错误*/
	SOCK_ERR_CLOSE  ,  /*关闭错误*/
	SOCK_ERR_MALLOC ,  /*资源申请错误*/
	SOCK_ERR_CALL   ,  /*函数调用错误*/
	SOCK_ERR_SYSTEM    /*系统函数错误*/
};

/****************************************SocketClient**************************************************/

/*客户端环境初始化*/
int SocketClient_init(ClientHandle **handle, Time conn_time, Time send_time, Time recv_time, int conn_count);

/*客户端获取连接socket描述符*/
int SocketClient_getConnSock(ClientHandle *handle, const char *ip_addr, int port, int *sockfd);

/*客户端发送数据报*/
int SocketClient_send(ClientHandle *handle, int sockfd, const Datagram *data, int data_length);

/*客户端接收数据报*/
int SocketClient_recv(ClientHandle *handle, int sockfd, Datagram *data, int *data_length);

/*客户端socket描述符关闭*/
void SocketClient_close(int sockfd);

/*客户端环境销毁*/
int SocketClient_destroy(ClientHandle **handle);


/****************************************SocketServer**************************************************/

/*服务器环境初始化*/
int SocketServer_init(ServerHandle **handle, Time accept_time, Time send_time, Time recv_time, 
											int port, const char *ip_addr);
											
/*服务器接受对方客户端的连接*/
int SocketServer_accept(ServerHandle *handle, char *peer_ip, int peer_ip_len, int *peer_sock);

/*服务器发送数据报*/
int SocketServer_send(ServerHandle *handle, int peer_sock, const Datagram *data, int data_length);

/*服务器接收数据报*/
int SocketServer_recv(ServerHandle *handle, int peer_sock, Datagram *data, int *data_length);

/*服务器关闭对方客户端的socket描述符*/
void SocketServer_closeClient(int peer_sock);

/*服务器关闭监听socket描述符*/
int SocketServer_closeListen(ServerHandle *handle);

/*服务器环境销毁*/
int SocketServer_destroy(ServerHandle **handle);


#ifdef __cpluspluse
}
#endif

#endif /*__SOCKET_API_H__*/
