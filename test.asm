

mov ecx, 36
mov edi, 0xdeadbeef
mov esi, 0xbeefdead

rep movsd


mov ecx, 36
mov edi, 0xdeadbeef
mov esi, 0xbeefdead

start:
    mov eax, [esi]
    mov [edi], eax
    add esi, 4
    add edi, 4
    dec ecx
    jnz start