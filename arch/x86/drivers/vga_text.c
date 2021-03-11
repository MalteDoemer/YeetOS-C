#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "libc/string.h"
#include "libc/ctype.h"

#include "arch/x86/asm.h"
#include "kernel/kernel.h"

#define TAB_SIZE 4
#define SCREEN_START (0xB8000 + KERNEL_BASE)
#define SCREEN_END (SCREEN_START + 0x8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define MAX_ARGS 16

static int32_t pos = 0;
static uint8_t attrs = 0x1F;
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
    // if (pos < 0) {
    //     scroll_up(-pos / SCREEN_WIDTH + 1);
    // } else if (pos >= SCREEN_WIDTH * SCREEN_HEIGHT) {
    //     scroll_down(pos / SCREEN_WIDTH + 1);
    // }
}


static inline void show_cursor()
{
    outb(0x3D4, 0x9); // max scan line register -> set char height
    outb(0x3D5, 0x0F);

    outb(0x3D4, 11); // cursor end line
    outb(0x3D5, 0x0F);

    outb(0x3D4, 10); // cursor start line
    outb(0x3D5, 0x0E);
}

static inline void hide_cursor()
{
    outb(0x3D4, 0x9); // max scan line register -> set char height
    outb(0x3D5, 0x0F);

    outb(0x3D4, 15); // cursor end line
    outb(0x3D5, 0x0F);

    outb(0x3D4, 15); // cursor start line
    outb(0x3D5, 0x0E);
}

static inline void bell()
{
}

static inline void backspace()
{
    pos--;
    vram[pos] = (attrs << 8) | ' ';
}

static inline void tab()
{
    int num = pos % TAB_SIZE;
    while (num--) {
        vram[pos++] = (attrs << 8) | ' ';
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
    vram[pos] = (attrs << 8) | ' ';
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

        while (argc < MAX_ARGS && isdigit(*buffer)) {

            args[argc++] = atoi_skip(&buffer);

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

        } break;

        case 'K': {

        } break;

        case 'm': {

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

    if (buffer[num] != 0){
        // WARN
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
            if (*b > ' ') {
                vram[pos++] = (attrs << 8) | *b;
            }
            b++;
        }
    }
    update_cursor();
    return b - buffer;
}
