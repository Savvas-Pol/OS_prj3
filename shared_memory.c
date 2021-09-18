#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "shared_memory.h"

// case 1:

// G->M: <VP_START,fakeid, size, size, duration>
// M->G: 
//     <STARTED,0, 0, 0, 0> 
//     <FAIL,fakeid, 0, 0, 0>


// case 1:

// G->M: <VP_STOP,fakeid, size, size, duration>
// M->G: 
//     <SUCCESS,fakeid, size, size, duration> 
//     <STARTED,fakeid, size, size, duration>

typedef struct Memory {
    // from G to M
    char command[20]; // VP_START or VP_STOP
    int fakeid;
    int size;
    int duration;
} Memory;

union senum {
    int val;
    struct semid_ds *buff;
    unsigned short *array;
};

#define SEM_G 0
#define SEM_M 1

static Memory * pmem;
static int shmid;
static int semid;

static void down(int s) {
    struct sembuf semopr = { 0 };
    semopr.sem_num = s;
    semopr.sem_op = -1;
    semopr.sem_flg = 0;
    if (semop(semid, &semopr, 1) < 0) {
        perror("semop");
        exit(1);
    }
}

static void up(int s) {
    struct sembuf semopr = { 0 };
    semopr.sem_num = s;
    semopr.sem_op = +1;
    semopr.sem_flg = 0;
    if (semop(semid, &semopr, 1) < 0) {
        perror("semop");
        exit(1);
    }
}

void shared_memory_init() {
    shmid = shmget(IPC_PRIVATE, sizeof (Memory), 0666 | IPC_CREAT);
    semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0660);
    
    if (shmid == 0) {
        perror("shared_memory_init - shmget failed \n");
        exit(1);
    }
    
    if (semid == 0) {
        perror("shared_memory_init - semget failed \n");
        exit(1);
    }

    union senum arg;
    arg.val = 1;
    semctl(semid, SEM_G, SETVAL, arg);

    arg.val = 0;
    semctl(semid, SEM_M, SETVAL, arg);
}

void shared_memory_attach() {
    pmem = (Memory*) shmat(shmid, 0, 0);        
}

void shared_memory_detach() {
    shmdt((char*) pmem);
}

void shared_memory_destroy() {
    shmctl(shmid, 0, IPC_RMID);
    semctl(semid, 0, IPC_RMID, 0);
}

void shared_memory_g_place(char command[20], int fakeid, int size, int duration) {
    strcpy(pmem->command, command);
    pmem->fakeid = fakeid;
    pmem->size = size;
    pmem->duration = duration;
    up(SEM_M);
}

void shared_memory_m_place(char command[20], int fakeid, int size, int duration) {
    strcpy(pmem->command, command);
    pmem->fakeid = fakeid;
    pmem->size = size;
    pmem->duration = duration;
    up(SEM_G);
}

void shared_memory_g_obtain(char command[20], int *fakeid, int *size, int * duration) {
    down(SEM_G);
    strcpy(command, pmem->command);
    *fakeid = pmem->fakeid;
    *size = pmem->size;
    *duration = pmem->duration;
}

void shared_memory_m_obtain(char command[20], int *fakeid, int *size, int * duration) {
    down(SEM_M);
    strcpy(command, pmem->command);
    *fakeid = pmem->fakeid;
    *size = pmem->size;
    *duration = pmem->duration;
}