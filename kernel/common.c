#include "kernel.h"

void* memset(void* d, int c, size_t n)
{
    for (uint8_t* dest = (uint8_t*)d; n--; dest++) {
        *dest = (uint8_t)c;
    }
}

void* memcpy(void* dest, void* src, size_t n)
{
}

size_t strlen(char* str)
{
}

int strcmp(char* str1, char* str2)
{
}
