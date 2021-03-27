#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"

#include "kernel/kernel.h"
#include "kernel/kheap.h"
#include "kernel/panic.h"

void color_test()
{
    printf("\033[30mA \033[31m B \033[32m C \033[33m D \033[0m \033[34m E \033[35m F \033[36m G \033[37m H \033[0m\n");
}

void heap_test()
{
    int* chars = kmalloc(1024);
    memcpy(chars, "+--------------------+\n| Welcome to YeetOS! |\n+--------------------+", 69);
    printf("%s\n\n", chars);
    kfree(chars);
}

void call_ctors()
{
    typedef int (*ctor_t)();

    extern ctor_t ctors_start;
    extern ctor_t ctors_end;

    if (&ctors_start == &ctors_end)
        return;

    ctor_t* ctor = &ctors_end;
    int ret;

    do {
        ctor--;
        ret = (*ctor)();
        if (ret != SUCCESS) {
            WARN("%s", "constructor failed");
        }

    } while (ctor > &ctors_start);
}

void kernel_main()
{
    init_kheap();
    init_arch();
    call_ctors();

    printf("\033[?25l");
    printf("\033[J");
    printf("\033[H");

    heap_test();
    color_test();
}