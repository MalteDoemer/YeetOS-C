#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "stdint.h"
#include "stddef.h"

#include "arch/x86/types.h"

#include "kernel/kernel.h"

typedef void (*intr_func_t)(regs_t* regs);

int init_interrupts();
void set_keyboard_int(void* int_handler);

#endif // #ifndef INTERRUPTS_H
