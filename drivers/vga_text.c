#include "stdint.h"
#include "kernel/kernel.h"

static uint32_t pos;

// static uintptr_t vram = (uintptr_t)(0xB8000)

// static void update_cursor()
// {
//     outb(0x3D4, 14);
//     outb(0x3D5, pos >> 8);
//     outb(0x3D4, 15);
//     outb(0x3D5, pos);
// }
