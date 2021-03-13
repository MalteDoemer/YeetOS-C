#ifndef ASSERT_H
#define ASSERT_H

#include "kernel/panic.h"

#ifndef NDEBUG

#ifdef __KERNEL__
#define assert(x) if (!(x)) PANIC("Assertion failed: %s\n", #x)
#else 
#error "no assert macro for usermode"
#endif // #ifdef __KERNEL__

#else

#define assert(x) (void)

#endif // #ifdef NDEBUG

#endif // #ifndef ASSERT_H
