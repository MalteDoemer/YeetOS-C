
OBJECTS=\
errno.o \
memset.o \
memcmp.o \
memcpy.o \
strlen.o \
strcmp.o \
strdup.o \
vsnprintf.o \
isdigit.o \
isalpha.o \
printf.o \

libc.a: $(OBJECTS)
	$(AR) -rcs -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(C_FLAGS) -MMD -c -o $@ $<

clean:
	rm -f libc.a $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)