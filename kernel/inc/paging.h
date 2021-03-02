#ifndef PAGING_H
#define PAGING_H

#include "stdint.h"

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
} __attribute__((packed)) pd_entry_t;

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
} __attribute__((packed)) pt_entry_t;

void* alloc_page_struct();
void free_page_struct(void* page_struct);

#endif // #ifndef PAGING_H
