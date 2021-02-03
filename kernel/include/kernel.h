#ifndef KERNEL_H
#define KERNEL_H

#define KERNEL_BASE 0xC0000000

#include "stdint.h"
#include "stdarg.h"
#include "stddef.h"
#include "stdbool.h"
#include "common.h"
#include "multiboot.h"
#include "tty.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"

#endif // #ifndef KERNEL_H
