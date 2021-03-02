#include "kernel.h"

idtr_t idtr;
idt_entry_t idt[256];

extern isr_stub_t isr_stub_table[256];

void remap_pic()
{
    // start initialization
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // set the vector address (int 0x20 - 0x2F)
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // pic1 is master and pic2 is slave
    outb(0x21, 4);
    outb(0xA1, 2);

    // 8086/8088 mode
    outb(0x21, 0x1);
    outb(0xA1, 0x1);

    // disable all interrupts
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

void mask_irq(uint8_t irq, bool mask)
{
    uint8_t pic1_mask = inb(0x21);
    uint8_t pic2_mask = inb(0xA1);

    if (irq < 8) {
        pic1_mask = mask ? pic1_mask | (1 << irq) : pic1_mask & ~(1 << irq);
    } else {
        pic2_mask = mask ? pic2_mask | (1 << (irq - 8)) : pic2_mask & ~(1 << (irq - 8));
    }

    outb(0x21, pic1_mask);
    outb(0xA1, pic2_mask);
}

bool init_idt()
{
    remap_pic();

    idtr.size = sizeof(idt) - 1;
    idtr.addr = (uint32_t)&idt;

    for (int i = 0; i < 256; i++) {
        idt[i].offset1 = ((uint32_t)&isr_stub_table[i]) & 0xFFFF;
        idt[i].offset2 = (((uint32_t)&isr_stub_table[i]) >> 16) & 0xFFFF;
        idt[i].selector = KERNEL_CODE_DESC;
        idt[i].type = 0x8E;
        idt[i].zero = 0;
    }

    load_idt(&idtr);

    return true;
}