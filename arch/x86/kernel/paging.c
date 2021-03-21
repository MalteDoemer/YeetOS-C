#include "stdint.h"
#include "stddef.h"

#include "kernel/kernel.h"

#define NUM_PAGE_STRUCTS 1024
#define PAGE_STRUCT_SIZE (4 * 1024)

uint32_t pt_bitmap[NUM_PAGE_STRUCTS / 32];
uint8_t page_structs[NUM_PAGE_STRUCTS * PAGE_STRUCT_SIZE] SECTION(".page_structs");