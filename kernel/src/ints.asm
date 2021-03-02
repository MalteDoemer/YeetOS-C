%macro ISR 1
[global isr%1]
isr%1:
    push dword 0
    push dword %1
    jmp near isr_common
%endmacro

%macro ISR_ERR 1
[global isr%1]
isr%1:
    nop
    nop
    push %1
    jmp near isr_common
%endmacro

extern isr_handler
isr_common:
    pusha
    mov eax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax

    push esp    ; pointer to regs_t structure on the stack

    call isr_handler

    add esp, 4

    pop ebx
    mov ds, bx
    mov es, bx
    mov gs, bx
    mov fs, bx

    popa

    add esp, 8

    iret


align 32

[global isr_stub_table]
isr_stub_table:

ISR 0       ; Divide By Zero
ISR 1       ; Debug
ISR 2       ; NMI
ISR 3       ; Breakpoint
ISR 4       ; Overflow
ISR 5       ; Bound Range Exceeded
ISR 6       ; Invalid Opcode
ISR 7       ; Device Not Available
ISR_ERR 8   ; Double Fault
ISR 9       ; Coprocessor Segment Overrun
ISR_ERR 10  ; Invalid TSS
ISR_ERR 11  ; Segment Not Present
ISR_ERR 12  ; Stack-Segment Fault
ISR_ERR 13  ; General Protection Fault
ISR_ERR 14  ; Page Fault
ISR 15      ; Reserved
ISR 16      ; x87 Floating-Point Exception
ISR_ERR 17  ; Alignment Check
ISR 18      ; Machine Check
ISR 19      ; SIMD Floating-Point Exception	
ISR 20      ; Virtualization Exception
ISR 21      ; Reserved
ISR 22      ; Reserved
ISR 23      ; Reserved
ISR 24      ; Reserved
ISR 25      ; Reserved
ISR 26      ; Reserved
ISR 27      ; Reserved
ISR 28      ; Reserved
ISR 29      ; Reserved
ISR 30      ; Security Exception
ISR 31      ; Reserved

%assign i 32

%rep 224
ISR i
%assign i (i+1)
%endrep
