#include "stdint.h"
#include "stddef.h"

#include "libc/string.h"
#include "libc/stdio.h"

#include "arch/x86/asm.h"
#include "arch/x86/paging.h"
#include "arch/x86/multiboot.h"

#include "kernel/kernel.h"
#include "kernel/kheap.h"
#include "kernel/panic.h"
#include "kernel/debug.h"
#include "kernel/mm.h"

#define PAGE_TO_PHYS(x) (x * 4 * 1024)

uint32_t page_bitmap[(1024 * 1024) / 32];
uint32_t num_pages;
uint32_t last_page;

void print_mmap()
{
    printf("\nBase Address       | Length             | Type\n");
    multiboot_mmap_t* mmap = (multiboot_mmap_t*)multiboot_ptr->mmap_addr;
    for (size_t i = 0; i < multiboot_ptr->mmap_length / sizeof(multiboot_mmap_t); i++, mmap++) {
        printf("%#.8x%.8x | %#.8x%.8x | %d\n", (uint32_t)(mmap->addr >> 32), (uint32_t)mmap->addr, (uint32_t)(mmap->length >> 32), (uint32_t)mmap->length, mmap->type);
    }
    printf("\n");
}

uint32_t alloc_page_4K()
{
    for (uint32_t i = last_page; i < num_pages; i++) {
        if (TEST_BIT(page_bitmap[i / 32], i % 32) == 0) {
            SET_BIT(page_bitmap[i / 32], i % 32);
            last_page = i + 1;
            return i;
        }
    }

    return 0;
}

void free_page_4K(uint32_t page)
{
    CLEAR_BIT(page_bitmap[page / 32], page % 32);

    if (page < last_page) {
        last_page = page;
    }
}

uint32_t alloc_page_4M()
{
    
}

void free_page_4M(uint32_t page)
{
}

void init_mm()
{

    /* mark all pages allocated by entry.asm as allocated */
    uint32_t kernel_pages = ((uint32_t)SYMBOL_VALUE(_kernel_size_a)) >> 12;
    for (uint32_t i = 0; i < kernel_pages; i++) {
        SET_BIT(page_bitmap[i / 32], i % 32);
    }

    num_pages = (multiboot_ptr->mem_upper + 1024) / 4;
    last_page = kernel_pages;

    init_paging();

    page_dir_t* mypd = create_page_dir();
    uint32_t phys = alloc_page_4K();

    map_kernel_pages(mypd);
    map_page_4k(mypd, 0, phys, PG_WRITABLE);

    activate_pd(mypd);

    int* i = NULL;
    *i = 0x36;
}