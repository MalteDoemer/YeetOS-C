#include "stdint.h"
#include "stddef.h"

#include "arch/x86/asm.h"
#include "arch/x86/paging.h"

#include "libc/string.h"

#include "kernel/kernel.h"
#include "kernel/mm.h"

#define NUM_PAGE_STRUCTS 1024
#define PAGE_STRUCT_SIZE (4 * 1024)

uint32_t pt_bitmap[NUM_PAGE_STRUCTS / 32];
uint8_t page_structs[NUM_PAGE_STRUCTS * PAGE_STRUCT_SIZE] SECTION(".page_structs");

page_dir_t* kernel_pd;

void* alloc_page_struct()
{
    for (size_t i = 0; i < NUM_PAGE_STRUCTS; i++) {
        if (TEST_BIT(pt_bitmap[i / 8], i % 8) == 0) {
            SET_BIT(pt_bitmap[i / 8], i % 8);
            return page_structs + i * PAGE_STRUCT_SIZE;
        }
    }
    return NULL;
}

void free_page_struct(void* ptr)
{
    size_t i = ((uintptr_t)ptr - (uintptr_t)page_structs) / PAGE_STRUCT_SIZE;
    if (i < NUM_PAGE_STRUCTS) {
        CLEAR_BIT(pt_bitmap[i / 8], i % 8);
    }
}

page_dir_t* create_page_dir()
{
    page_dir_t* pd = alloc_page_struct();

    if (pd) {
        stosd(pd, 0, sizeof(page_dir_t) / 4);
    }

    return pd;
}

void free_page_dir(page_dir_t* pd)
{
    for (size_t i = 0; i < 1024; i++) {
        if (pd->entries[i].p && !pd->entries[i].s) {
            page_table_t* table = (page_table_t*)((pd->entries[i].address << 12) + KERNEL_BASE);
            stosd(table, 0, sizeof(page_table_t) / 4);
            free_page_struct(table);
        }
    }

    stosd(pd, 0, sizeof(page_dir_t) / 4);
    free_page_struct(pd);
}

void map_page_4K(page_dir_t* pd, uintptr_t virtual, uintptr_t physical, pg_flags_t flags)
{
    uintptr_t pdindex;
    uintptr_t ptindex;
    page_table_t* table;

    pdindex = virtual >> 22;
    ptindex = virtual >> 12 & 0x03FF;

    if (pd->entries[pdindex].p) {
        // TODO error checking
        table = (page_table_t*)((pd->entries[pdindex].address << 12) + KERNEL_BASE);
    } else {
        // TODO handle out of memory
        table = alloc_page_struct();
        memset(table, 0, sizeof(page_table_t));

        pd->entries[pdindex].address = ((uintptr_t)table - KERNEL_BASE) >> 12;
        pd->entries[pdindex].p = 1;

        // FIXME is this right ?
        pd->entries[pdindex].r = 1;
        pd->entries[pdindex].u = 1;
    }

    table->entries[ptindex].p = 1;
    table->entries[ptindex].r = flags & PG_WRITABLE;
    table->entries[ptindex].u = flags & PG_USER;
    table->entries[ptindex].address = physical >> 12;
}

void unmap_page_4K(page_dir_t* pd, uintptr_t virtual)
{
    uintptr_t pdindex;
    uintptr_t ptindex;
    page_table_t* table;

    pdindex = virtual >> 22;
    ptindex = virtual >> 12 & 0x03FF;

    if (!pd->entries[pdindex].p && !pd->entries[pdindex].s)
        return;

    // TODO error checking
    table = (page_table_t*)((pd->entries[pdindex].address << 12) + KERNEL_BASE);

    memset(&table->entries[ptindex], 0, sizeof(pt_entry_t));

    // check if the page table is empty
    for (uint32_t i = 0; i < 1024; i++) {
        if (table->entries[i].p) {
            return;
        }
    }

    free_page_struct(table);
    memset(&pd->entries[pdindex], 0, sizeof(pd_entry_t));
}

void map_page_4M(page_dir_t* pd, uintptr_t virtual, uintptr_t physical, pg_flags_t flags)
{
    uintptr_t pdindex;
    pdindex = virtual >> 22;

    // TODO check if page already exists

    pd->entries[pdindex].p = 1;
    pd->entries[pdindex].s = 1;
    pd->entries[pdindex].r = flags & PG_WRITABLE;
    pd->entries[pdindex].u = flags & PG_USER;
    pd->entries[pdindex].address = physical >> 12;
}

void unmap_page_4M(page_dir_t* pd, uintptr_t virtual)
{
    uintptr_t pdindex;
    pdindex = virtual >> 22;

    // TODO check if page is really 4 MiB

    memset(&pd->entries[pdindex], 0, sizeof(pd_entry_t));
}

void map_kernel_pages(page_dir_t* pd)
{
    for (uint32_t i = 0; i < 1024; i++) {
        if (kernel_pd->entries[i].p) {
            pd->entries[i] = kernel_pd->entries[i];
        }
    }
}

void activate_pd(page_dir_t* pd)
{
    set_cr3((uint32_t)pd - KERNEL_BASE);
}

void init_paging()
{
    kernel_pd = (page_dir_t*)&page_structs[0];
}