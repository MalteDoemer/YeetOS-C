#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "stdio.h"
#include "string.h"

#include "kernel/kernel.h"
#include "kernel/multiboot.h"


void kernel_main()
{
    init_multiboot();
    init_arch();
}