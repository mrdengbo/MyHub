#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

#include "work.h"
#include "mysqlite.h"
#include "seqlist.h"
#include "socket_api.h"
#include "msgqueue.h"
#include "semaphore.h"
#include "mysignal.h"

#define DEBUG_WORK fprintf(stdout, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__)

#define SEM_COUNT 2
#define SEND_MSG(arg) 	memcpy(buf + DATAGRAM_MSG, arg, strlen(arg)); \
												ret = sendDatagram(g_pa->handle, g_pa->sockfd, (const void *)buf, DATAGRAM_MSG + strlen(arg))

/*定义参数传递的全局变量*/
Parameter *g_pa = NULL;
/*定义当前客户端的全局唯一标识ID*/
int g_id = 0;
/*定义统计好友个数的全局变量*/
int g_count = 0;
/*定义消息队列句柄的全局变量*/
int g_msg_id = 0;
/*定义信号量集句柄的全局变量*/
int g_sem_id = 0;

/*获取消息头数据结构*/
static MsgControl *getMsgControl(void *data)
{
	return (MsgControl *)data;
}

/*获取当前时间*/
static const char *getCurTime()
{
	static char str[32];
	time_t p_time;
	p_time = time(NULL);
	struct tm *s_tm;
	s_tm = localtime(&p_time);
	int var_year = s_tm->tm_year + 1900;
	int var_month = s_tm->tm_mon + 1;
	int var_mday = s_tm->tm_mday;
	int var_hour = s_tm->tm_hour;
	int var_min = s_tm->tm_min;
	int var_sec = s_tm->tm_sec;
	memset(str, 0, sizeof(str));
	sprintf(str, "%4d-%2d-%2d %2d:%2d:%2d", var_year, var_month, var_mday, var_hour, var_min, var_sec);
	return str;
}

/*向客户端发送数据报*/
static int sendDatagram(ServerHandle *handle, int sockfd, const void *data, int length)
{
	int ret = WORK_SUCCESS;
	ret = SocketServer_send(handle, sockfd, (const Datagram *)data, length);
  if (ret != WORK_SUCCESS) {
    if (ret == SOCK_ERR_TIMEOUT) {
      fprintf(stdout, "send timeout\n");
      return CLIENT_SEND_TIMEOUT;
    }
    fprintf(stderr, "SocketServer_send() failed\n");
  }
  return ret;
}

/*信号处理函数*/
void handler(int sig_num, siginfo_t *val, void *arg)
{
	Parameter *pa = (Parameter *)val->si_value.sival_ptr;
	switch (sig_num) {
  	case SIGRTMIN_T:
  			 DEBUG_WORK;
				 printf("g_id: %d", g_id);
         printf("%s, sig_num: %d\n", __FUNCTION__, sig_num);
         printf("%s, data: %s, length: %d\n", __FUNCTION__, (char *)pa->data + DATAGRAM_MSG, pa->length - DATAGRAM_MSG);
         sendDatagram(pa->handle, pa->sockfd, (const void *)pa->data, pa->length);
         break;
    default:
    		fprintf(stderr, "other signal failed, %d\n", sig_num);
        break;
    }
}

/*线程处理函数*/
void *Start(void *arg)
{
	/*设置为可分离线程属性*/
	pthread_detach(pthread_self());
	Parameter *tmp = (Parameter *)arg;
	Datagram *data = (Datagram *)malloc(DATA_SIZE);
	Parameter *pa = (Parameter *)malloc(sizeof(Parameter));
	pa->handle = tmp->handle;
	pa->sockfd = tmp->sockfd;
	pa->data = (Datagram *)malloc(BUF_SIZE);
	pa->length = 0;
	MsgControl msg;
	int length;
	int peer_id;
	printf("pthread begin, pid: %lu\n", pthread_self());
	while (1) {
		memset(data, 0, DATA_SIZE);
		if (IPC_MsgRecv(g_msg_id, (void *)data, &length, g_id) == MSG_FAILURE) {
			fprintf(stderr, "IPC_MsgRecv() failed\n");
			break;
		}
		/*解析数据的前四个字节*/
		peer_id = *((int *)data);
		/*添加好友*/
		if (peer_id == -1) { 
			DEBUG_WORK;
			printf("g_id: %d", g_id);
			memset(pa->data, 0, BUF_SIZE);
			memcpy(pa->data, (const void *)data + sizeof(int), DATAGRAM_MSG + DATAGRAM_USER);
			pa->length = DATAGRAM_MSG + DATAGRAM_USER;
		}
		/*正常转发消息*/
		else {
			memset(&msg, 0, DATAGRAM_MSG);
			msg.server_reply = MSG_SEND;
			msg.head_1 = g_id;
			msg.head_2 = peer_id;
			memset(pa->data, 0, BUF_SIZE);
			memcpy(pa->data, (const void *)&msg, DATAGRAM_MSG);
			memcpy(pa->data + DATAGRAM_MSG, (void *)data + sizeof(int), length - sizeof(int));
			pa->length = DATAGRAM_MSG + length - sizeof(int);
		}
		/*发送信号*/
		if (sendSignal(SIGRTMIN, (void *)pa) == SIGNAL_FAILURE) {
			fprintf(stderr, "sendSignal() failed\n");
		}
	}
	printf("pthread end, pid: %lu\n", pthread_self());
	if (pa->data != NULL) {
		free(pa->data);
	}
	if (pa != NULL) {
		free(pa);
	}
	if (data != NULL) {
		free(data);
	}
	pthread_exit(NULL);
}

