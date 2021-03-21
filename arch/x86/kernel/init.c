
#include "stdint.h"
#include "stddef.h"

#include "arch/x86/asm.h"

#include "arch/x86/gdt.h"
#include "arch/x86/idt.h"
#include "arch/x86/multiboot.h"
#include "arch/x86/interrupts.h"

#include "kernel/kernel.h"

rcode_t init_arch()
{
    init_multiboot();

    init_gdt();
    init_idt();
    init_interrupts();

    return RCODE_SUCESS;
}