#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "libc/stdio.h"
#include "libc/string.h"

#include "kernel/kernel.h"
#include "kernel/multiboot.h"


void kernel_main()
{
    init_multiboot();
    init_arch();

    printf("fett\n");
    printf("\x1b[?25l");
    printf("lol");
}