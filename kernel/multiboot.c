#include "kernel.h"

static mboot_info_t* mbi;

mboot_info_t* get_mboot_info()
{
    return mbi;
}

void init_multiboot(mboot_info_t* multiboot_info)
{
    uint32_t flags;

    mbi = multiboot_info;
    flags = mbi->flags;

    if (flags & MULTIBOOT_FLAGS_CMDLINE)
        mbi->cmdline += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_MODULES) {
        mbi->mods_addr += KERNEL_BASE;

        mboot_mod_t* module = (mboot_mod_t*)mbi->mods_addr;

        for (uint32_t i = 0; i < mbi->mods_count; i++) {
            module[i].mod_start += KERNEL_BASE;
            module[i].mod_end += KERNEL_BASE;
            if (module[i].string)
                module[i].string += KERNEL_BASE;
        }
    }

    if (flags & MULTIBOOT_FLAGS_MMAP)
        mbi->mmap_addr += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_DRIVES)
        mbi->drives_addr += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_CONFIG)
        mbi->config_table += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_NAME)
        mbi->boot_loader_name += KERNEL_BASE;
}