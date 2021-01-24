#include "kernel.h"

tss_t tss;
gdtr_t gdtr;
gdt_entry_t gdt[6];

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

    gdtr.addr = (uint32_t)&gdt;
    gdtr.size = sizeof(gdt) - 1;

    load_gdt(&gdtr);
}