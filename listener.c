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
#include <termios.h>

#include "relay.h"

union semun {
    int val;
    struct semid_ds *buf;
    short *array;
};

int main(void)
{
    // Get shm
    key_t key;
    int shmid;
    char *data;

    key = ftok("/", 'R');
    shmid = shmget(key, SHM_SIZE, IPC_EXCL | 0644);
    if (shmid < 0) {
        perror("shmget");
        fprintf(stderr, "Please start the dispatcher then try again.\n");
        exit(-1);
    }
    data = shmat(shmid, (void *) 0, 0);

    struct termios saved_termios, new_termios;
    tcgetattr(0, &saved_termios);
    new_termios = saved_termios;
    new_termios.c_lflag &= ~(ICANON);
    new_termios.c_lflag &= ~(ECHO);
    tcflush(0, TCIFLUSH);
    tcsetattr(0, TCSANOW, &new_termios);

    // Print data
    int c;
    while (1) {
        c = *data;
        usleep(SLEEP_TM);

        // Check for EOT
        if (c == 0x04) {
            break;
        } else if (c) {
            //printf("%c\n", buff);
            printf("%c", c);
            fflush(stdout);
        }
    }

    // Cleanup
    putchar('\n');
    tcsetattr(0, TCSANOW, &saved_termios);
    shmdt(data);

    return 0;
}
