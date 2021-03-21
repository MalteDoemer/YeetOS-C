#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "arch-x86/registers.h"
#include "kernel/kernel.h"

void divide_by_zero(regs_t* regs);
void invalid_opcode(regs_t* regs);
void double_fault(regs_t* regs);
void invalid_tss(regs_t* regs);
void segment_not_present(regs_t* regs);
void stack_segment_fault(regs_t* regs);
void general_protection_fault(regs_t* regs);
void page_fault(regs_t* regs);

#endif // #ifndef EXCEPTIONS_H
