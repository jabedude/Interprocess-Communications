#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

union semun {
    int val;
    struct semid_ds *buf;
    short *array;
};

int main(void)
{
    /* Get access to semaphore set */
    // key_t semkey;
    // int semid;
    // union semun arg;
    // semkey = ftok("/", 'E');
    // semid = semget(semkey, 1, 0);

    // Get shm
    key_t key;
    int shmid;
    char *data;

    key = ftok("/", 'R');
    shmid = shmget(key, 1024, IPC_EXCL | 0644);
    if (shmid < 0) {
        perror("shmget");
        fprintf(stderr, "Please start the dispatcher then try again.\n");
        exit(-1);
    }
    data = shmat(shmid, (void *) 0, 0);

    // Print data
    char *buff = malloc(1024 * sizeof(char));
    while (1) {
        memcpy(buff, data, 1024);
        usleep(5000);

        // Check for EOT
        if (buff[0] == 0x04) {
            break;
        } else if (*buff)
            printf("%s\n", buff);
    }

    // Cleanup
    free(buff);
    shmdt(data);
    // shmctl(shmid, IPC_RMID, NULL);
    // semctl(semid, 0, IPC_RMID, arg);

    return 0;
}
