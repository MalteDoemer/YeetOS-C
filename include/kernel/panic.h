#ifndef PANIC_H
#define PANIC_H

#include "libc/stdio.h"
#include "kernel/kernel.h"

#define WARN(x, __VA_ARGS) warn(__func__, __FILE__, __LINE__, x, __VA_ARGS)
#define PANIC(x, __VA_ARGS) panic(__func__, __FILE__, __LINE__, x, __VA_ARGS)

void warn(const char* func, const char* file, int line, const char* msg, ...);
void panic(const char* func, const char* file, int line, const char* msg, ...) NORETURN;

#endif // #ifndef PANIC_H
