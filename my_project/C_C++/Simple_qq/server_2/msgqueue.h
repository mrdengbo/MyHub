#ifndef __MSGQUEUE_H__
#define __MSGQUEUE_H__

#ifdef __cplusplus 
extern "C" {
#endif

#define MSG_SUCCESS  0
#define MSG_FAILURE -1

int IPC_MsgCreate(const char *ipc_path, int *msg_id);

int IPC_MsgSend(int msg_id, const void *msg_data, int msg_size, int msg_type);

int IPC_MsgRecv(int msg_id, void *msg_data, int *msg_size, int msg_type);

int IPC_MsgDelete(int msg_id);

#ifdef __cplusplus
}
#endif

#endif /*__MSGQUEUE_H__*/