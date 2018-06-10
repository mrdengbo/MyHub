#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "mysignal.h"

typedef void (*signalHandler)(int, siginfo_t *, void *);

/*注册信号*/
int installSignal(int sig_num, void *signal_handler)
{
  struct sigaction act;
  /*添加信号处理函数*/
  act.sa_sigaction = (signalHandler *)signal_handler;
  /*设置可以发送数据参数的标志位*/
  act.sa_flags = SA_SIGINFO;
  /*注册SIGRTMIN实时信号*/
  if (sigaction(sig_num, &act, NULL) == -1) {
    fprintf(stderr, "install SIGRTMIN failed, %s\n", strerror(errno));
    return SIGNAL_FAILURE;
  }
  return SIGNAL_SUCCESS;
}

int sendSignal(int sig_num, void *arg)
{
	/*定义要发送的数据参数*/
  sigval_t val;
  val.sival_ptr = arg;
  /*向本进程发送带参信号函数*/
  if (sigqueue(getpid(), sig_num, val) == -1) {
    fprintf(stderr, "sigqueue failed, %s\n", strerror(errno));
    return SIGNAL_FAILURE;
  }
  return SIGNAL_SUCCESS;
}

#if 0
  /*定义设置状态字的信号集变量*/
  //sigset_t bset;
  /*清空信号集*/
  //sigemptyset(&bset);
  /*把信号SIGINT添加到信号集中*/
  //sigaddset(&bset, SIGINT);
  /*把SIGINT指定信号设置为阻塞状态*/
  //sigprocmask(SIG_BLOCK, &bset, NULL);
  
  //sigemptyset(&bset);
  //sigaddset(&bset, SIGINT);
  /*解除阻塞*/
  //sigprocmask(SIG_UNBLOCK, &bset, NULL);
#endif