/*清空顺序表中的所有好友信息数据*/
static void clearFriendList(SeqList *list)
{
	UserInfo *tmp = NULL;
  while (list->length()) {
		tmp = (UserInfo *)list->remove(list->length() - 1);
		if (NULL != tmp) {
			free(tmp);
			tmp = NULL;	
		}
	}
	list->clear();
}

/*从数据库中获取用户信息并载入到user参数中,成功返回0,失败返回-1*/
static int loadUserInfo(UserInfo *user)
{
	int ret = 0;
	char **result = NULL;
	int row, column, index = 0;
	IPC_SemP_2(g_sem_id);
	ret = getUserInfo(user->id, &result, &row, &column);
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE)
		return WORK_FAILURE;
	index = column;
	strcpy(user->ap.account, result[index]);
	strcpy(user->ap.passwd,  result[index + 1]);
	strcpy(user->name,  result[index + 2]);
	strcpy(user->indiv, result[index + 3]);
	strcpy(user->regis, result[index + 4]);
	strcpy(user->last,  result[index + 5]);
	user->status = atoi(result[index + 6]);
	return WORK_SUCCESS;
}

/*获取好友ID*/
static int getFriendInfo(SeqList *list, int *count)
{
	int ret = 0;
	char **result = NULL;
	int row, column, i, index;
	UserInfo *tmp = NULL;
	/*在获取新的所有好友信息前,先清空原先信息*/
	clearFriendList(list);
	IPC_SemP_2(g_sem_id);
	/*row 查询结果的行数,column 查询结果的列数*/
	ret = getFriendsId(g_id, &result, &row, &column);
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE)
		return WORK_FAILURE;
	index = column;
	//printf("%s-------------------------------------begin\n", __FUNCTION__);
	for (i = 0; i < row; i++)
	{
		tmp = (UserInfo *)malloc(sizeof(UserInfo));
		if (NULL == tmp) {
			return WORK_FAILURE;
		}
		memset(tmp, 0, DATAGRAM_USER);
		/*转换字符串获取ID*/
		tmp->id = atoi(result[index]);
		/*获取好友信息*/
		loadUserInfo(tmp);
		
		//printf("name: %s\n", tmp->name);
		//printf("individual: %s\n", tmp->indiv);
		//printf("regis: %s\n", tmp->regis);
		//printf("last: %s\n", tmp->last);
		//printf("status: %d\n", tmp->status);
		
		/*插入顺序表中*/
		list->insert((SeqListNode *)tmp, list->length());
		index += column;
	}
	//printf("%s-------------------------------------end\n", __FUNCTION__);
	/*统计好友个数*/
	*count = row;
	return WORK_SUCCESS;
}

/*服务器处理流程后向客户端发送的反馈消息说明*/
static int feedBackMsg(const void *arg, int msg_type, int result_type, int err_type)
{
	int ret = WORK_SUCCESS;
	char buf[BUF_SIZE] = {0};
	MsgControl msg;
	memset(&msg, 0, sizeof(MsgControl));
	msg.client_request = result_type;
	msg.server_reply   = msg_type;
	memcpy(buf, (const void *)&msg, DATAGRAM_MSG);
	g_pa = (Parameter *)arg;
	if (result_type == CLIENT_SUCCESS) {
		return sendDatagram(g_pa->handle, g_pa->sockfd, (const void *)buf, DATAGRAM_MSG);
	}
	switch (err_type) {
		case ACCOUNT_ERR:
			SEND_MSG(ACCOUNT_ERR_STR);
    	break;
    case AP_ERR:
    	SEND_MSG(AP_ERR_STR);
    	break;
    case USER_ONLINE_ERR:
    	SEND_MSG(USER_ONLINE_ERR_STR);
    	break;
    case STATUS_ERR:
    	SEND_MSG(STATUS_ERR_STR);
    	break;
    case LOAD_ERR:
    	SEND_MSG(LOAD_ERR_STR);
    	break;
    case USER_ERR:
    	SEND_MSG(USER_ERR_STR);
    	break;
    case CONFIG_ERR:
    	SEND_MSG(CONFIG_ERR_STR);
    	break;
    case SEND_MSG_ERR:
    	SEND_MSG(SEND_MSG_ERR_STR);
    	break;
    case ACCOUNT_EXIST_ERR:
    	SEND_MSG(ACCOUNT_EXIST_ERR_STR);
    	break;
    case REGISTER_ERR:
    	SEND_MSG(REGISTER_ERR_STR);
    	break;
    case UPDATE_NICKNAME_ERR:
    	SEND_MSG(UPDATE_NICKNAME_ERR_STR);
    	break;
    case UPDATE_INDIV_ERR:
    	SEND_MSG(UPDATE_INDIV_ERR_STR);
    	break;
   case QUERY_ONLINE_FRIEND_ERR:
    	SEND_MSG(QUERY_ONLINE_FRIEND_ERR_STR);
    	break;
   case GET_FRIEND_OF_USER_ERR:
    	SEND_MSG(GET_FRIEND_OF_USER_ERR_STR);
    	break;	
   case ADD_NEW_FRIEND_ERR:
    	SEND_MSG(ADD_NEW_FRIEND_ERR_STR);
    	break;	
   case DELIVER_MSG_ERR:
    	SEND_MSG(DELIVER_MSG_ERR_STR);
    	break;	
    default:
    	break;
	}
	return ret;	
}

/*向客户端回发消息*/
static int sendSelf(ServerHandle *handle, int sockfd, const char *str, int str_len, int id)
{
	char buf[BUF_SIZE] = {0};
	MsgControl msg;
	memset(&msg, 0, sizeof(MsgControl));
	msg.client_request = CLIENT_SUCCESS;
	msg.server_reply   = MSG_SEND;
	msg.head_1 = id;
	msg.head_2 = id;
	memcpy(buf, (const void *)&msg, DATAGRAM_MSG);
	memcpy(buf + DATAGRAM_MSG, (const void *)str, str_len);
	return sendDatagram(handle, sockfd, (const char *)buf, str_len + DATAGRAM_MSG);
}

/*获取所有在线好友的ID*/
static int getAllOnlineFriend(const void *arg, int *count, int **array)
{
	int ret = 0;
	char **result = NULL;
	int row, column, i, index;
	/*在获取新的所有好友信息前,先清空原先信息*/
	IPC_SemP_2(g_sem_id);
	/*row 查询结果的行数,column 查询结果的列数*/
	ret = selectOnlineFriend(g_id, &result, &row, &column);
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE) {
		feedBackMsg(arg, QUERY_ONLINE_FRIEND, CLIENT_FAILURE, QUERY_ONLINE_FRIEND_ERR);
		return ret;
	}
	int *id_array = (int *)malloc(sizeof(int) *row);
	memset(id_array, 0, sizeof(int) *row);
	index = column;
	for (i = 0; i < row; i++)
	{
		/*转换字符串获取ID*/
		id_array[i] = atoi(result[index]);
		index += column;
		DEBUG_WORK;
	}
	/*统计好友个数*/
	*count = row;
	/*主调函数定义空指针,函数内分配内存*/
	*array = id_array;
	return WORK_SUCCESS;
}

