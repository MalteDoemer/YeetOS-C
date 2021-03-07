
OBJECTS=\
entry.o \
init.o \

arch.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CPP_FLAGS) -MMD -c -o $@ $<

%.o: %.asm
	$(AS) $(AS_FLAGS) -o $@ $<

clean:
	rm -f arch.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)