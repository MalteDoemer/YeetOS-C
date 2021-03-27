#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "errno.h"

#include "kernel/kernel.h"

#if !defined(__x86__) && !defined(__x64__)
#error "serial only supported on x86 or x64"
#endif

#ifdef __x86__
#include "arch/x86/asm.h"
#endif

#define COM1 0x3F8

#define DATA 0
#define IER 1
#define FIFO_CTRL 2
#define LINE_CTRL 3
#define MODEM_CTRL 4
#define LINE_STAT 5
#define MODEM_STAT 6
#define SCRATCH 7

static int init_com_port(uint16_t port)
{
    // disable all serial interrupts
    outb(port + IER, 0x00);

    // set divisor to 1
    outb(port + LINE_CTRL, 0x80);
    outb(port + 0, 0x01);
    outb(port + 1, 0x00);

    // set 8N1
    outb(port + LINE_CTRL, 0x03);

    // enable FIFO
    outb(port + FIFO_CTRL, 0xC7);

    // set OUT1, OUT2, RTS and DSR to inactive
    outb(port + MODEM_CTRL, 0x0F);

    return SUCCESS;
}

static inline bool is_transmit_empty(uint16_t port)
{
    return inb(port + LINE_STAT) & 0x20;
}

size_t serial_write(uint16_t port, char* buffer, size_t num)
{

    for (size_t n = num; n; n--) {

        while (!is_transmit_empty(port));

        outb(port + DATA, *buffer++);
    }

    return num;
}

CTOR int init_serial()
{
    init_com_port(COM1);
    return SUCCESS;
}
