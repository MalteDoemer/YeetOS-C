#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "arch-x86/asm.h"
#include "arch-x86/gdt.h"
#include "arch-x86/idt.h"
#include "arch-x86/interrupts.h"

#include "kernel/kernel.h"
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
    for (;;) hlt();
}

void segment_not_present(regs_t* regs)
{
    PANIC("%s", "Segfault");
    for (;;) hlt();
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