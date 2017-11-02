#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    /* Create the shm */
    key_t key;
    int shmid;
    char *data;

    key = ftok("/", 'R');
    shmid = shmget(key, 1024, IPC_CREAT | 0644);
    data = shmat(shmid, (void *) 0, 0);
    memset(data, '\0', 1024);


    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
