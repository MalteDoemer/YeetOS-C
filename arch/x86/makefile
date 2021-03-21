
OBJECTS=\
boot/entry.o \
kernel/init.o \
kernel/cpu.o \
kernel/multiboot.o \
kernel/paging.o \
kernel/interrupts.o \
kernel/ints.o \
kernel/exceptions.o \
kernel/idt.o \
kernel/gdt.o \


arch.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f arch.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)