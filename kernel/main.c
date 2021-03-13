#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/assert.h"

#include "kernel/kernel.h"
#include "kernel/kheap.h"
#include "kernel/panic.h"
#include "kernel/debug.h"
#include "kernel/multiboot.h"

void color_test()
{
    printf("\x1b[30m A \x1b[31m B \x1b[32m C \x1b[33m D \x1b[0m \x1b[34m E \x1b[35m F \x1b[36m G \x1b[37m H \x1b[0m\n");
}

void kernel_main()
{
    init_multiboot();
    init_arch();
    init_kheap();

    printf("\033[?25l");
    printf("\033[J");
    printf("\033[H");

    color_test();

    debugf("Hello to serial console\n");
}