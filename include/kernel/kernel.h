#ifndef KERNEL_H
#define KERNEL_H

#define KERNEL_BASE 0xC0000000
#define ALIGN(x,n) ((x + (n) - 1) & ~((n) - 1))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define PACKED __attribute__((packed))
#define NORETURN __attribute__((noreturn))
#define MALLOC __attribute((malloc))

void init_arch();
void kernel_main();

#endif // #ifndef KERNEL_H
