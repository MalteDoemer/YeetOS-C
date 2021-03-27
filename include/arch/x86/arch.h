#ifndef ARCH_H
#define ARCH_H

int init_arch();

#define __x86__
#define KERNEL_BASE 0xC0000000

#ifdef ARCH_INCLUDE_ASM
#include "arch/x86/asm.h"
#endif

#ifdef ARCH_INCLUDE_TYPES
#include "arch/x86/types.h"
#endif

#endif // #ifndef ARCH_H
