#include "kernel.h"

tss_t tss;
gdt_desc_t gdt_desc;
gdt_entry_t gdt[6];

// gdt_entry_t gdt[] = {
//
//     // null descriptor
//     {
//         limit1 : 0,
//         base1 : 0,
//         base2 : 0,
//         access : 0,
//         limit2 : 0,
//         flags : 0,
//         base3 : 0
//     },
//
//     // kernel code descriptor
//     {
//         limit1 : 0xFFFF,
//         base1 : 0,
//         base2 : 0,
//         access : 0b10011010,
//         limit2 : 0xF,
//         flags : 0b1100,
//         base3 : 0
//     },
//
//     // kernel data descriptor
//     {
//         limit1 : 0xFFFF,
//         base1 : 0,
//         base2 : 0,
//         access : 0b10010010,
//         limit2 : 0xF,
//         flags : 0b1100,
//         base3 : 0
//     },
//
//     // user code descriptor
//     {
//         limit1 : 0xFFFF,
//         base1 : 0,
//         base2 : 0,
//         access : 0b11111010,
//         limit2 : 0xF,
//         flags : 0b1100,
//         base3 : 0
//     },
//
//     // user data descriptor
//     {
//         limit1 : 0xFFFF,
//         base1 : 0,
//         base2 : 0,
//         access : 0b11110010,
//         limit2 : 0xF,
//         flags : 0b1100,
//         base3 : 0
//     },
//
//     // tss descriptor
//     {
//         limit1 : sizeof(tss) & 0xFFFF,
//         base1 : 0,
//         base2 : 0,
//         access : 0b11101001,
//         limit2 : (sizeof(tss) >> 16) & 0xF,
//         flags : 0,
//         base3 : 0
//     }
// };

void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    gdt[index].base1 = base & 0xFFFF;
    gdt[index].base2 = (base >> 16) & 0xFF;
    gdt[index].base3 = (base >> 24) & 0xFF;

    gdt[index].limit1 = limit & 0xFFFF;
    gdt[index].limit2 = (limit >> 16) & 0xF;

    gdt[index].access = access;
    gdt[index].flags = flags & 0xF;
}

void init_gdt()
{
    memset(&tss, 0, sizeof(tss));
    memset(&gdt, 0, sizeof(gdt));

    set_gdt_entry(1, 0, 0xFFFFF, 0x9A, 0xC);
    set_gdt_entry(2, 0, 0xFFFFF, 0x92, 0xC);
    set_gdt_entry(3, 0, 0xFFFFF, 0xFA, 0xC);
    set_gdt_entry(4, 0, 0xFFFFF, 0xF2, 0xC);
    set_gdt_entry(5, (uint32_t)&tss, sizeof(tss), 0xE9, 0);

    
    gdt_desc.addr = (uint32_t)&gdt;
    gdt_desc.size = sizeof(gdt) - 1;

    load_gdt(&gdt_desc);
}