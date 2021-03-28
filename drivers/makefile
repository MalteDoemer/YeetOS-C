
ifeq ($(ARCH), x86)
OBJECTS=\
serial.o \
vga_text.o \
ata.o \
ps2_keyboard.o \


else

endif


drivers.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f drivers.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)