/*获取与本用户相关的好友ID*/
static int getFriendOfUser(const void *arg, int *count, int **array)
{
	int ret = 0;
	char **result = NULL;
	int row, column, i, index;
	/*在获取新的所有好友信息前,先清空原先信息*/
	IPC_SemP_2(g_sem_id);
	/*row 查询结果的行数,column 查询结果的列数*/
	ret = getFriendsId(g_id, &result, &row, &column);
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE) {
		feedBackMsg(arg, QUERY_ONLINE_FRIEND, CLIENT_FAILURE, GET_FRIEND_OF_USER_ERR);
		return ret;
	}
	int *id_array = (int *)malloc(sizeof(int) *row);
	memset(id_array, 0, sizeof(int) *row);
	index = column;
	for (i = 0; i < row; i++)
	{
		/*转换字符串获取ID*/
		id_array[i] = atoi(result[index]);
		index += column;
	}
	/*统计好友个数*/
	*count = row;
	/*主调函数定义空指针,函数内分配内存*/
	*array = id_array;
	return WORK_SUCCESS;
}

/*发送在线好友信息*/
static int sendOnlineFriend(const void *arg, UserInfo *user)
{
	char buf[BUF_SIZE] = {0};
	MsgControl msg;
	memset(&msg, 0, sizeof(MsgControl));
	msg.client_request = CLIENT_SUCCESS;
	msg.server_reply   = QUERY_ONLINE_FRIEND;
	memcpy(buf, (const void *)&msg, DATAGRAM_MSG);
	memcpy(buf + DATAGRAM_MSG, (const void *)user, DATAGRAM_USER);
	g_pa = (Parameter *)arg;
	return sendDatagram(g_pa->handle, g_pa->sockfd, (const char *)buf, DATAGRAM_MSG + DATAGRAM_USER);
}

/************************************************************************************************************/

/*刷新用户信息,成功返回0,失败返回-1*/
static int refreshUserInfo(const void *arg, UserInfo *user)
{
	char buf[BUF_SIZE] = {0};
	MsgControl msg;
	memset(&msg, 0, sizeof(MsgControl));
	msg.client_request = CLIENT_SUCCESS;
	msg.server_reply   = REFRESH_USER_INFO;
	memcpy(buf, (const void *)&msg, DATAGRAM_MSG);
	memcpy(buf + DATAGRAM_MSG, (const void *)user, DATAGRAM_USER);
	g_pa = (Parameter *)arg;
	return sendDatagram(g_pa->handle, g_pa->sockfd, (const char *)buf, DATAGRAM_MSG + DATAGRAM_USER);
}

/*登录处理流程*/
static int dealLogin(const void *arg, UserInfo *user)
{
	int ret = WORK_SUCCESS;
	printf("%s, login\n", __FUNCTION__);
	g_pa = (Parameter *)arg;
	AccountPasswd *ap = (AccountPasswd *)((char *)g_pa->data + DATAGRAM_MSG);
	printf("account: %s, passwd: %s\n", ap->account, ap->passwd);
	IPC_SemP_2(g_sem_id);
	/*判断账号是否存在,若存在则返回对应用户ID,否则返回错误码*/
	ret = getUserId(ap->account);
	IPC_SemV_2(g_sem_id);
	DEBUG_WORK;
	if (ret == SQLITE_FAILURE) {
		feedBackMsg(arg, LOGIN, CLIENT_FAILURE, ACCOUNT_ERR);
		return ret;
	}
	printf("id: %d\n", ret);
	/*设置该客户端的唯一全局ID*/
	g_id = ret;
	user->id = ret;
	IPC_SemP_2(g_sem_id);
	/*判断账号密码是否匹配*/
	ret = isExistUser(ap->account, ap->passwd);
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE) {
		feedBackMsg(arg, LOGIN, CLIENT_FAILURE, AP_ERR);
		return ret;
	}
	IPC_SemP_2(g_sem_id);
	ret = getUserStatus(g_id);
	IPC_SemV_2(g_sem_id);
	if (ret == ONLINE) {
		feedBackMsg(arg, LOGIN, CLIENT_FAILURE, USER_ONLINE_ERR);
		return ret;
	}
	IPC_SemP_2(g_sem_id);
	/*账号密码匹配成功,设置登录状态为在线状态*/
	ret = setUserStatus(user->id, UPDATE_STATUS);
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE) {
		feedBackMsg(arg, LOGIN, CLIENT_FAILURE, STATUS_ERR);
		return ret;
	}
	/*载入用户信息*/
	ret = loadUserInfo(user);
	if (ret == WORK_FAILURE) {
		feedBackMsg(arg, LOGIN, CLIENT_FAILURE, LOAD_ERR);
		return ret;
	}
	printf("%s-------------------------------------begin\n", __FUNCTION__);
	printf("name: %s\n", user->name);
	printf("individual: %s\n", user->indiv);
	printf("regis: %s\n", user->regis);
	printf("last: %s\n", user->last);
	printf("status: %d\n", user->status);
	printf("%s-------------------------------------end\n", __FUNCTION__);
	/*向客户端发送刷新用户信息消息*/
	ret = refreshUserInfo(arg, user);
	if (ret == WORK_FAILURE) {
		feedBackMsg(arg, LOGIN, CLIENT_FAILURE, USER_ERR);
		return ret;
	}
	/*执行登录成功后创建线程*/
	pthread_t tid;
	ret = pthread_create(&tid, NULL, Start, (void *)arg);
	if (ret != 0) {
		fprintf(stderr, "%s, pthread_create() failed, %s\n", __FUNCTION__, strerror(ret));
		feedBackMsg(arg, LOGIN, CLIENT_FAILURE, CONFIG_ERR);
  	exit(EXIT_FAILURE);
	}
	return feedBackMsg(arg, LOGIN, CLIENT_SUCCESS, INVALID_ARG);
}

