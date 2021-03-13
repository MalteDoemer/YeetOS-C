#ifndef DEBUG_H
#define DEBUG_H

#include "stdarg.h"

int debugf(const char* fmt, ...);
int vdebugf(const char* fmt, va_list va);

#endif // #ifndef DEBUG_H
