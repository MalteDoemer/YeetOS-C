#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "libc/string.h"

#include "arch/x86/asm.h"
#include "arch/x86/gdt.h"
#include "arch/x86/idt.h"
#include "arch/x86/registers.h"
#include "arch/x86/exceptions.h"

#include "kernel/kernel.h"
#include "kernel/interrupts.h"
#include "kernel/panic.h"

typedef void (*intr_func_t)(regs_t* regs);

static intr_func_t handlers[256];

void isr_handler(regs_t* regs)
{
    if (handlers[regs->int_no])
        handlers[regs->int_no](regs);
    else
        WARN("No interrupt handler for int%#x\n", regs->int_no);
}

void set_intr_handler(size_t index, intr_func_t handler)
{
    if (index < 256)
        handlers[index] = handler;
    else
        WARN("Couldn't add interrupt handler %#x\n", index);
}

void set_keyboard_int(void* int_handler)
{
    set_intr_handler(0x21, (intr_func_t)int_handler);
}

void init_interrupts()
{
    set_intr_handler(0, divide_by_zero);
    set_intr_handler(6, invalid_opcode);
    set_intr_handler(8, double_fault);
    set_intr_handler(10, invalid_tss);
    set_intr_handler(11, segment_not_present);
    set_intr_handler(12, stack_segment_fault);
    set_intr_handler(13, general_protection_fault);
    set_intr_handler(14, page_fault);
}
