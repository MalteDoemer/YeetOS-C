#include "stdio.h"


#include "kernel/kernel.h"
#include "kernel/panic.h"
#include "kernel/cpu.h"
#include "kernel/debug.h"

void warn(const char* func, const char* file, int line, const char* msg, ...)
{
    debugf("Warning: %s:%d in %s():\n", file, line, func);
    va_list va;
    va_start(va, msg);
    vdebugf(msg, va);
    va_end(va);
}

void panic(const char* func, const char* file, int line, const char* msg, ...)
{
    printf("\033[31;2;40m\033[J\033[H");
    printf("PANIC: %s:%d in %s():\n", file, line, func);
    va_list va;
    va_start(va, msg);
    vprintf(msg, va);
    va_end(va);
    for (;;) halt();
}
