#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "libc/stdio.h"
#include "libc/string.h"

#include "arch/x86/asm.h"
#include "kernel/kernel.h"
#include "kernel/multiboot.h"


// this will be handled by the fs abstraction eventually
extern void init_serial();

// this will be handled by the fs abstraction eventually
extern void init_vga_text();

void init_arch()
{
    init_serial();
    init_vga_text();
}