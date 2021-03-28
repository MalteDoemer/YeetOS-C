#ifndef TYPES_H
#define TYPES_H

#include "stdint.h"
#include "stddef.h"

typedef long long lloff_t;
typedef long loff_t;

typedef struct regs_t {
    uint32_t user_ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t user_eip, user_cs, user_eflags, user_esp, user_ss;
} regs_t;


#endif // #ifndef TYPES_H
