#include "stdint.h"
#include "stddef.h"

#include "kernel/kernel.h"

#define HEAP_MAGIC 0x69365420

typedef struct block_t {
    uint32_t magic;
    struct block_t* prev;
    struct block_t* next;
    size_t size; // requested size
    size_t cap; // actual capacity
} block_t;




