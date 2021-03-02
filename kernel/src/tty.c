#include "kernel.h"

#ifdef GRAPHICS
#error "graphics not supported!"
#else

uint16_t* const vram = (uint16_t* const)(0xB8000 + KERNEL_BASE);
uint16_t cursor_pos = 0;
uint8_t vga_color = 0x1F;

static void update_cursor()
{
    outb(0x3D4, 14);
    outb(0x3D5, cursor_pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_pos);
}

static void scroll()
{
    if (cursor_pos / SCREEN_WIDTH >= 25) {

        for (int i = 0; i < SCREEN_WIDTH * (SCREEN_HEIGHT - 1); i++)
            vram[i] = vram[i + SCREEN_WIDTH];

        for (int i = SCREEN_WIDTH * (SCREEN_HEIGHT - 1); i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
            vram[i] = (vga_color << 8) | ' ';

        cursor_pos = SCREEN_WIDTH * (SCREEN_HEIGHT - 1);
    }
}

static void write_char(char c)
{
    switch (c) {
    case '\b':
        if (cursor_pos)
            cursor_pos--;
        break;
    case '\t':
        for (int i = 0; i < TAB_SIZE; i++) {
            vram[cursor_pos + i] = (vga_color << 8) | ' ';
        }
        cursor_pos = (cursor_pos + TAB_SIZE) & ~(TAB_SIZE - 1);
        break;
    case '\n':
        cursor_pos += SCREEN_WIDTH;
        // break;
    case '\r':
        cursor_pos = (cursor_pos / SCREEN_WIDTH) * SCREEN_WIDTH;
        break;
    default:
        if (c >= ' ') {
            vram[cursor_pos] = (vga_color << 8) | c;
            cursor_pos++;
        }
        break;
    }
}

static void write_num(uint32_t num, int base)
{
    if (num == 0) {
        putc('0');
        return;
    }

    char buffer[32]; // a 32-bit number should never be longer than 32 chars

    int i = 0;
    while (num != 0) {
        buffer[i] = (num % base) < 10 ? (num % base) + '0' : (num % base) + '7';
        num /= base;
        i++;
    }

    while (i > 0) {
        putc(buffer[--i]);
    }
}

void putc(char c)
{
    write_char(c);
    scroll();
    update_cursor();
}

void puts(const char* s)
{

    while (*s != '\0') {
        write_char(*s);
        scroll();
        s++;
    }

    update_cursor();
}

void printf(const char* fmt, ...)
{
    va_list params;
    va_start(params, fmt);

    while (*fmt != '\0') {

        if (*fmt != '%') {
            write_char(*(fmt++));
        } else {
            fmt++;
            switch (*fmt) {
            case 'd':
            case 'i':
            case 'u':
            case 'x':
            case 'X':
            case 'b':
            case 'B':
            case 'o':
            case 'O':;
                uint32_t num = va_arg(params, uint32_t);
                int base;

                if (*fmt == 'd' || *fmt == 'i') {
                    base = 10;

                    if (((int32_t)num) < 0) {
                        putc('-');
                        num = (uint32_t)((int32_t)num * -1);
                    }
                } else if (*fmt == 'u') {
                    base = 10;
                } else if (*fmt == 'x' || *fmt == 'X') {
                    base = 16;
                    puts("0x");
                } else if (*fmt == 'o' || *fmt == 'O') {
                    base = 8;
                    puts("0o");
                } else if (*fmt == 'b' || *fmt == 'B') {
                    base = 2;
                    puts("0b");
                }
                write_num(num, base);
                break;
            case 'c':
                putc(va_arg(params, int)); // need to use int instead of char
                break;
            case 's':
                puts(va_arg(params, char*));
                break;
            case 'p':;
                uint32_t ptr = va_arg(params, uint32_t);
                puts("0x");
                for (int i =  28; i >= 0; i -= 4) {
                    char c = ((ptr >> i) & 0xF);
                    c += c < 10 ? '0' : '7';
                    putc(c);
                }

                break;
            case '%':
                putc('%');
                break;
            }
            fmt++;
        }
    }

    va_end(params);
}

#endif
