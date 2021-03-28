
#ifdef __KERNEL__

#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "kernel/kheap.h"

char* strdup(const char* str)
{
    size_t len = strlen(str);
    char* newstr = kmalloc(len + 1);
    memcpy(newstr, str, (len + 1) * sizeof(char));
    return newstr;
}

#endif
