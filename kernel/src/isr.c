
#include "kernel.h"

isr_t isrs[256];

void isr_handler(regs_t* regs)
{
    if (isrs[regs->int_no])
        isrs[regs->int_no](regs);
    else
        WARN("missing intterupt handler #%d\n", regs->int_no);
}

void divide_by_zero(regs_t* regs)
{
    PANIC("%s", "Divide By Zero AMK!!");
    for (;;)
        hlt();
}

void debug(regs_t* regs)
{
    WARN("%s", "Debug Exception.");
}

void nmi(regs_t* regs)
{
    WARN("%s", "NMI occured.");
}

void breakpoint(regs_t* regs)
{
    WARN("%s", "Breakpoint hit.");
}

void overflow(regs_t* regs)
{
    PANIC("%s", "Overflow Exception");
    for (;;) hlt();
}

void bound_range_exceeded(regs_t* regs)
{
    PANIC("%s", "Bound Range Exception");
    for (;;) hlt();
}

void invalid_opcode(regs_t* regs)
{
    PANIC("%s", "Error: The CPU found an invalid opcode.\nThat is very bad.\nIt means the human being who wrote this program is very bad at programming.\nI wish him good luck for fixing that bug.\nHave a nice day!\n\nMaybe he isn't that bad after all and just used an int 0x6 to print this message");
    for (;;) hlt();
}

void device_not_available(regs_t* regs)
{
    PANIC("%s", "Device Not Available Exception");
    for (;;) hlt();
}

void double_fault(regs_t* regs)
{
    PANIC("%s", "Double Fault");
    for (;;) hlt();
}

void coprocessor_segment_overrun(regs_t* regs)
{
    PANIC("%s", "Coprocessor Segment Overrun");
    for (;;) hlt();
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
    for (;;) hlt();
}

void general_protection_fault(regs_t* regs)
{
    PANIC("%s", "General Protection Fault");
    for (;;) hlt();

}

void page_fault(regs_t* regs)
{
    PANIC("%s", "Pagefault");
    for (;;) hlt;
}

void x87_floating_point_exception(regs_t* regs)
{
    PANIC("%s", "x87 Floating Point Exception");
    for (;;) hlt();
}

void alignment_check(regs_t* regs)
{
    PANIC("%s", "Alignment Check Exception");
    for (;;) hlt();
}

void machine_check(regs_t* regs)
{
    PANIC("%s", "Machine Check Exception");
    for (;;) hlt();

}

void simd_floating_point_exception(regs_t* regs)
{
    PANIC("%s", "SIMD Floating-Point Exception");
    for (;;) hlt();
}

void virtualization_exception(regs_t* regs)
{
    PANIC("%s", "Virtualization Exception");
    for (;;) hlt();
}

void security_exception(regs_t* regs)
{
    PANIC("%s", "Security Exception");
    for (;;) hlt();
}

void int80()
{
}

void fett(regs_t* regs)
{
    printf("Keypressed\n");
}

bool init_isr()
{
    memset(isrs, 0, sizeof(isrs));

    isrs[0] = divide_by_zero;
    isrs[1] = debug;
    isrs[2] = nmi;
    isrs[3] = breakpoint;
    isrs[4] = overflow;
    isrs[5] = bound_range_exceeded;
    isrs[6] = invalid_opcode;
    isrs[7] = device_not_available;
    isrs[8] = double_fault;
    isrs[9] = coprocessor_segment_overrun;
    isrs[10] = invalid_tss;
    isrs[11] = segment_not_present;
    isrs[12] = stack_segment_fault;
    isrs[13] = general_protection_fault;
    isrs[14] = page_fault;
    isrs[16] = x87_floating_point_exception;
    isrs[17] = alignment_check;
    isrs[18] = machine_check;
    isrs[19] = simd_floating_point_exception;
    isrs[20] = virtualization_exception;
    isrs[30] = security_exception;

    isrs[0x80] = int80;

    return true;
}
