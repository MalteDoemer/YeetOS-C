#include "stdint.h"
#include "stddef.h"

#include "libc/stdio.h"

#include "arch/x86/asm.h"
#include "arch/x86/paging.h"
#include "arch/x86/multiboot.h"

#include "kernel/kernel.h"
#include "kernel/kheap.h"
#include "kernel/panic.h"
#include "kernel/debug.h"
#include "kernel/mm.h"

uint32_t page_bitmap[(1024 * 1024) / 32] SECTION(".page_bitmap");

void print_mmap()
{
    printf("\nBase Address       | Length             | Type\n");
    multiboot_mmap_t* mmap = (multiboot_mmap_t*)multiboot_ptr->mmap_addr;
    for (size_t i = 0; i < multiboot_ptr->mmap_length / sizeof(multiboot_mmap_t); i++, mmap++) {
        printf("%#.8x%.8x | %#.8x%.8x | %d\n", (uint32_t)(mmap->addr >> 32), (uint32_t)mmap->addr, (uint32_t)(mmap->length >> 32), (uint32_t)mmap->length, mmap->type);
    }
    printf("\n");
}

void init_mm()
{
    init_paging();
}