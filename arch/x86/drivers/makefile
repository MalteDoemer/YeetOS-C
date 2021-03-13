
OBJECTS=\
vga_text.o \
serial.o \

drivers.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f drivers.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)