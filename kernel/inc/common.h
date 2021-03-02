#ifndef COMMON_H
#define COMMON_H

#include "stdint.h"
#include "stddef.h"

#define SET_BIT(A,B) (A = (A | (1 << (B))))
#define CLEAR_BIT(A,B) (A = (A & ~(1 << (B))))
#define TOGGLE_BIT(A,B) (A = (A ^ (1 << (B))))
#define TEST_BIT(A,B) (A & (1 << (B)))

static inline void outb(uint16_t port, uint8_t data)
{
    __asm volatile("outb %1, %0" ::"dN"(port), "a"(data));
}

static inline void outw(uint16_t port, uint16_t data)
{
    __asm volatile("outw %1, %0" ::"dN"(port), "a"(data));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm volatile("inw %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

static inline void cli()
{
    __asm("cli");
}

static inline void sti()
{
    __asm("sti");
}

static inline void hlt()
{
    __asm("hlt");
}

void* memset(void* d, int c, size_t n);

void* memcpy(void* dest, void* src, size_t n);

size_t strlen(char* str);

int strcmp(char* str1, char* str2);

#endif // #ifndef COMMON_H
