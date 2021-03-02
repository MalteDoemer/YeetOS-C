bits 32 

KERNEL_BASE equ 0xC0000000


[global load_gdt]
load_gdt:
    mov eax, [esp + 4]
    lgdt [eax]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    jmp 0x08:.next
.next:
    ret


[global load_idt]
load_idt:
    mov eax, [esp + 4]
    lidt [eax]

    ret

[global load_tss]
load_tss:
    mov ax, [esp + 4]
    ltr ax
    ret

    

[global clear_screen]
[extern vga_color]
[extern vram]
clear_screen:
    mov ah, byte [vga_color]
    mov al, ' '
    shl eax, 16
    mov ah, byte [vga_color]
    mov al, ' '
    mov ecx, (80*25) / 2
    mov edi, dword [vram]
    rep stosd
    ret
