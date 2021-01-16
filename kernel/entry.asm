bits 32

extern main
extern code
extern bss
extern end

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
    dd code
    dd bss
    dd end
    dd start

align 4*1024

; the boot page directory contains two entries:
;   1: this identity maps the first 4MiB 
;   2: this maps 4MiB from 0xC0000000 to 0x00
boot_page_dir:
    dd 0x00000083
    times ((KERNEL_BASE >> 22) - 1) dd 0
    dd 0x00000083
    times (1024 - (KERNEL_BASE >> 22) - 1) dd 0


start:
    cli
    cld

    ; tell the MMU where to find the page directory
    mov ecx, boot_page_dir
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

up:
    ; delete the identety  mapped entry
    mov dword [boot_page_dir + KERNEL_BASE], 0
    invlpg [0]

    mov esp, kernel_stack.top
    mov ebp, esp

    add ebx, KERNEL_BASE
    push ebx
    call main

.halt:
    hlt
    jmp .halt


global clear_screen
clear_screen:
    mov edi, 0xB8000 + KERNEL_BASE
    mov eax, 0x1F201F20
    mov ecx, (80*25)/2
    rep stosd
    ret


section .bss

kernel_stack:
resb 1024 * 4
.top: