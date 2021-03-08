#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"

#include "kernel/kernel.h"
#include "kernel/multiboot.h"

extern void vga_text_write(char* buffer, size_t num);

void kernel_main()
{
    init_multiboot();
    
    printf("fett\n");
}