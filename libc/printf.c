#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"
#include "libc/stdio.h"

int printf(const char* fmt, ...)
{
#ifdef __KERNEL__
    extern size_t vga_text_write(char* buffer, size_t num);

    char buf[1024];

    va_list va;
    va_start(va, fmt);
    size_t chars = vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);

    // num is the size of the buffer including zero terminator
    return (int)vga_text_write(buf, chars + 1);
#endif
}

int vprintf(const char* fmt, va_list va)
{
#ifdef __KERNEL__
    extern size_t vga_text_write(char* buffer, size_t num);

    char buf[1024];

    size_t chars = vsnprintf(buf, sizeof(buf), fmt, va);

    // num is the size of the buffer including zero terminator
    return (int)vga_text_write(buf, chars + 1);

#endif
}