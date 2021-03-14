#ifndef MM_H
#define MM_H

#include "stdint.h"
#include "stddef.h"

#define MM_FLAGS_RDONLY 1
#define MM_FLAGS_EXEC 2
#define MM_FLAGS_KERNEL 4


#define MM_OUT_OF_MEMORY 1
#define MM_INVALID_VADDR 2
#define MM_INVALID_FLAGS 3

/* Architectur specific representation for paging */
typedef void* paging_desc_t;

/* Initializes the memory manager */
void init_mm();

/* Allocates and maps physical memory to start_addr -> end_addr in the virtual address space */
int map_virtual_mem(paging_desc_t pd, uintptr_t start_addr, uintptr_t end_addr, uint32_t flags);

/* Makes pd the currently used paging descriptor */
void enable_desc(paging_desc_t pd);


#endif // #ifndef MM_H
