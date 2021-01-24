#ifndef IDT_H
#define IDT_H

typedef struct idt_entry_t {
    uint16_t offset1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t offset2;
} __attribute__((packed, aligned(8))) idt_entry_t;

typedef struct idtr_t {
    uint16_t size;
    uint32_t addr;
} __attribute__((packed, aligned(8))) idtr_t;

void init_idt();
void load_idt(idtr_t* idtr);

#endif // #ifndef IDT_H
