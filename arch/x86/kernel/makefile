
OBJECTS=\
init.o \
cpu.o \
multiboot.o \
paging.o \
interrupts.o \
ints.o \
exceptions.o \
idt.o \
gdt.o \


kernel.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f kernel.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)