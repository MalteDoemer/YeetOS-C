#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

typedef void (*intr_func_t)(void* cpu_state);

void init_interrupts();

void set_intr_handler(size_t index, intr_func_t handler);

#endif // #ifndef INTERRUPTS_H
