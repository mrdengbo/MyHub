#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "semaphore.h"

//#define DEBUG_SEM fprintf(stdout, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__)

#define PROJ_ID 'm'
		
union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO                          (Linux specific) */
};

/*创建信号量集*/
int IPC_SemCreate(const char *ipc_path, int sem_count, int *sem_id)
{
	key_t key = ftok(ipc_path, PROJ_ID);
	if (key == -1) {
		fprintf(stderr, "ftok() failed, %s\n", strerror(errno));
		return SEM_FAILURE;
	}
	/*获取信号量集对象，若不存在就创建，存在则出错，并设置error为EEXIST*/
	int id = semget(key, sem_count, IPC_CREAT | IPC_EXCL | 0666);
	if (id == -1) {
		if (errno == EEXIST) {
			/*存在则直接获取信号量集ID*/
			id = semget(key, sem_count, IPC_CREAT | 0666);
		} 
		else {
			fprintf(stderr, "IPC_SemInit() failed, %s\n", strerror(errno));
			return SEM_FAILURE;
		}
	} 
	else {
		/*初始化读写两个信号量*/
		/*第二个参数表示信号量编号，第三个参数为设置的信号量初始值*/
		IPC_SemInit(id, SEM_WRITE, 0);
		IPC_SemInit(id, SEM_READ, 1);
	}
	*sem_id = id;
	return SEM_SUCCESS;
}

/*指定创建一个信号量的信号量集*/
int IPC_SemCreate_2(const char *ipc_path, int *sem_id)
{
	//DEBUG_SEM;
	key_t key = ftok(ipc_path, PROJ_ID);
	if (key == -1) {
		fprintf(stderr, "ftok() failed, %s\n", strerror(errno));
		return SEM_FAILURE;
	}
	/*获取信号量集对象，若不存在就创建，存在则出错，并设置error为EEXIST*/
	int id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if (id == -1) {
		if (errno == EEXIST) {
			//DEBUG_SEM;
			/*存在则直接获取信号量集ID*/
			id = semget(key, 1, IPC_CREAT | 0666);
		} 
		else {
			fprintf(stderr, "IPC_SemInit() failed, %s\n", strerror(errno));
			return SEM_FAILURE;
		}
	} 
	else {
		//DEBUG_SEM;
		/*初始化读写两个信号量*/
		/*第二个参数表示信号量编号，第三个参数为设置的信号量初始值*/
		IPC_SemInit(id, 0, 1);
	}
	//DEBUG_SEM;
	*sem_id = id;
	return SEM_SUCCESS;
}

/*初始化信号量集*/
int IPC_SemInit(int sem_id, int sem_num, int sem_val)
{
	union semun un;
	un.val = sem_val;
	//DEBUG_SEM;
	if (semctl(sem_id, sem_num, SETVAL, un) == -1) {
		fprintf(stderr, "IPC_SemInit() failed, %s\n", strerror(errno));
		return SEM_FAILURE;
	}
	return SEM_SUCCESS;
}

/*P操作（申请资源）*/
int IPC_SemP(int sem_id, int sem_num)
{
	//DEBUG_SEM;
	struct sembuf sops;
	/*信号量集的编号*/
	sops.sem_num = sem_num;
	/* -1 信号量申请资源操作*/
	sops.sem_op  = -1;
	/*0默认模式，条件不满足则阻塞*/
	sops.sem_flg = 0;
	/*第三个参数表示为操作信号量的个数*/
	if (semop(sem_id, &sops, 1) == -1) {
		fprintf(stderr, "P semop failed, %s\n", strerror(errno));
		return SEM_FAILURE;
	}
	//DEBUG_SEM;
	return SEM_SUCCESS;
}

/*只指定的信号量执行P申请资源操作*/
int IPC_SemP_2(int sem_id)
{
	return IPC_SemP(sem_id, 0);
}

/*V操作（释放资源）*/
int IPC_SemV(int sem_id, int sem_num)
{
	//DEBUG_SEM;
	struct sembuf sops;
	/*信号量集的编号*/
	sops.sem_num = sem_num;
	/* +1 信号量释放资源操作*/
	sops.sem_op  = 1;
	/*0默认模式，条件不满足则阻塞*/
	sops.sem_flg = 0;
	if (semop(sem_id, &sops, 1) == -1) {
		fprintf(stderr, "V semop failed, %s\n", strerror(errno));
		return SEM_FAILURE;
	}
	//DEBUG_SEM;
	return SEM_SUCCESS;
}

/*只指定的信号量执行V释放资源操作*/
int IPC_SemV_2(int sem_id)
{
	return IPC_SemV(sem_id, 0);
}

int IPC_SemDelete(int sem_id)
{
	if (semctl(sem_id, 0, IPC_RMID) < 0) {
		fprintf(stderr, "IPC_SemDelete() failed, %s\n", strerror(errno));
		return SEM_FAILURE;
	}
	return SEM_SUCCESS;
}


