#include "kernel.h"

static mboot_info_t* mboot_info;

mboot_info_t* get_mboot_info()
{
    return mboot_info;
}

void set_mboot_info(mboot_info_t* mboot)
{
    mboot_info = mboot;
}