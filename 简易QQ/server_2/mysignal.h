#ifndef __MYSIGNAL_H__
#define __MYSIGNAL_H__

#ifdef __cplusplus 
extern "C" {
#endif

#define SIGNAL_SUCCESS  0
#define SIGNAL_FAILURE -1

#define SIGRTMIN_T 34

int installSignal(int sig_num, void *signal_handler);

int sendSignal(int sig_num, void *arg);

#ifdef __cplusplus
}
#endif

#endif /*__MYSIGNAL_H__*/