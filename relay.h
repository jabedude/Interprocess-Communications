#ifndef _RELAY_H
#define _RELAY_H

#include <termios.h>

#define SHM_SIZE    1
#define BUF_SIZE    128
#define SLEEP_TM    50000

void unbuf_term(struct termios *, struct termios *);

#endif
