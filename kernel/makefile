
OBJECTS=\
main.o \
multiboot.o \
tty.o \

kernel.a: $(OBJECTS)
	$(AR) -rcs $(LD_FLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CC) $(CPP_FLAGS) -MMD -c -o $@ $<

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f kernel.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)