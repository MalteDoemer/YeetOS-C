#ifndef TTY_H
#define TTY_H

#include "stdint.h"
#include "stddef.h"

// must be a power of two
#define TTY_BUF_SIZE 1024

#define TTY_SIZE(q) (((q).head - (q).tail) & (TTY_BUF_SIZE - 1))
#define TTY_INC(a) ((a) = ((a) + 1) & (TTY_BUF_SIZE - 1))
#define TTY_PUTCH(q, c) ({(q).buffer[(q).head] = c; TTY_INC((q).head); c; })
#define TTY_GETCH(q, c) ({c = (q).buffer[(q).tail]; TTY_INC((q).tail); c; })


#define TTY0 0
#define TTY1 1
#define TTY2 2


typedef struct tty_queue_t {
    uint32_t head;
    uint32_t tail;
    char buffer[TTY_BUF_SIZE];
} tty_queue_t;

typedef struct tty_t {
    uint32_t flags;
    size_t (*write_func)(char* buf, size_t num);
    tty_queue_t read_q;
} tty_t;

extern tty_t tty_table[];

void init_tty();

size_t tty_write(int id, char* buf, size_t num);
size_t tty_read(int id, char* buf, size_t maxlen);

#endif // #ifndef TTY_H
