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

void init_paging()
{
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

int map_page(page_dir_t* pd, uintptr_t virtual, uintptr_t physical, bool user, bool read_write, bool large)
{
    uintptr_t pdindex = virtual >> 22;

    if (large) {
        pd->entries[pdindex].s = 1;
        pd->entries[pdindex].p = 1;
        pd->entries[pdindex].r = read_write;
        pd->entries[pdindex].u = user;

        pd->entries[pdindex].w = 0;
        pd->entries[pdindex].d = 0;
        pd->entries[pdindex].a = 0;
        pd->entries[pdindex].g = 0;
        pd->entries[pdindex].zero = 0;

        pd->entries[pdindex].address = physical >> 12;

    } else {
        page_table_t* table;

        // TODO error checking
        if (pd->entries[pdindex].p) {
            table = (page_table_t*)((pd->entries[pdindex].address << 12) + KERNEL_BASE);
        } else {
            table = alloc_page_struct();
            // TODO handle out of memory
            stosd(table, 0, sizeof(page_table_t) / 4);

            pd->entries[pdindex].address = ((uintptr_t)table - KERNEL_BASE) >> 12;
            pd->entries[pdindex].p = 1;

            // FIXME is this right ?
            pd->entries[pdindex].r = 1;
            pd->entries[pdindex].u = 1;
        }

        uintptr_t ptindex = virtual >> 12 & 0x03FF;

        table->entries[ptindex].p = 1;
        table->entries[ptindex].r = read_write;
        table->entries[ptindex].u = user;

        table->entries[ptindex].w = 0;
        table->entries[ptindex].c = 0;
        table->entries[ptindex].a = 0;
        table->entries[ptindex].d = 0;
        table->entries[ptindex].g = 0;
        table->entries[ptindex].zero = 0;

        table->entries[ptindex].address = physical >> 12;
    }

    return 0;
}

void unmap_page(page_dir_t* pd, uintptr_t virtual)
{
    uintptr_t pdindex = virtual >> 22;

    if (!pd->entries[pdindex].p)
        return;

    if (!pd->entries[pdindex].s) {
        page_table_t* table = (page_table_t*)((pd->entries[pdindex].address << 12) + KERNEL_BASE);
        stosd(table, 0, sizeof(page_table_t) / 4);
        free_page_struct(table);
    }

    *(uint32_t*)(&pd->entries[pdindex]) = 0;

    invlpg(virtual);
}
