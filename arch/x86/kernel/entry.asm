bits 32

extern _code
extern _bss_start
extern _bss_end

extern pt_bitmap
extern page_structs
extern _kernel_size_a

global start

section .mboot

; Multiboot stuff
MBOOT_PAGE_ALIGN	equ 1
MBOOT_MEM_INFO		equ 2
MBOOT_HEADER_MAGIC	equ 0x1BADB002
MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM		equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

; The virtual address of the kernel
KERNEL_BASE equ 0xC0000000


; Multiboot header for grub
; It is important to use the physical address
mboot_header:
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM

    dd mboot_header
    dd _code
    dd _bss_start
    dd _bss_end
    dd start

; align 4*1024

; boot_page_dir:
;     dd 0x00000083
;     times ((KERNEL_BASE >> 22) - 1) dd 0
;     dd 0x00000083
;     dd 0x00400083
;     times (1024 - (KERNEL_BASE >> 22) - 2) dd 0

start:
    cli
    cld

    ; pt_bitmap is used to keep track of all paging structures
    mov dword [pt_bitmap - KERNEL_BASE], 0x1

    ; map the first 4 MiB to address 0x00
    mov dword [page_structs - KERNEL_BASE], 0x00000083



    ; map the kernel image into higher half
    mov ecx, _kernel_size_a
    shr ecx, 22

    mov edi, (page_structs - KERNEL_BASE) + ((KERNEL_BASE >> 22) * 4)
    mov edx, 0x00000083

.fill_pd:
    mov dword [edi], edx
    add edx, 0x400000
    add edi, 4
    loop .fill_pd


    ; tell the MMU where to find the page directory
    mov ecx, page_structs - KERNEL_BASE
    mov cr3, ecx

    ; set PSE bit in CR4 to enable 4MiB pages.
    mov ecx, cr4
    or ecx, 0x00000010
    mov cr4, ecx


    ; set PG bit in CR0 to enable paging
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    ; jump into higher half Yey
    jmp up

section .text

extern multiboot_ptr
extern multiboot_sig
extern kernel_main

up:
    ; delete the identety  mapped entry
    mov dword [page_structs], 0
    invlpg [0]

    mov esp, kernel_stack_top
    mov ebp, esp

    ; correct the pointer to mboot structure
    add ebx, KERNEL_BASE

    ; store the pointer to mboot structure
    mov dword [multiboot_ptr], ebx

    ; store the multiboot signature
    mov dword [multiboot_sig], eax

    call kernel_main

.halt:
    hlt
    jmp .halt


section .bss


[global kernel_stack]
[global kernel_stack_top]

kernel_stack:
resb 1024 * 4
kernel_stack_top: