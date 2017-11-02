#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

int main(void)
{
    /* Create semaphore set */
    // key_t semkey;
    // int semid;
    // semkey = ftok("/", 'E');
    // semid = semget(semkey, 1, IPC_CREAT | 0644);
    // semctl(semkey, 1, SETALL, 0);

    /* Create the shm */
    key_t key;
    int shmid;
    char *data;
    key = ftok("/", 'R');
    shmid = shmget(key, 1024, IPC_CREAT | 0644);
    data = shmat(shmid, (void *) 0, 0);
    memset(data, '\0', 1024);

    struct termios old_term, new_term;

    tcgetattr(0, &old_term); //save current port settings
    new_term = old_term;
    new_term.c_lflag &= ~ICANON;
    new_term.c_lflag &= ~ECHO;
    tcflush(0, TCIFLUSH);
    tcsetattr(0, TCSANOW, &new_term);

    /* Get input */
    int c;
    short pos = 0;
    char buff[128];
    while (1) {
        c = getchar();
        if (c == 0x04) {
            data[0] = 0x04;
            break;
        } else {
            // TODO: overflow here
            putchar(c);
            buff[pos] = c;
            strcpy(data, buff);
            usleep(5000);
            memset(data, 0, 1024);
            continue;
        }
        pos++;
    }

    tcsetattr(0, TCSANOW, &old_term);
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
