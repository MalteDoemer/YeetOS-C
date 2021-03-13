#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "libc/stdio.h"
#include "libc/string.h"

#include "arch/x86/asm.h"
#include "arch/x86/gdt.h"
#include "arch/x86/idt.h"
#include "arch/x86/multiboot.h"

#include "kernel/kernel.h"



void init_arch()
{
    init_multiboot();
    init_gdt();
    init_idt();

    extern uint8_t kernel_stack_top;
    set_esp0((uintptr_t)&kernel_stack_top);
}