/*注册处理流程*/
static int dealRegister(const void *arg)
{
	int ret = WORK_SUCCESS;
	printf("%s, register\n", __FUNCTION__);
	g_pa = (Parameter *)arg;
	AccountPasswd *ap = (AccountPasswd *)((char *)g_pa->data + DATAGRAM_MSG);
	printf("account: %s, passwd: %s\n", ap->account, ap->passwd);
	IPC_SemP_2(g_sem_id);
	/*判断账号是否存在,若存在则注册失败*/
	ret = getUserId(ap->account);
	IPC_SemV_2(g_sem_id);
	if (ret != SQLITE_FAILURE) {
		feedBackMsg(arg, REGISTER, CLIENT_FAILURE, ACCOUNT_EXIST_ERR);
		return ret;
	}
	IPC_SemP_2(g_sem_id);
	ret = insertNewUser(ap->account, ap->passwd, DEFAULT_NICKNAME, DEFAULT_INDIV, getCurTime());
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE) {
		feedBackMsg(arg, REGISTER, CLIENT_FAILURE, REGISTER_ERR);
		return ret;
	}
	return feedBackMsg(arg, REGISTER, CLIENT_SUCCESS, INVALID_ARG);
}

/*注销处理流程*/
static int dealLogout(const void *arg, UserInfo *user)
{
	//TODO
	return 0;
}

/*刷新好友信息,成功返回0,失败返回-1*/
static int refreshFriendInfo(const void *arg, SeqList *list)
{
	printf("%s, refreshFriendInfo\n", __FUNCTION__);
	char buf[BUF_SIZE] = {0};
	g_pa = (Parameter *)arg;
	MsgControl msg;
	UserInfo *tmp = NULL;
	int i;
	getFriendInfo(list, &g_count);
	memset(&msg, 0, sizeof(MsgControl));
	msg.client_request = CLIENT_SUCCESS;
	msg.server_reply   = REFRESH_FRIEND_INFO;
	for (i = 0; i < list->length(); i ++) {
		tmp = (UserInfo *)list->get(i);
		memset(buf, 0, BUF_SIZE);
		memcpy(buf, (const void *)&msg, DATAGRAM_MSG);
		memcpy(buf + DATAGRAM_MSG, (const void *)tmp, DATAGRAM_USER);
		sendDatagram(g_pa->handle, g_pa->sockfd, (const char *)buf, DATAGRAM_MSG + DATAGRAM_USER);
		printf("%s, send name: %s\n", __FUNCTION__, tmp->name);
	}
	return WORK_SUCCESS;
}

