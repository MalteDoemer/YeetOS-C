#ifndef ISR_H
#define ISR_H

#include "stdint.h"
#include "stdbool.h"


typedef struct regs_t {
    uint32_t user_ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t user_eip, user_cs, user_eflags, user_esp, user_ss;
} regs_t;

typedef void (*isr_t)(regs_t*);

bool init_isr();

#endif // #ifndef ISR_H
