#ifndef GDT_H
#define GDT_H

#include "stdint.h"

#include "kernel/kernel.h"

#define KERNEL_CODE_DESC 0x08
#define KERNEL_DATA_DESC 0x10
#define USER_CODE_DESC 0x1B
#define USER_DATA_DESC 0x23
#define TSS_DESC 0x2B


typedef struct gdt_entry_t {
    uint16_t limit1;
    uint16_t base1;
    uint8_t base2;
    uint8_t access;
    uint8_t limit2 : 4;
    uint8_t flags : 4;
    uint8_t base3;
} __attribute__((packed, aligned(8))) gdt_entry_t;

typedef struct gdtr_t {
    uint16_t size;
    uint32_t addr;
} __attribute__((packed, aligned(8))) gdtr_t;

typedef struct tss_t {
    uint32_t link;
    uint32_t esp0;
    uint32_t ss0;

    uint32_t esp1;
    uint32_t ss1;

    uint32_t esp2;
    uint32_t ss2;

    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldtr;
    uint32_t iopb;

} __attribute__((packed, aligned(16))) tss_t;

void init_gdt();

void load_gdt(gdtr_t* gdtr);
void load_tss(uint16_t desc);

void set_esp0(uint32_t esp0);

#endif // #ifndef GDT_H
