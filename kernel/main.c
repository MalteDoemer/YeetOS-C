
#include "kernel.h"

void main(multiboot_info_t* multiboot_struct)
{
    init_gdt();
    printf("Success!");
}