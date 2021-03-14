
OBJECTS=\
entry.o \
setup.o \
multiboot.o \
cpu.o \
gdt.o \
idt.o \
ints.o \
interrupts.o \
exceptions.o \
paging.o \
mm.o \

kernel.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f kernel.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)