#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "kernel/kernel.h"
#include "kernel/multiboot.h"

void kernel_main()
{
    init_multiboot();
    init_tty();
    
    tty_write(TTY0, "Fett\n", 5);


    int i = ({ 
        int b = 36;
        b;
    });

    for(int i = ({int b = 36;b;}); i < 100; {i ++; printf(i);} )
}