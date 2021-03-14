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

void init_paging();

page_dir_t* create_page_dir();
void free_page_dir(page_dir_t* pd);

int map_page(page_dir_t* pd, uintptr_t virtual, uintptr_t physical, bool user, bool read_write, bool large);
void unmap_page(page_dir_t* pd, uintptr_t virtual);

#endif // #ifndef PAGING_H
