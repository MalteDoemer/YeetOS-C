#include "kernel.h"


uint32_t page_bitmap[0x8000];

uint32_t page_start;
uint32_t page_end;

bool needs_serach;
uint32_t last_page;


bool init_mem()
{
    mboot_info_t* mboot_info = get_mboot_info();

    if (!(mboot_info->flags & MULTIBOOT_FLAGS_MEMORY))
        return false;

    printf("Found %u MiB of upper memory\n", mboot_info->mem_upper / 1024);


    // The first 8-MiB are used by the Kernel 4-MiB for code and data and 4-MiB for page tables and page directories


    // that implies the first page for user memory is page 0x800
    page_start = 0x800; 

    // mem_upper is in 1-KiB units and starts at 1-MiB
    page_end = (mboot_info->mem_upper + 1024) / 4;

    printf("%x, %x\n", page_start, page_end);


    return true;
}