
#include "kernel.h"

void main(multiboot_info_t* multiboot_struct)
{
    clear_screen();
    init_gdt();
    printf("Success!");
}