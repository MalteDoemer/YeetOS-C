
#include "kernel.h"

void main(mboot_info_t* multiboot_struct)
{
    set_mboot_info(multiboot_struct);

    clear_screen();

    init_gdt();
    init_idt();
    init_isr();

    init_mem();


    printf("Success!");

    return;
}