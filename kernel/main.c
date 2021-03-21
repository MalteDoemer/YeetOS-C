#include "stdint.h"
#include "stddef.h"
#include "stdio.h"

#include "arch/arch.h"

#include "kernel/kernel.h"

void kernel_main()
{
    init_arch();
}