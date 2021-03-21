#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "stdint.h"
#include "stddef.h"

#include "kernel/kernel.h"

typedef struct regs_t {
    uint32_t user_ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t user_eip, user_cs, user_eflags, user_esp, user_ss;
} regs_t;

typedef void (*intr_func_t)(regs_t* regs);

rcode_t init_interrupts();
void set_keyboard_int(void* int_handler);

#endif // #ifndef INTERRUPTS_H
