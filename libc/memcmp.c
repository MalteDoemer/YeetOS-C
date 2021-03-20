#include "stdint.h"
#include "stddef.h"

int memcmp(const void* ptr1, const void* ptr2, size_t n)
{
    const uint8_t* p1 = (uint8_t*)ptr1;
    const uint8_t* p2 = (uint8_t*)ptr2;

    while (n--) {
        if (*p1 != *p2)
            return *p1 - *p2;

        p1++;
        p2++;
    }

    return 0;
}