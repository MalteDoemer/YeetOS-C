#include "stdint.h"
#include "stddef.h"
#include "kernel/kernel.h"
#include "kernel/tty.h"

tty_t tty_table[3];


size_t tty_write(int id, char* buf, size_t num)
{
    tty_t* tty = tty_table + id;
    return tty->write_func(buf, num);
}

size_t tty_read(int id, char* buf, size_t maxlen)
{
    tty_t* tty = tty_table + id;
    size_t chars = TTY_SIZE(tty->read_q);

    if (chars > maxlen)
        chars = maxlen;

    for (size_t i = 0; i < chars; i++) {
        TTY_GETCH(tty->read_q, buf[i]);
    }

    return chars;
}
