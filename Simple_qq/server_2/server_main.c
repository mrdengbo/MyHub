#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "socket_api.h"
#include "mysqlite.h"
#include "work.h"
#include "msgqueue.h"
#include "semaphore.h"

//#define SQLFILE "./qq.db"

#define handle_error(msg) \
  do {perror(msg); exit(EXIT_FAILURE); } while(0)
  
void recycle(int sig)
{
  pid_t pid = waitpid(-1, NULL, WNOHANG);
  if (pid > 0)
    printf("success recycle child process, pid is %d\n", pid);
  return ;
}

int main(int argc, const char *argv[])
{
	int ret = 0;
	if (argc < 4) {
        fprintf(stderr, "Usage : %s ip port dbfile\n", argv[0]);
        return -1;
  }
	/*子进程退出,处理回收僵尸进程*/
  if (signal(SIGCHLD, recycle) == SIG_ERR) {
  	ret = -1;
    fprintf(stderr, "SIGCHLD signal failed\n");
    return ret;
  }
  /*处理SIGPIPE的系统管道(socket文件缓冲区)破裂信号,防止程序异常终止*/
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
  	ret = -2;
    fprintf(stderr, "SIGPIPE signal failed\n");
    return ret;
  }
  openMysqlite(argv[3]);
	if (clearUsersStatus() == SQLITE_FAILURE) {
		ret = -3;
    fprintf(stderr, "clearUsersStatus() failed\n");
    return ret;
	}	  
	int msg_id, sem_id;
	if (IPC_MsgCreate(IPC_FILE, &msg_id) == MSG_FAILURE) {
  	fprintf(stderr, "%s, IPC_MsgCreate() failed\n", __FUNCTION__);
  	exit(EXIT_FAILURE);
  }
  if (IPC_SemCreate_2(IPC_FILE, &sem_id) == SEM_FAILURE) {
  	fprintf(stderr, "%s, IPC_SemCreate() failed\n", __FUNCTION__);
  	exit(EXIT_FAILURE);
  }
  IPC_MsgDelete(msg_id);
  IPC_SemDelete(sem_id);
  ServerHandle *handle = NULL;
  int sockfd;
  char client_ip[32] = {0};
  printf("server begin\n");
  //ret = SocketServer_init(&handle, 30, 0, 0, 8181, "127.0.0.1");
  //ret = SocketServer_init(&handle, 30, 0, 0, 8181, "192.168.1.183");
  ret = SocketServer_init(&handle, 0, 0, 0, atoi(argv[2]), argv[1]);
  if (ret != 0) {
    printf("SocketServer_init() failed\n");
    return ret;
  }
  pid_t pid;
  while (1) {
    ret = SocketServer_accept(handle, client_ip, sizeof(client_ip), &sockfd);
    if (ret != 0) {
      if (ret == SOCK_ERR_TIMEOUT) {
        fprintf(stdout, "accept timeout\n");
        break;
      }
      fprintf(stderr, "SocketServer_accept() failed\n");
      break;
    }
    fprintf(stdout, "welcome client, ip: %s\n", client_ip);
    pid = fork();
    if (pid < 0)
      handle_error("fork failed\n");
    else if (pid == 0) { /*child process*/
    	/*子进程关闭监听套接字*/
      SocketServer_closeListen(handle);
      doClient(handle, sockfd, client_ip);
    } /*if (pid == 0)*/
    else if (pid > 0) {
    	/*父进程关闭对方连接套接字*/
    	SocketServer_closeClient(sockfd);
  	}
  }
  SocketServer_destroy(&handle);
  closeMysqlite();
  printf("server end\n");
  return 0;
}
