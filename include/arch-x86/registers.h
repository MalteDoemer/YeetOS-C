#ifndef REGISTERS_H
#define REGISTERS_H

#include "stdint.h"

typedef struct regs_t {
    uint32_t user_ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t user_eip, user_cs, user_eflags, user_esp, user_ss;
} regs_t;

#endif // #ifndef REGISTERS_H
