#ifndef KERNEL_H
#define KERNEL_H

#define SET_BIT(A,B) (A = (A | (1 << (B))))
#define CLEAR_BIT(A,B) (A = (A & ~(1 << (B))))
#define TOGGLE_BIT(A,B) (A = (A ^ (1 << (B))))
#define TEST_BIT(A,B) (A & (1 << (B)))

#define ALIGN(x, n) ((x + (n)-1) & ~((n)-1))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define PACKED __attribute__((packed))
#define NORETURN __attribute__((noreturn))
#define MALLOC __attribute__((malloc))
#define CONSTRUCTOR __attribute__((constructor))
#define ALIGNED(x) __attribute__((aligned(x)))
#define SECTION(x) __attribute__((section(x)))

#define SYMBOL_VALUE(x) ({ extern char x; &x; })

typedef enum rcode_t {
   RCODE_SUCESS = 0,
   RCODE_FAILURE,
} rcode_t;

void kernel_main();

#if ARCH == x86
#include "arch/x86/arch.h"
#else 
#error "unkonw architecture"
#endif

#endif // #ifndef KERNEL_H
