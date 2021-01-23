#include "kernel.h"

static tss_t tss;

static gdt_entry_t gdt[] = {

    // null descriptor
    {
        limit1 : 0,
        base1 : 0,
        base2 : 0,
        access : 0,
        limit2 : 0,
        flags : 0,
        base3 : 0
    },

    // kernel code descriptor
    {
        limit1 : 0xFFFF,
        base1 : 0,
        base2 : 0,
        access : 0b10011010,
        limit2 : 0xF,
        flags : 0b1100,
        base3 : 0
    },

    // kernel data descriptor
    {
        limit1 : 0xFFFF,
        base1 : 0,
        base2 : 0,
        access : 0b10010010,
        limit2 : 0xF,
        flags : 0b1100,
        base3 : 0
    },

    // user code descriptor
    {
        limit1 : 0xFFFF,
        base1 : 0,
        base2 : 0,
        access : 0b11111010,
        limit2 : 0xF,
        flags : 0b1100,
        base3 : 0
    },

    // user data descriptor
    {
        limit1 : 0xFFFF,
        base1 : 0,
        base2 : 0,
        access : 0b11110010,
        limit2 : 0xF,
        flags : 0b1100,
        base3 : 0
    },

    // tss descriptor
    {
        limit1 : sizeof(tss) & 0xFFFF,
        base1 : 0,
        base2 : 0,
        access : 0b11101001,
        limit2 : (sizeof(tss) >> 16) & 0xF,
        flags : 0,
        base3 : 0
    }
};

void init_gdt()
{
    
    uint32_t tss_addr = (uint32_t)(&tss);
    gdt[5].base1 = tss_addr & 0xFFFF;
    gdt[5].base2 = (tss_addr >> 16) & 0xFF;
    gdt[5].base3 = (tss_addr >> 24) & 0xFF;
}