#ifndef PAGING_H
#define PAGING_H

#include "stdint.h"
#include "stdbool.h"

#include "kernel/kernel.h"

typedef struct pd_entry_t {
    uint8_t p : 1;
    uint8_t r : 1;
    uint8_t u : 1;
    uint8_t w : 1;
    uint8_t d : 1;
    uint8_t a : 1;
    uint8_t zero : 1;
    uint8_t s : 1;
    uint8_t g : 1;
    uint8_t available : 3;
    uint32_t address : 20;
} PACKED pd_entry_t;

typedef struct pt_entry_t {
    uint8_t p : 1;
    uint8_t r : 1;
    uint8_t u : 1;
    uint8_t w : 1;
    uint8_t c : 1;
    uint8_t a : 1;
    uint8_t d : 1;
    uint8_t zero : 1;
    uint8_t g : 1;
    uint8_t available : 3;
    uint32_t address : 20;
} PACKED pt_entry_t;

typedef struct page_table_t {
    pt_entry_t entries[1024];
} PACKED page_table_t;

typedef struct page_dir_t {
    pd_entry_t entries[1024];
} PACKED page_dir_t;

typedef enum pg_flags_t {
    PG_USER,
    PG_WRITABLE,
    PG_EXECUTABLE,
} pg_flags_t;

rcode_t init_paging();

page_dir_t* create_page_dir();
void free_page_dir(page_dir_t* pd);

void map_kernel_pages(page_dir_t* pd);

void map_page_4K(page_dir_t* pd, uintptr_t virtual, uintptr_t physical, pg_flags_t flags);
void unmap_page_4K(page_dir_t* pd, uintptr_t virtual);

void map_page_4M(page_dir_t* pd, uintptr_t virtual, uintptr_t physical, pg_flags_t flags);
void unmap_page_4M(page_dir_t* pd, uintptr_t virtual);


void activate_pd(page_dir_t* pd);

#endif // #ifndef PAGING_H
