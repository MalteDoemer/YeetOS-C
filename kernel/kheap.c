#include "stdint.h"
#include "stddef.h"

#include "libc/string.h"
#include "kernel/kernel.h"

#define HEAP_MAGIC 0x69365420
#define HEAP_SIZE_THRESHOLD 0

#define HEAP_SIZE 1 * 1024 * 1024
uint8_t heap_mem[HEAP_SIZE] SECTION(".heap");

typedef struct heap_block_t {
    uint32_t magic;
    struct heap_block_t* prev;
    struct heap_block_t* next;
    size_t size; // requested size
    size_t cap;  // actual capacity
} heap_block_t;

uintptr_t heap_start;
uintptr_t heap_end;
size_t heap_size;

void init_kheap()
{
    heap_start = (uintptr_t)&heap_mem;
    heap_size = sizeof(heap_mem);
    heap_end = heap_start + heap_size;

    heap_block_t* blk = (heap_block_t*)heap_start;

    blk->magic = HEAP_MAGIC;
    blk->prev = NULL;
    blk->next = NULL;

    blk->size = 0;
    blk->cap = heap_size - sizeof(heap_block_t);
}

void* kmalloc(size_t size)
{
    heap_block_t* block = (heap_block_t*)heap_start;

    size_t cap = ALIGN(size, sizeof(void*)) + HEAP_SIZE_THRESHOLD;

    while (block && (block->size || block->cap < size))
        block = block->next;

    if (!block)
        return NULL;

    if (block->cap > cap + sizeof(heap_block_t)) {
        heap_block_t* temp = (heap_block_t*)((uintptr_t)block + sizeof(heap_block_t) + cap);

        temp->size = 0;
        temp->magic = HEAP_MAGIC;
        temp->next = block->next;
        temp->cap = block->cap - sizeof(heap_block_t) - cap;

        heap_block_t* next = block->next;
        if (next) {
            next->prev = temp;
        }

        block->next = temp;
        temp->prev = block;
        block->cap = cap;
    }

    block->size = size;

    return (void*)((uintptr_t)block + sizeof(heap_block_t));
}

void kfree(void* ptr)
{
    heap_block_t* block = (heap_block_t*)((uintptr_t)ptr - sizeof(heap_block_t));

    if ((uintptr_t)block < heap_start || (uintptr_t)block > heap_end)
        return;

    if (block->magic != HEAP_MAGIC)
        return;

    block->size = 0;

    if (block->prev && block->prev->size == 0) {
        block->prev->next = block->next;
        block->prev->cap += block->cap + sizeof(heap_block_t);

        if (block->next) {
            block->next->prev = block->prev;
        }

        block = block->prev;
    }

    if (block->next && block->next->size == 0) {
        block->cap += block->next->cap + sizeof(heap_block_t);
        block->next = block->next->next;

        if (block->next) {
            block->next->prev = block;
        }
    }
}

void* kcmalloc(size_t num, size_t size)
{
    void* ptr = kmalloc(size * num);

    if (ptr) {
        memset(ptr, 0, size * num);
    }

    return ptr;
}

void* krealloc(void* ptr, size_t newsize)
{
    heap_block_t* block = (heap_block_t*)((uintptr_t)ptr - sizeof(heap_block_t));

    if ((uintptr_t)block < heap_start || (uintptr_t)block > heap_end) {
        return kmalloc(newsize);
    }

    if (block->magic != HEAP_MAGIC) {
        return kmalloc(newsize);
    }

    if (block->cap >= newsize) {
        block->size = newsize;
        return ptr;
    } else {
        void* newptr = kmalloc(newsize);
        if (newptr) {
            memcpy(newptr, ptr, MIN(newsize, block->size));
        }
        kfree(ptr);
        return newptr;
    }
}