CC = i686-elf-gcc
LD = i686-elf-ld
ASM = nasm -f elf32

ASM_FLAGS=-g

CC_FLAGS=\
-ggdb \
-ffreestanding \
-nostdlib \
-std=c99 \
-I inc
# -Wno-packed-bitfield-compat

LD_FLAGS= -m elf_i386

OBJS=\
src/entry.o \
src/main.o \
src/tty.o \
src/gdt.o \
src/idt.o \
src/misc.o \
src/common.o \
src/ints.o \
src/isr.o \
src/memory.o \
src/multiboot.o

YeetOS: $(OBJS) link.ld
	$(LD) $(LD_FLAGS) -T link.ld -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) YeetOS

%.o: %.asm
	$(ASM) $(ASM_FLAGS) $< -o $@

%.o: %.c
	$(CC) -MMD -c $(CC_FLAGS) $< -o $@

-include $(OBJS:.o=.d)