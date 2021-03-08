#ifndef KERNEL_H
#define KERNEL_H

#define KERNEL_BASE 0xC0000000
#define PACKED __attribute__((packed))

void init_arch();
void kernel_main();

#endif // #ifndef KERNEL_H
