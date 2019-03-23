#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "msgqueue.h"

#define TEXT_SIZE 2048
#define PROJ_ID   'm'
//#define DEBUG_MSG fprintf(stdout, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__)

/*定义消息队列通信协议*/
struct msg_buf {
	long type;    			    /*消息类型*/
	char text[TEXT_SIZE];   /*消息内容*/
};

int IPC_MsgCreate(const char *ipc_path, int *msg_id)
{
	/*定义IPC对象的键值*/
	key_t key;
	/*定义消息队列ID号*/
	int id;
	key = ftok(ipc_path, PROJ_ID);
	if (key == -1) {
		fprintf(stderr, "ftok failed, %s\n", strerror(errno));
		return MSG_FAILURE;
	}
	printf("key: %d, key: %#x\n", key, key);
	/*若不存在则创建，并返回id，若存在则直接返回对应id，失败返回-1*/
	id = msgget(key, IPC_CREAT | 0666);
	if (id == -1) {
		fprintf(stderr, "msgget failed, %s\n", strerror(errno));
		return MSG_FAILURE;
	}
	*msg_id = id;
	//DEBUG_MSG;
	return MSG_SUCCESS;
}

int IPC_MsgSend(int msg_id, const void *msg_data, int msg_size, int msg_type)
{
	int ret = 0;
	/*定义通信消息结构变量*/
	struct msg_buf send_msg;
	memset(&send_msg, 0, sizeof(struct msg_buf));
	/*设置发送消息类型*/
	send_msg.type = msg_type;
	if (msg_size > TEXT_SIZE) {
		fprintf(stderr, "msg size argument %d greater than 2048\n", msg_size);
		return MSG_FAILURE;
	}
	memcpy(send_msg.text, msg_data, msg_size);
	/* @msgflg  				 0:阻塞的方式发送消息 
   * 					IPC_NOWAIT:非阻塞发送消息(当消息队列中没有可用空间时，
	 * 若不阻塞，立即返回一个错误码EAGAIN) 
	 */
	ret = msgsnd(msg_id, (const void *)&send_msg, msg_size, 0);
	if (ret == -1) {
		fprintf(stderr, "msgsnd failed, %s, %d\n", strerror(errno), errno);
		return MSG_FAILURE;
	}
	/* msgsnd failed, Invalid argument, 22
	 * msgsnd failed, Identifier removed, 43
	 */
	return ret;
}

int IPC_MsgRecv(int msg_id, void *msg_data, int *msg_size, int msg_type)
{
	int ret = 0;
	/*定义通信消息结构变量*/
	struct msg_buf recv_msg;
	long type = msg_type;
	memset(&recv_msg, 0, sizeof(struct msg_buf));
	/*设置发送消息类型*/
	ret = msgrcv(msg_id, (void *)&recv_msg, TEXT_SIZE, type, 0);
	if (ret == -1) {
		fprintf(stderr, "msgrcv failed, %s\n", strerror(errno));
		return MSG_FAILURE;
	}
	memcpy(msg_data, recv_msg.text, ret);
	*msg_size = ret;
	return ret;
}

int IPC_MsgDelete(int msg_id)
{
	if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
		fprintf(stderr, "msgctl failed, %s\n", strerror(errno));
		return MSG_FAILURE;
	}
	//DEBUG_MSG;
	return MSG_SUCCESS;
}