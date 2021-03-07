#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "arch/x86/asm.h"
#include "kernel/kernel.h"
#include "kernel/tty.h"

uint32_t cursor_pos = 0;

// TODO: definitly put this somewhere else!
size_t vga_text_write(char* buffer, size_t num)
{
    char c;
    volatile uint16_t* vram = (volatile uint16_t*)(0xB8000 + KERNEL_BASE);

    for (size_t i = 0; i < num; i++) {
        c = buffer[i];

        switch (c) {
        case '\b':
            if (cursor_pos)
                cursor_pos--;
            break;
        case '\t':
            for (int i = 0; i < 4; i++) {
                vram[cursor_pos + i] = (0x0F << 8) | ' ';
            }
            cursor_pos = (cursor_pos + 4) & ~(4 - 1);
            break;
        case '\n':
            cursor_pos += 80;
            // break;
        case '\r':
            cursor_pos = (cursor_pos / 80) * 80;
            break;
        default:
            if (c >= ' ') {
                vram[cursor_pos] = (0x0F << 8) | c;
                cursor_pos++;
            }
            break;
        }
    }
}

void init_tty()
{
    memset(&tty_table[TTY0], 0, sizeof(tty_t));
    tty_table[TTY0].write_func = vga_text_write;
}