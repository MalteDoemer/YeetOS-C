
#include "kernel.h"

void main(mboot_info_t* multiboot_struct, uint32_t kernel_stack)
{
    set_mboot_info(multiboot_struct);

    clear_screen();

    init_gdt();
    init_idt();
    init_isr();
    init_mem();

    set_esp0(kernel_stack);

    printf("Success!");

    return;
}