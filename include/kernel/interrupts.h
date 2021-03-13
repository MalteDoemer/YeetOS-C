#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

void init_interrupts();

void set_intr_handler(size_t index, void* handler);

#endif // #ifndef INTERRUPTS_H
