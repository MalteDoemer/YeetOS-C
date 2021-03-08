#include "stdint.h"
#include "stddef.h"
#include "arch/x86/asm.h"
#include "kernel/kernel.h"

#define TAB_SIZE 4
#define SCREEN_START (0xB8000 + KERNEL_BASE)
#define SCREEN_END (SCREEN_START + 0x8000)

#define SCREEN_WIDTH 80

static uint32_t pos = 0;
static uint8_t attrs = 0x1F;
static uint16_t* vram = (uint16_t*)SCREEN_START;


static void update_cursor()
{
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos);
}

void update_origin()
{
    uint32_t org = ((uintptr_t)vram - SCREEN_START);
    outb(0x3D4, 12);
    outb(0x3D5, 0xff & (org >> 9));
    outb(0x3D4, 13);
    outb(0x3d5, 0xff & (org >> 1));
}

static void scroll_up(uint32_t lines)
{
    vram -= SCREEN_WIDTH * lines;
    if (vram < (uint16_t*)SCREEN_START)
        vram = (uint16_t*)SCREEN_START;
    update_origin();
}

static inline void scroll_down(uint32_t lines)
{
    vram += SCREEN_WIDTH * lines;

    if (vram > (uint16_t*)SCREEN_END)
    {
        
    }
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
            vram[pos + i] = (attrs << 8) | ' ';
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
            vram[pos] = (attrs << 8) | c;
            pos++;
        }
        break;
    }
}

void vga_text_test()
{

    uint16_t data[32];
    
    stosw(data, 0, 32);
    stosw(data, 0xdead, 32);
}

size_t vga_text_write(char* buffer, size_t num)
{
    for (size_t i = 0; i < num; i++) {
        write_char(buffer[i]);
    }
    return num;
}
