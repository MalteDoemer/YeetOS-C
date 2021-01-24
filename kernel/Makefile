CC = i686-elf-gcc
LD = i686-elf-ld
ASM = nasm -f elf32

ASM_FLAGS=-g

CC_FLAGS=\
-ggdb \
-ffreestanding \
-fno-PIE \
-nostdlib \
-std=c99 \
-I include
# -Wno-packed-bitfield-compat

LD_FLAGS= -m elf_i386

OBJS=\
entry.o \
main.o \
tty.o \
gdt.o \
idt.o \
misc.o \
common.o \
ints.o

YeetOS: $(OBJS) link.ld
	$(LD) $(LD_FLAGS) -T link.ld -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) YeetOS

%.o: %.asm
	$(ASM) $(ASM_FLAGS) $< -o $@

%.o: %.c
	$(CC) -MMD -c $(CC_FLAGS) $< -o $@

-include $(OBJS:.o=.d)