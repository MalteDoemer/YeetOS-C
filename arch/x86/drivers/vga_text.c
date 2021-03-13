#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "libc/string.h"
#include "libc/ctype.h"

#include "arch/x86/asm.h"
#include "kernel/panic.h"
#include "kernel/kernel.h"

#define TAB_SIZE 4
#define SCREEN_START (0xB8000 + KERNEL_BASE)
#define SCREEN_END (SCREEN_START + 0x8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define MAX_ARGS 16

typedef struct vga_color_t {
    union {
        struct {
            uint8_t fg : 4;
            uint8_t bg : 4;
        };
        uint8_t col;
    };
} vga_color_t;

static int32_t pos = 0;
static vga_color_t attrs = { 0x0F };
static uint16_t* vram = (uint16_t*)SCREEN_START;

static void update_cursor()
{
    // ASSERT
    uint32_t p = (uint32_t)pos;

    outb(0x3D4, 14);
    outb(0x3D5, p >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, p);
}

static void update_origin()
{
    uint32_t org = ((uintptr_t)vram - SCREEN_START);
    outb(0x3D4, 12);
    outb(0x3D5, 0xff & (org >> 9));
    outb(0x3D4, 13);
    outb(0x3d5, 0xff & (org >> 1));
}

static void gotoxy(int32_t x, int32_t y)
{
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        pos = y * SCREEN_WIDTH + x;
    }
}

static void scroll_up(uint32_t lines)
{
}

static void scroll_down(uint32_t lines)
{
}

static inline void check_scroll()
{
}

static inline void clear_screen()
{
    stosw((void*)SCREEN_START, (attrs.col << 8) | ' ', (SCREEN_END - SCREEN_START) / 2);
}

static inline void show_cursor()
{
    outb(0x3D4, 0x09);
    outb(0x3D5, 0x0F); // max scan line register -> set char height

    outb(0x3D4, 0x0B);
    outb(0x3D5, 15); // cursor end line

    outb(0x3D4, 0x0A);
    outb(0x3D5, 14); // cursor start line
}

static inline void hide_cursor()
{
    outb(0x3D4, 0x09);
    outb(0x3D5, 0x0F); // max scan line register -> set char height

    outb(0x3D4, 0x0B);
    outb(0x3D5, 16); // cursor end line

    outb(0x3D4, 0x0A);
    outb(0x3D5, 16); // cursor start line
}

static inline void bell()
{
}

static inline void backspace()
{
    pos--;
    vram[pos] = (attrs.col << 8) | ' ';
}

static inline void tab()
{
    int num = pos % TAB_SIZE;
    while (num--) {
        vram[pos++] = (attrs.col << 8) | ' ';
    }
}

static inline void newline()
{
    pos = (pos / SCREEN_WIDTH + 1) * SCREEN_WIDTH;
}

static inline void carriage_return()
{
    pos = (pos / SCREEN_WIDTH) * SCREEN_WIDTH;
}

static inline void del()
{
    vram[pos] = (attrs.col << 8) | ' ';
    pos--;
}

static size_t atoi_skip(const char** s)
{
    size_t i = 0;

    while (isdigit(**s)) {
        i = i * 10 + **s - '0';
        (*s)++;
    }

    return i;
}

