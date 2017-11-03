#include <stdlib.h>

#include "relay.h"

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

    // Set up terminal settings
    struct termios saved_termios, new_termios;
    unbuf_term(&saved_termios, &new_termios);

    // Print data
    while (1) {
        usleep(SLEEP_TM);

        // Check for EOT
        if (*data == 0x04) {
            break;
        } else if (*data) {
            printf("%c", *data);
            fflush(stdout);
        }
    }

    // Cleanup
    putchar('\n');
    tcsetattr(0, TCSANOW, &saved_termios);
    shmdt(data);

    return 0;
}
