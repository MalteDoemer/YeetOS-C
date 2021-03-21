#include "stdint.h"
#include "stddef.h"
#include "stdio.h"

#include "arch/arch.h"

#include "kernel/kernel.h"
#include "kernel/panic.h"

void color_test()
{
    printf("\033[30mA \033[31m B \033[32m C \033[33m D \033[0m \033[34m E \033[35m F \033[36m G \033[37m H \033[0m\n");
}

void call_constructors()
{
    typedef rcode_t (*ctor_t)();

    extern ctor_t ctors_start;
    extern ctor_t ctors_end;

    if (&ctors_start == &ctors_end)
        return;

    ctor_t* ctor = &ctors_end;
    rcode_t ret;

    do {
        ctor--;
        ret = (*ctor)();
        if (ret != RCODE_SUCESS) {
            WARN("constructor failed with rcode %d", ret);
        }

    } while (ctor > &ctors_start);
}

void kernel_main()
{
    init_arch();

    call_constructors();

    printf("\033[?25l");
    printf("\033[J");
    printf("\033[H");

    color_test();
}