/*消息发送处理流程*/
static int dealMsgSend(const void *arg, int id)
{
	printf("%s, dealMsgSend\n", __FUNCTION__);
	char buf[BUF_SIZE] = {0};
	g_pa = (Parameter *)arg;
	char *tmp = (char *)g_pa->data + DATAGRAM_MSG;
	printf("send msg: %s, size: %d\n", tmp, g_pa->length - DATAGRAM_MSG);
	/*若接收者的ID为该客户端ID,则转发数据给自身*/
	if (id == g_id) {
		sendSelf(g_pa->handle, g_pa->sockfd, tmp, g_pa->length - DATAGRAM_MSG, id);
	}
	else {
		/*将该客户端的ID载入到消息队列数据报的包头*/
		memcpy(buf, (const void *)&g_id, sizeof(int));
		memcpy(buf + sizeof(int), (const void *)tmp, g_pa->length - DATAGRAM_MSG);
		if (IPC_MsgSend(g_msg_id, (const void *)buf, sizeof(int) + g_pa->length - DATAGRAM_MSG, id) == MSG_FAILURE) {
			fprintf(stderr, "%s, IPC_MsgSend() failed\n", __FUNCTION__);
			feedBackMsg(arg, MSG_SEND, CLIENT_FAILURE, SEND_MSG_ERR);
			return WORK_FAILURE;
		}
	}
	return WORK_SUCCESS;
}

/*修改用户信息流程*/
static int dealUpdateUserInfo(const void *arg)
{
	int ret = WORK_SUCCESS;
	char buf[BUF_SIZE] = {0};
	g_pa = (Parameter *)arg;
	UserInfo *user = (UserInfo *)((char *)g_pa->data + DATAGRAM_MSG);
	IPC_SemP_2(g_sem_id);
	ret = setUserInfo(g_id, (const char *)user->name, UPDATE_NICKNAME);
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE) {
		feedBackMsg(arg, UPDATE_USER_INFO, CLIENT_FAILURE, UPDATE_NICKNAME_ERR);
		return ret;
	}
	IPC_SemP_2(g_sem_id);
	ret = setUserInfo(g_id, (const char *)user->indiv, UPDATE_INDIV);
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE) {
		feedBackMsg(arg, UPDATE_USER_INFO, CLIENT_FAILURE, UPDATE_INDIV_ERR);
		return ret;
	}
	MsgControl msg;
	memset(&msg, 0, DATAGRAM_MSG);
	msg.client_request = CLIENT_SUCCESS;
	msg.server_reply   = UPDATE_USER_INFO;
	memset(buf, 0, BUF_SIZE);
	memcpy(buf, (const void *)&msg, DATAGRAM_MSG);
	memcpy(buf + DATAGRAM_MSG, (const void *)user, DATAGRAM_USER);
	return sendDatagram(g_pa->handle, g_pa->sockfd, (const void *)buf, DATAGRAM_USER + DATAGRAM_MSG);
}

/*删除用户信息流程*/
static int dealDeleteUserInfo(const void *arg)
{
	int ret = WORK_SUCCESS;
	char buf[BUF_SIZE] = {0};
	g_pa = (Parameter *)arg;
	IPC_SemP_2(g_sem_id);
	ret = deleteUser(g_id, DELETE_USER);
	IPC_SemV_2(g_sem_id);
	if (ret == SQLITE_FAILURE) {
		feedBackMsg(arg, DELETE_USER_INFO, CLIENT_FAILURE, INVALID_ARG);
		return ret;
	}
	IPC_SemP_2(g_sem_id);
	deleteUser(g_id, DELETE_FRIEND);
	IPC_SemV_2(g_sem_id);
	MsgControl msg;
	memset(buf, 0, BUF_SIZE);
	memset(&msg, 0, DATAGRAM_MSG);
	msg.client_request = CLIENT_SUCCESS;
	msg.server_reply   = DELETE_USER_INFO;
	memcpy(buf, (const void *)&msg, DATAGRAM_MSG);
	return sendDatagram(g_pa->handle, g_pa->sockfd, (const void *)buf, DATAGRAM_MSG);
}

