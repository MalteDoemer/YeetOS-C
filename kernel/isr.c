
#include "kernel.h"

isr_t isrs[256];

void isr_handler(regs_t* regs)
{
    if (isrs[regs->int_no])
        isrs[regs->int_no](regs);
    else 
        printf("missing intterupt handler #%d\n", regs->int_no);
}

void divide_by_zero(regs_t* regs)
{
    clear_screen();
    printf("Divide By Zero AMK!!");
    for (;;);
}

void debug(regs_t* regs)
{
}

void nmi(regs_t* regs)
{
}

void breakpoint(regs_t* regs)
{
}

void overflow(regs_t* regs)
{
}

void bound_range_exceeded(regs_t* regs)
{
}

void invalid_opcode(regs_t* regs)
{
}

void device_not_available(regs_t* regs)
{
}

void double_fault(regs_t* regs)
{
}
void coprocessor_segment_overrun(regs_t* regs)
{
}
void invalid_tss(regs_t* regs)
{
}

void segment_not_present(regs_t* regs)
{
}
void stack_segment_fault(regs_t* regs)
{
}
void general_protection_fault(regs_t* regs)
{
}
void page_fault(regs_t* regs)
{
}

void x87_floating_point_exception(regs_t* regs)
{
}
void alignment_check(regs_t* regs)
{
}
void machine_check(regs_t* regs)
{
}
void simd_floating_point_exception(regs_t* regs)
{
}
void virtualization_exception(regs_t* regs)
{
}
void security_exception(regs_t* regs)
{
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

    return true;
}
