#include <signal.h>
#include <stdbool.h>
#include "relay.h"

static void _int_hand(int _sig)
{
    _sig = _sig;
    signal(SIGINT, _int_hand);
}

int main(void)
{
    // Create the shm
    key_t key;
    int shmid;
    char *data;
    key = ftok("/", 'R');
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0644);
    data = shmat(shmid, (void *) 0, 0);
    *data = '\0';

    // Set termios settings
    struct termios saved_termios, new_termios;
    unbuf_term(&saved_termios, &new_termios);
    signal(SIGINT, _int_hand);

    // Get input
    int c;
    while (1) {
        c = getchar();
        if (c == 0x04) {
            *data = 0x04;
            break;
        } else {
            putchar(c);
            *data = c;
            usleep(SLEEP_TM);
            *data = '\0';
            continue;
        }
    }

    // Cleanup
    putchar('\n');
    tcsetattr(0, TCSANOW, &saved_termios);
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