/*查询在线好友*/
static int dealQueryFriend(const void *arg)
{
	int *o_array = NULL, *u_array = NULL;
	int o_count, u_count, i, j;
	UserInfo user;
	/*查询在线好友id*/
	if (getAllOnlineFriend(arg, &o_count, &o_array) != WORK_SUCCESS) {
		return WORK_FAILURE;
	}
	/*查询用户的好友id*/
	if (getFriendOfUser(arg, &u_count, &u_array) != WORK_SUCCESS) {
		return WORK_FAILURE;
	}
	
#if 0
	DEBUG_WORK;
	for (i = 0; i < u_count; i++) {
		printf("u_array[%d]: %d\n", i, u_array[i]);
	}
		for (i = 0; i < o_count; i++) {
		printf("o_array[%d]: %d\n", i, o_array[i]);
	}
	DEBUG_WORK;
#endif

	/*在在线好友id集合中去掉用户的在线好友id*/
	for (i = 0; i < u_count; i++) {
		for (j = 0; j < o_count; j++) {
			if (u_array[i] == o_array[j]) {
				o_array[j] = -1;
			}
		}
	}
	for (i = 0; i < o_count; i++) {
		if (o_array[i] != -1) {
			memset(&user, 0, DATAGRAM_USER);
			user.id = o_array[i];
			printf("array[%d]: %d\n", i, o_array[i]);
			loadUserInfo(&user);
			sendOnlineFriend(arg, &user);
		}
	}
	free(o_array);
	free(u_array);
	return WORK_SUCCESS;
}

/*添加好友处理流程*/
static int dealRequestAddFriend(const void *arg, int new_id, int id, int flg)
{
	int ret = WORK_SUCCESS;
	char buf[BUF_SIZE] = {0};
	int head = -1;
	g_pa = (Parameter *)arg;
	MsgControl msg;
	UserInfo user;
	memset(buf, 0, BUF_SIZE);
	memset(&msg, 0, DATAGRAM_MSG);
	memset(&user, 0, DATAGRAM_USER);
	DEBUG_WORK;
	user.id = g_id;
	loadUserInfo(&user);
	msg.client_request = flg;
	msg.server_reply = REQUEST_ADD_FRIEND;
	msg.head_1 = new_id;
	msg.head_2 = id;
	/*同意之后,将好友添加到数据库中*/
	if (flg == AGREEMENT) {
		DEBUG_WORK;
		IPC_SemP_2(g_sem_id);
		ret = insertNewFriend(id, new_id);
		IPC_SemV_2(g_sem_id);
		if (ret == SQLITE_FAILURE) {
			feedBackMsg(arg, REQUEST_ADD_FRIEND, CLIENT_FAILURE, ADD_NEW_FRIEND_ERR);
			return ret;
		}
		IPC_SemP_2(g_sem_id);
		ret = insertNewFriend(new_id, id);
		IPC_SemV_2(g_sem_id);
		if (ret == SQLITE_FAILURE) {
			feedBackMsg(arg, REQUEST_ADD_FRIEND, CLIENT_FAILURE, ADD_NEW_FRIEND_ERR);
			return ret;
		}
		DEBUG_WORK;
	}
	else if (flg == REQUEST) {
		printf("%s, name: %s request add friend\n", __FUNCTION__, user.name);
	}
	/*将该客户端的ID载入到消息队列数据报的包头*/
	memcpy(buf, (const void *)&head, sizeof(int));
	memcpy(buf + sizeof(int), (const void *)&msg, DATAGRAM_MSG);
	memcpy(buf + DATAGRAM_MSG + sizeof(int), (const void *)&user, DATAGRAM_USER);
	if (IPC_MsgSend(g_msg_id, (const void *)buf, sizeof(int) + DATAGRAM_MSG + DATAGRAM_USER, id) == MSG_FAILURE) {
		fprintf(stderr, "%s, IPC_MsgSend() failed\n", __FUNCTION__);
		feedBackMsg(arg, REQUEST_ADD_FRIEND, CLIENT_FAILURE, DELIVER_MSG_ERR);
		return WORK_FAILURE;
	}
	return ret;
}

/************************************************************************************************************/

/*服务器接收消息处理流程*/
static void doWork(void *arg, UserInfo *user, SeqList *list)
{
	Parameter *p = (Parameter *)arg;
	MsgControl *msg = getMsgControl((void *)p->data);
	printf("%s, msg->client_request: %d\n", __FUNCTION__, msg->client_request);
	printf("%s, msg->server_reply: %d\n", __FUNCTION__, msg->server_reply);
	printf("%s, msg->head_1: %d\n", __FUNCTION__, msg->head_1);
	printf("%s, msg->head_2: %d\n", __FUNCTION__, msg->head_2);
	switch (msg->client_request) {
		case LOGIN:
			dealLogin(arg, user);
			break;
		case REGISTER:
			dealRegister(arg);
			break;
		case LOGOUT:
			dealLogout(arg, user);
			break;
		case REFRESH_USER_INFO:
			refreshUserInfo(arg, user);
			break;
		case REFRESH_FRIEND_INFO:
			refreshFriendInfo(arg, list);
			printf("%s, friend count: %d\n", __FUNCTION__, g_count);
			break;
		case MSG_SEND:
			dealMsgSend(arg, msg->head_2);
			break;
		case UPDATE_USER_INFO:
			dealUpdateUserInfo(arg);
			break;
		case DELETE_USER_INFO:
			dealDeleteUserInfo(arg);
			break;
		case QUERY_ONLINE_FRIEND:
			dealQueryFriend(arg);
			break;
		case REQUEST_ADD_FRIEND:
			dealRequestAddFriend(arg, msg->head_1, msg->head_2, msg->server_reply);
			break;
		default:
			printf("%s default, msg->client_request: %d\n", __FUNCTION__, msg->client_request);
			break;
	}
	return ;
}

