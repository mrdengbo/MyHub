#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#ifdef __cplusplus 
extern "C" {
#endif

#define SEM_SUCCESS  0
#define SEM_FAILURE -1

#define SEM_WRITE 0 /*读资源*/
#define SEM_READ  1 /*写资源*/

int IPC_SemCreate(const char *ipc_path, int sem_count, int *sem_id);

int IPC_SemCreate_2(const char *ipc_path, int *sem_id);

int IPC_SemInit(int sem_id, int sem_num, int sem_val);

int IPC_SemP(int sem_id, int sem_num);

int IPC_SemP_2(int sem_id);

int IPC_SemV(int sem_id, int sem_num);

int IPC_SemV_2(int sem_id);

int IPC_SemDelete(int sem_id);

#ifdef __cplusplus
}
#endif

#endif /*__SEMAPHORE_H__*/