static char* escape(char* buffer)
{
    size_t args[MAX_ARGS];
    memset(args, 0, sizeof(args));

    char letter = 0;
    bool ques = false;
    bool equ = false;
    size_t argc = 0;

    if (*buffer == '[') {
        buffer++;

        if (*buffer == '?') {
            ques = true;
            buffer++;
        }

        if (*buffer == '=') {
            equ = true;
            buffer++;
        }

        while (isdigit(*buffer)) {

            size_t num = atoi_skip(&buffer);
            if (argc < MAX_ARGS)
                args[argc++] = num;

            if (*buffer == ';') {
                buffer++;
            }
        }

        letter = *buffer++;

        uint32_t cx = pos % SCREEN_WIDTH;
        uint32_t cy = pos / SCREEN_WIDTH;

        switch (letter) {

        case 'H': {
            if (argc == 0) {
                gotoxy(0, 0);
            } else if (argc >= 2) {
                gotoxy(args[0], args[1]);
            }
        } break;

        case 'f': {
            if (argc >= 2) {
                gotoxy(args[0], args[1]);
            }
        } break;

        case 'A': {
            if (argc >= 1) {
                gotoxy(cx, cy - args[0]);
            }
        } break;

        case 'B': {
            if (argc >= 1) {
                gotoxy(cx, cy + args[0]);
            }
        } break;

        case 'C': {
            if (argc >= 1) {
                gotoxy(cx - args[0], cy);
            }
        } break;

        case 'D': {
            if (argc >= 1) {
                gotoxy(cx + args[0], cy);
            }
        } break;

        case 'E': {

        } break;

        case 'F': {

        } break;

        case 'G': {

        } break;

        case 'R': {

        } break;

        case 's': {

        } break;

        case 'u': {

        } break;

        case 'J': {
            if (argc == 0 || (argc >= 1 && args[0] == 2)) {
                clear_screen();
            }
        } break;

        case 'K': {

        } break;

        case 'm': {
            bool bright = true;

            for (size_t i = 0; i < argc; i++) {
                size_t num = args[i];

                // if (num == 0) { // reset
                //     attrs = 0x0F;
                // } else if (num == 2) { // bright/dark
                //     bright = false;
                // } else if (num == 7) { // inverse
                //     uint8_t lo = attrs & 0xF;
                //     uint8_t hi = (attrs >> 4) & 0xF;
                //     attrs = lo << 4 | hi;
                // } else if (num >= 30 && num <= 37) {
                //     attrs = (attrs & 0xF0) | ((num - 30 + bright * 8) & 0xF);
                // } else if (num >= 40 && num <= 47) {
                //     attrs = (((num - 40 + bright * 8) & 0xF) << 4) | (attrs & 0xF);
                // }

                switch (num) {
                case 0: // reset
                    attrs.col = 0x0F;
                    break;
                case 2: // toggle dim/faint
                    bright = !bright;
                    break;
                case 7: // inverse
                    attrs = ({ vga_color_t tmp; tmp.bg = attrs.fg; tmp.fg = attrs.bg; tmp; });
                    break;

                case 30: // black fg
                    attrs.fg = 0 + bright * 8;
                    break;

                case 31: // red fg
                    attrs.fg = 4 + bright * 8;
                    break;

                case 32: // green fg
                    attrs.fg = 2 + bright * 8;
                    break;

                case 33: // yellow fg
                    attrs.fg = 6 + bright * 8;
                    break;

                case 34: // blue fg
                    attrs.fg = 1 + bright * 8;
                    break;

                case 35: // magenta fg
                    attrs.fg = 5 + bright * 8;
                    break;

                case 36: // cyan fg
                    attrs.fg = 3 + bright * 8;
                    break;

                case 37: // white fg
                    attrs.fg = 7 + bright * 8;
                    break;

                case 40: // black bg
                    attrs.bg = 0 + bright * 8;
                    break;

                case 41: // red bg
                    attrs.bg = 4 + bright * 8;
                    break;

                case 42: // green bg
                    attrs.bg = 2 + bright * 8;
                    break;

                case 43: // yellow bg
                    attrs.bg = 6 + bright * 8;
                    break;

                case 44: // blue bg
                    attrs.bg = 1 + bright * 8;
                    break;

                case 45: // magenta bg
                    attrs.bg = 5 + bright * 8;
                    break;

                case 46: // cyan bg
                    attrs.bg = 3 + bright * 8;
                    break;

                case 47: // white bg
                    attrs.bg = 7 + bright * 8;
                    break;

                default:
                    break;
                }
            }

        } break;

        case 'l': {
            if (ques && argc >= 1 && args[0] == 25) {
                show_cursor();
            }
        } break;

        case 'h': {
            if (ques && argc >= 1 && args[0] == 25) {
                hide_cursor();
            }
        } break;

        default:
            break;
        }
    }

    return buffer;
}

size_t vga_text_write(char* buffer, size_t num)
{

    if (buffer[num - 1] != 0) {
        WARN("%s\n", "buffer is not zero terminated");
        return 0;
    }

    char* b = buffer;

    while (*b) {
        switch (*b) {
        case '\a':
            bell();
            b++;
            break;

        case '\b':
            backspace();
            b++;
            break;

        case '\t':
            tab();
            b++;
            break;

        case '\n':
        case '\v':
        case '\f':
            newline();
            b++;
            break;
        case '\r':
            carriage_return();
            b++;
            break;

        case '\e':
            b++;
            b = escape(b);
            break;
        case 127:
            del();
            b++;
            break;

        default:
            if (*b >= ' ') {
                vram[pos++] = (attrs.col << 8) | *b;
            }
            b++;
        }
    }
    update_cursor();
    return b - buffer;
}
