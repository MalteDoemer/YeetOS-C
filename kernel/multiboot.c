#include "kernel/kernel.h"

void init_multiboot()
{
    uint32_t flags;

    flags = multiboot_ptr->flags;

    if (flags & MULTIBOOT_FLAGS_CMDLINE)
        multiboot_ptr->cmdline += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_MODULES) {
        multiboot_ptr->mods_addr += KERNEL_BASE;

        multiboot_mod_t* module = (multiboot_mod_t*)multiboot_ptr->mods_addr;

        for (uint32_t i = 0; i < multiboot_ptr->mods_count; i++) {
            module[i].mod_start += KERNEL_BASE;
            module[i].mod_end += KERNEL_BASE;
            if (module[i].string)
                module[i].string += KERNEL_BASE;
        }
    }

    if (flags & MULTIBOOT_FLAGS_MMAP)
        multiboot_ptr->mmap_addr += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_DRIVES)
        multiboot_ptr->drives_addr += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_CONFIG)
        multiboot_ptr->config_table += KERNEL_BASE;

    if (flags & MULTIBOOT_FLAGS_NAME)
        multiboot_ptr->boot_loader_name += KERNEL_BASE;
}

multiboot_info_t* multiboot_ptr;
uint32_t* multiboot_sig;