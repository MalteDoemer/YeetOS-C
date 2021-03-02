#include "kernel.h"

uint32_t page_bitmap[0x8000];

uint32_t page_start;
uint32_t page_end;

uint32_t last_page;


uint32_t alloc_page_frame() 
{
    for (uint32_t i = last_page; i < page_end; i++)
    {
        if (TEST_BIT(page_bitmap[i / 32], i & 0b11111))
        {
            SET_BIT(page_bitmap[i / 32], i & 0b11111);
            last_page = i;
            return i;
        }
    }

    return -1;
}

void free_page_frame(uint32_t page_frame) 
{

}

bool init_mem()
{
    mboot_info_t* mboot_info = get_mboot_info();

    if (!(mboot_info->flags & MULTIBOOT_FLAGS_MEMORY))
        return false;

    printf("Found %u MiB of upper memory\n", mboot_info->mem_upper / 1024);

    // The first 8-MiB are used by the Kernel: 4-MiB for code and data and 4-MiB for page tables and page directories
    // that implies the first page for user memory is page 0x800
    page_start = 0x800;

    // mem_upper is in 1-KiB units and starts at 1-MiB
    page_end = (mboot_info->mem_upper + 1024) / 4;

    last_page = page_start;

    memset(page_bitmap, 0, sizeof(page_bitmap));

    return true;
}