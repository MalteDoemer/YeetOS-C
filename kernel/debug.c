
#include "stddef.h"
#include "stdint.h"
#include "stdarg.h"

#include "libc/stdio.h"
#include "kernel/debug.h"

int debugf(const char* fmt, ...)
{
    extern size_t serial_write(char* buffer, size_t num);

    char buf[1024];
    va_list va;
    va_start(va, fmt);
    size_t chars = vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);

    // num is the size of the buffer including zero terminator
    return (int)serial_write(buf, chars + 1);
}

int vdebugf(const char* fmt, va_list va)
{
    extern size_t serial_write(char* buffer, size_t num);

    char buf[1024];
    size_t chars = vsnprintf(buf, sizeof(buf), fmt, va);

    // num is the size of the buffer including zero terminator
    return (int)serial_write(buf, chars + 1);
}