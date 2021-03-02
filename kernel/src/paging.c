#include "kernel.h"

extern uint8_t __page_structs_start;
extern uint8_t __page_structs_end;

void* page_structs_start = &__page_structs_start;
void* page_structs_end = &__page_structs_end;

void* alloc_page_struct()
{
    
}

void free_page_struct(void* page_struct)
{
    if (page_struct >= page_structs_start && page_struct < page_structs_end && ((uint32_t)page_struct & 0xFFF) == 0) {
    }
}
