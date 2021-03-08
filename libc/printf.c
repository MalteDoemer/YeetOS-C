#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"

int printf(char* fmt, ...)
{
#ifdef __KERNEL__
    extern void vga_text_write(char* buffer, size_t num);

    char buf[1024];

    va_list va;
    va_start(va, fmt);
    size_t chars = vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);

    vga_text_write(buf, chars);


#endif
}