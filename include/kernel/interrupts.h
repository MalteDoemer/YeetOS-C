#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

rcode_t init_interrupts();

void set_keyboard_int(void* int_handler);

#endif // #ifndef INTERRUPTS_H
