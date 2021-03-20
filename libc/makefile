
OBJECTS=\
memcpy.o \
memset.o \
memcmp.o \
vsnprintf.o \
printf.o \
isdigit.o \
isalpha.o \

libc.a: $(OBJECTS)
	$(AR) -rcs $(LD_FLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f libc.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)