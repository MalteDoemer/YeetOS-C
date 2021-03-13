
OBJECTS=\
main.o \
panic.o \
kheap.o \
debug.o \

kernel.a: $(OBJECTS)
	$(AR) -rcs $(LD_FLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f kernel.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)