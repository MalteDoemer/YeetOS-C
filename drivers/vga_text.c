#include "stdint.h"
#include "stddef.h"
#include "arch/x86/asm.h"
#include "kernel/kernel.h"

static uint32_t pos = 0;
static uint8_t attrs = 0x1F;
static uintptr_t vram = (uintptr_t)(0xB8000 + KERNEL_BASE);

#define TAB_SIZE 4
#define SCREEN_WIDTH 80

static void update_cursor()
{
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos);
}

static void write_char(char c)
{
    switch (c) {
    case '\b':
        if (pos)
            pos--;
        break;
    case '\t':
        for (int i = 0; i < TAB_SIZE; i++) {
            ((uint16_t*)vram)[pos + i] = (attrs << 8) | ' ';
        }
        pos = (pos + TAB_SIZE) & ~(TAB_SIZE - 1);
        break;
    case '\n':
        pos += SCREEN_WIDTH;
        // break;
    case '\r':
        pos = (pos / SCREEN_WIDTH) * SCREEN_WIDTH;
        break;
    default:
        if (c >= ' ') {
            ((uint16_t*)vram)[pos] = (attrs << 8) | c;
            pos++;
        }
        break;
    }
}

size_t vga_text_write(char* buffer, size_t num)
{
    for (size_t i = 0; i < num; i++) {
        write_char(buffer[i]);
    }
    return num;
}
