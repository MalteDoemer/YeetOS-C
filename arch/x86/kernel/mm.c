#include "stdint.h"
#include "stddef.h"

#include "arch/x86/asm.h"
#include "arch/x86/paging.h"

#include "kernel/kernel.h"
#include "kernel/mm.h"

void init_mm()
{
    init_paging();
}

int map_virtual_mem(paging_desc_t pd, uintptr_t start_addr, uintptr_t end_addr, uint32_t flags)
{
}

void enable_desc(paging_desc_t pd)
{
    __asm("movl %%eax, %%cr3" ::"a"(pd));
}
