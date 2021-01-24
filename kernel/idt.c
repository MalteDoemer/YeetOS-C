#include "kernel.h"

idtr_t idtr;
idt_entry_t idt[256];

void init_idt()
{

    memset(idt, 0, sizeof(idt));

    idtr.size = sizeof(idt) - 1;
    idtr.addr = (uint32_t)&idt;

    load_idt(&idtr);
}