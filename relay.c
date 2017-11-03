#include "relay.h"


void unbuf_term(struct termios *saved_termios, struct termios *new_termios)
{
    tcgetattr(0, saved_termios);
    new_termios = saved_termios;
    new_termios.c_lflag &= ~(ICANON);
    new_termios.c_lflag &= ~(ECHO);
    tcflush(0, TCIFLUSH);
    tcsetattr(0, TCSANOW, new_termios);
}
