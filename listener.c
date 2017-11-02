#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    /* Get shm */
    key_t key;
    int shmid;
    char *data;

    key = ftok("/", 'R');
    shmid = shmget(key, 1024, IPC_EXCL | 0644);
    data = shmat(shmid, (void *) 0, 0);

    /* Print data */
    char *buff = malloc(1024 * sizeof(char));
    while (1) {
        memcpy(buff, data, 1024);
        usleep(5000);
        if (*buff)
            printf("%s\n", buff);
    }

    free(buff);
    shmdt(data);
    return 0;
}
