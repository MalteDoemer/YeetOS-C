#ifndef KERNEL_H
#define KERNEL_H

#define KERNEL_BASE 0xC0000000

#define PANIC(msg, __VA_ARGS) clear_screen(); printf("Fatal Error: "); printf(msg, __VA_ARGS)
#define WARN(msg, __VA_ARGS) printf("Warning: "); printf(msg, __VA_ARGS)

#include "stdint.h"
#include "stdarg.h"
#include "stddef.h"
#include "stdbool.h"
#include "common.h"
#include "multiboot.h"
#include "tty.h"
#include "memory.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"

#endif // #ifndef KERNEL_H