/*接收客户端消息主框架*/
void doClient(void *handle, int sockfd, const char *ip)
{
	int ret = 0;
	printf("client begin, ip: %s\n", ip);
	if (installSignal(SIGRTMIN, handler) == SIGNAL_FAILURE) {
		fprintf(stderr, "%s, installSignal() failed\n", __FUNCTION__);
  	exit(EXIT_FAILURE);
	}
	Datagram data[DATA_SIZE];
  int length;
  Parameter *parameter = NULL;
  parameter = (Parameter *)malloc(sizeof(Parameter));
  if (NULL == parameter) {
  	fprintf(stderr, "%s, parameter, malloc failed\n", __FUNCTION__);
  	exit(EXIT_FAILURE);
  }
  UserInfo *user_info = NULL;
  user_info = (UserInfo *)malloc(sizeof(UserInfo));
  if (NULL == user_info) {
  	fprintf(stderr, "%s, user_info, malloc failed\n", __FUNCTION__);
  	exit(EXIT_FAILURE);
  }
  /*顺序表对象的构造*/
  SeqList *list = SeqList_Construction(SEQLIST_SIZE);
  if (NULL == list) {
  	fprintf(stderr, "%s, SeqList_Construction() failed\n", __FUNCTION__);
  	exit(EXIT_FAILURE);
  }
  if (IPC_MsgCreate(IPC_FILE, &g_msg_id) == MSG_FAILURE) {
  	fprintf(stderr, "%s, IPC_MsgCreate() failed\n", __FUNCTION__);
  	exit(EXIT_FAILURE);
  }
  printf("msgid: %d\n", g_msg_id);
  if (IPC_SemCreate_2(IPC_FILE, &g_sem_id) == SEM_FAILURE) {
  	fprintf(stderr, "%s, IPC_SemCreate() failed\n", __FUNCTION__);
  	exit(EXIT_FAILURE);
  }
  printf("sem_id: %d\n", g_sem_id);
  while (1) {
  	memset(data, 0, DATA_SIZE);
    ret = SocketServer_recv(handle, sockfd, data, &length);
    if (ret != 0) {
      if (ret == SOCK_ERR_TIMEOUT) {
        fprintf(stdout, "recv timeout\n");
        continue;
      }
      fprintf(stderr, "SocketServer_recv() failed\n");
      break;
    }
    memset(parameter, 0, sizeof(Parameter));
    parameter->handle = handle;
    parameter->sockfd = sockfd;
    parameter->data = data;
    parameter->length = length;
    doWork((void *)parameter, user_info, list);
  }
  //IPC_MsgDelete(g_msg_id);
  //IPC_SemDelete(g_sem_id);
	clearFriendList(list);
  /*顺序表的析构*/
  SeqList_Destruction(list);
  if (user_info != NULL) {
  	free(user_info);
  }
  if (parameter != NULL) {
  	free(parameter);
  }
  if (g_id != 0) {
  	IPC_SemP_2(g_sem_id);
  	/*更新最后一次的登录时间*/
  	setUserInfo(g_id, getCurTime(), UPDATE_LAST);
  	/*断开连接前重置登录状态*/
  	setUserStatus(g_id, RESET_STATUS);
  	IPC_SemV_2(g_sem_id);
  }
  /*关闭数据库*/
  closeMysqlite();
  /*关闭客户端socket*/
  SocketServer_closeClient(sockfd);
  /*销毁服务器socket对象句柄*/
  SocketServer_destroy(&handle);
  printf("client end, ip: %s\n", ip);
  exit(EXIT_SUCCESS);
}
