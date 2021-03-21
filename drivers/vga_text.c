#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "libc/string.h"
#include "libc/ctype.h"

#include "arch/arch.h"

#if !defined(__X86__)
#error "vga text mode only supported for x86"
#endif

#include "arch/asm.h"

#include "kernel/kernel.h"
#include "kernel/panic.h"

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
static vga_color_t attrs = { .col = 0x0F };
static uint16_t* vram = (uint16_t*)SCREEN_START;

uint8_t ansi_to_vga[256] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '\b',
    '\t',
    '\n',
    '\v',
    '\f',
    '\r',
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '\e',
    0,
    0,
    0,
    0,
    ' ',
    '!',
    '"',
    '#',
    '$',
    '%',
    '&',
    '\'',
    '(', ')',
    '*',
    '+',
    ',',
    '-',
    '.',
    '/',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    ':',
    ';',
    '<',
    '=',
    '>',
    '?',
    '@',
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    '[',
    '\\',
    ']',
    '^',
    '_',
    '`',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z',
    '{',
    '|',
    '}',
    '~',
    0,
    0,   // €
    0,   //
    0,   // ‚
    159, // ƒ
    0,   // „
    0,   // …
    0,   // †
    0,   // ‡
    0,   // ˆ
    0,   // ‰
    0,   // Š
    0,   // ‹
    0,   // Œ
    0,   //
    0,   // Ž
    0,   //
    0,   //
    0,   // ‘
    0,   // ’
    0,   // “
    0,   // ”
    0,   // •
    0,   // –
    0,   // —
    0,   // ˜
    0,   // ™
    0,   // š
    0,   // ›
    0,   // œ
    0,   //
    0,   // ž
    0,   // Ÿ
    0,   //
    173, // ¡
    155, // ¢
    156, // £
    0,   // ¤
    157, // ¥
    '|', // ¦
    22,  // §
    0,   // ¨
    0,   // ©
    166, // ª
    174, // «
    170, // ¬
    0,   // ­
    0,   // ®
    0,   // ¯
    248, // °
    241, // ±
    254, // ²
    0,   // ³
    0,   // ´
    230, // µ
    0,   // ¶
    249, // ·
    0,   // ¸
    0,   // ¹
    167, // º
    175, // »
    172, // ¼
    171, // ½
    0,   // ¾
    168, // ¿
    0,   // À
    0,   // Á
    0,   // Â
    0,   // Ã
    142, // Ä
    143, // Å
    146, // Æ
    128, // Ç
    0,   // È
    144, // É
    0,   // Ê
    0,   // Ë
    0,   // Ì
    0,   // Í
    0,   // Î
    0,   // Ï
    0,   // Ð
    165, // Ñ
    0,   // Ò
    0,   // Ó
    0,   // Ô
    0,   // Õ
    153, // Ö
    0,   // ×
    0,   // Ø
    0,   // Ù
    0,   // Ú
    0,   // Û
    154, // Ü
    0,   // Ý
    0,   // Þ
    0,   // ß
    133, // à
    160, // á
    131, // â
    0,   // ã
    132, // ä
    134, // å
    0,   // æ
    135, // ç
    138, // è
    130, // é
    136, // ê
    137, // ë
    141, // ì
    161, // í
    140, // î
    139, // ï
    0,   // ð
    164, // ñ
    149, // ò
    162, // ó
    147, // ô
    0,   // õ
    148, // ö
    246, // ÷
    237, // ø
    151, // ù
    163, // ú
    150, // û
    129, // ü
    0,   // ý
    0,   // þ
    152, // ÿ
};

static void update_cursor()
{
    // ASSERT
    uint32_t p = (uint32_t)pos;

    outb(0x3D4, 14);
    outb(0x3D5, p >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, p);
}

// static void update_origin()
// {
//     uint32_t org = ((uintptr_t)vram - SCREEN_START);
//     outb(0x3D4, 12);
//     outb(0x3D5, 0xff & (org >> 9));
//     outb(0x3D4, 13);
//     outb(0x3d5, 0xff & (org >> 1));
// }

static void gotoxy(int32_t x, int32_t y)
{
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        pos = y * SCREEN_WIDTH + x;
    }
}

// static void scroll_up(uint32_t lines)
// {
// }

// static void scroll_down(uint32_t lines)
// {
// }

// static inline void check_scroll()
// {
// }

static inline void clear_screen()
{
    stosw((void*)SCREEN_START, (attrs.col << 8) | ansi_to_vga[' '], (SCREEN_END - SCREEN_START) / 2);
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
    vram[pos] = (attrs.col << 8) | ansi_to_vga[' '];
}

static inline void tab()
{
    int num = pos % TAB_SIZE;
    while (num--) {
        vram[pos++] = (attrs.col << 8) | ansi_to_vga[' '];
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
    vram[pos] = (attrs.col << 8) | ansi_to_vga[' '];
    pos--;
}

static size_t atoi_skip(uint8_t** s)
{
    size_t i = 0;

    while (isdigit((int)**s)) {
        i = i * 10 + **s - '0';
        (*s)++;
    }

    return i;
}

static uint8_t* escape(uint8_t* buffer)
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

    uint8_t* b = (uint8_t*)buffer;

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
                vram[pos++] = (attrs.col << 8) | ansi_to_vga[*b];
            }
            b++;
        }
    }
    update_cursor();
    return (uintptr_t)b - (uintptr_t)buffer;
}

CONSTRUCTOR rcode_t init_vga_text()
{
    return RCODE_SUCESS;
}
