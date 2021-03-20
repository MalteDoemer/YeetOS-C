#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/assert.h"

#include "arch/x86/asm.h"
#include "arch/x86/gdt.h"
#include "arch/x86/idt.h"
#include "arch/x86/multiboot.h"

#include "kernel/kernel.h"
#include "kernel/mm.h"
#include "kernel/cpu.h"
#include "kernel/kheap.h"
#include "kernel/panic.h"
#include "kernel/debug.h"
#include "kernel/interrupts.h"

void color_test()
{
    printf("\033[30mA \033[31m B \033[32m C \033[33m D \033[0m \033[34m E \033[35m F \033[36m G \033[37m H \033[0m\n");
}

void heap_test()
{
    int* chars = kmalloc(1024);
    memcpy(chars, "+--------------------+\n| Welcome to YeetOS! |\n+--------------------+", 69);
    printf("%s\n\n", chars);
    kfree(chars);
}

void call_constructors()
{
    typedef void (*ctor_t)();

    extern ctor_t ctors_start;
    extern ctor_t ctors_end;

    if (&ctors_start == &ctors_end)
        return;

    ctor_t* ctor = &ctors_end;

    do {
        ctor--;
        (*ctor)();
    } while (ctor > &ctors_start);
}

void kernel_main()
{
    init_multiboot();
    init_mm();
    init_gdt();
    init_idt();
    init_kheap();

    init_interrupts();

    call_constructors();

    printf("\033[?25l");
    printf("\033[J");
    printf("\033[H");

    heap_test();
    color_test();

    set_esp0((uint32_t)SYMBOL_VALUE(kernel_stack_top));
}