#ifndef KERNEL_H
#define KERNEL_H

#define KERNEL_BASE 0xC0000000

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

#define SYMBOL_VALUE(x) ({ extern void x(); &x; })

void kernel_main();

#endif // #ifndef KERNEL_H
