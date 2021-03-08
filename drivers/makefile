
OBJECTS=\
vga_text.o \

drivers.a: $(OBJECTS)
	$(AR) -rcs $(LD_FLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CC) $(CPP_FLAGS) -MMD -c -o $@ $<

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f drivers.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)