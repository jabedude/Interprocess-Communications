#include <string.h>

#include "relay.h"


void unbuf_term(struct termios *savt, struct termios *newt)
{
    tcgetattr(0, savt);
    // Deep copy termios
    memcpy(newt, savt, sizeof(struct termios));
    for (size_t i = 0; i < sizeof(newt->c_cc); i++) {
        newt->c_cc[i] = savt->c_cc[i];
    }
    newt->c_lflag &= ~(ICANON | ECHO);
    tcflush(0, TCIFLUSH);
    tcsetattr(0, TCSANOW, newt);
}
