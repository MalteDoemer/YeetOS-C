
#include "arch/x86/asm.h"
#include "kernel/kernel.h"
#include "kernel/cpu.h"

void halt()
{
    hlt();
}