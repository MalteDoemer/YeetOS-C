#ifndef KHEAP_H
#define KHEAP_H

#include "stdint.h"
#include "stddef.h"

#include "kernel/kernel.h"

rcode_t init_kheap();

void* kmalloc(size_t size) MALLOC;
void* kcmalloc(size_t num, size_t size) MALLOC;
void* krealloc(void* ptr, size_t newsize) MALLOC;
void kfree(void* ptr);

#endif // #ifndef KHEAP_H
