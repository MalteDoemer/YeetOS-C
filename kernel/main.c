#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "kernel/kernel.h"
#include "kernel/multiboot.h"
#include "kernel/tty.h"

void kernel_main()
{
    init_multiboot();
    init_tty();
    
    tty_write(TTY0, "Fett\n", 5);

}