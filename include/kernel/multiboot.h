#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "stdint.h"
#include "kernel/kernel.h"

#define MULTIBOOT_FLAGS_MEMORY (1 << 0)
#define MULTIBOOT_FLAGS_DECICE (1 << 1)
#define MULTIBOOT_FLAGS_CMDLINE (1 << 2)
#define MULTIBOOT_FLAGS_MODULES (1 << 3)
#define MULTIBOOT_FLAGS_AOUT (1 << 4)
#define MULTIBOOT_FLAGS_ELF (1 << 5)
#define MULTIBOOT_FLAGS_MMAP (1 << 6)
#define MULTIBOOT_FLAGS_DRIVES (1 << 7)
#define MULTIBOOT_FLAGS_CONFIG (1 << 8)
#define MULTIBOOT_FLAGS_NAME (1 << 9)
#define MULTIBOOT_FLAGS_APM (1 << 10)
#define MULTIBOOT_FLAGS_VBE (1 << 11)
#define MULTIBOOT_FLAGS_FRAME_BUF (1 << 12)

#define MULTIBOOT_MAGIC 0x2BADB002

typedef struct multiboot_info_t {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;

    uint32_t elf_num;
    uint32_t elf_size;
    uint32_t elf_addr;
    uint32_t elf_shndx;

    uint32_t mmap_length;
    uint32_t mmap_addr;

    uint32_t drives_length;
    uint32_t drives_addr;

    uint32_t config_table;
    uint32_t boot_loader_name;

    uint32_t apm_table;

    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;

    uint8_t framebuffer_type;

    union {
        struct
        {
            uint32_t framebuffer_palette_addr;
            uint16_t framebuffer_palette_num_colors;
        } PACKED;
        struct
        {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        } PACKED;
    } PACKED;

} PACKED multiboot_info_t;

typedef struct multiboot_mmap_t {
    uint32_t size;
    uint64_t addr;
    uint64_t length;
    uint32_t type;
} PACKED multiboot_mmap_t;

typedef struct multiboot_mod_t {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
} PACKED multiboot_mod_t;

extern multiboot_info_t* multiboot_ptr;
extern uint32_t* multiboot_sig;

void init_multiboot();

#endif // #ifndef MULTIBOOT_H