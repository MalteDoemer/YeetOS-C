SECTIONS {
    . = 0x100000;

    .mboot : {
        *(.mboot)
    }

    . += 0xFF800000;

    .text : AT(ADDR(.text) - 0xFF800000) {
       code = . - 0xFF800000;
       *(.text)
       *(.rodata*)
    }

    .data ALIGN (4K) : AT(ADDR(.data) - 0xFF800000) {
       *(.data)
    }

    .bss ALIGN (4K) : AT(ADDR(.bss) - 0xFF800000) {
       bss = . - 0xFF800000;
       *(COMMON)
       *(.bss)
       end = . - 0xFF800000;
    }


    __page_structs_start = 0xFFC00000;
    __page_structs_end = 0x100000000;
}