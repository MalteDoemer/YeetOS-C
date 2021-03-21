#include "stdint.h"
#include "stddef.h"
#include "stdio.h"

#include "arch/arch.h"

#include "kernel/kernel.h"
#include "kernel/panic.h"

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
}