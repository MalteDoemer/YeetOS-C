
#include "kernel.h"

void main(multiboot_info_t* multiboot_struct)
{
    clear_screen();

    init_gdt();
    init_idt();
    init_isr();

    __asm("int $0x80");

    printf("Success!");

    return;
}