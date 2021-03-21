#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "arch/x86/types.h"

#include "kernel/kernel.h"
#include "kernel/cpu.h"
#include "kernel/panic.h"

void divide_by_zero(regs_t* regs)
{
    PANIC("%s", "Divide By Zero AMK!!");
}

void invalid_opcode(regs_t* regs)
{
    PANIC("%s", "Invalid opcode");
}

void double_fault(regs_t* regs)
{
    PANIC("%s", "Double Fault");
}

void invalid_tss(regs_t* regs)
{
    PANIC("%s", "Invalid TSS");
    for (;;) halt();
}

void segment_not_present(regs_t* regs)
{
    PANIC("%s", "Segfault");
    for (;;) halt();
}

void stack_segment_fault(regs_t* regs)
{
    PANIC("%s", "Stack Segfault");
}

void general_protection_fault(regs_t* regs)
{
    PANIC("%s", "General Protection Fault");
}

void page_fault(regs_t* regs)
{
    PANIC("%s", "Pagefault");
}