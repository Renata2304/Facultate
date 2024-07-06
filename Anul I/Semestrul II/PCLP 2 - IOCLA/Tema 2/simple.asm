%include "../include/io.mac"

section .text
    global simple
    extern printf
simple:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     ecx, [ebp + 8]  ; len
    mov     esi, [ebp + 12] ; plain
    mov     edi, [ebp + 16] ; enc_string
    mov     edx, [ebp + 20] ; step

    ;; DO NOT MODIFY
    ;; Your code starts here
    ; imi fac un index pe care il folosesc pentru a parcurge cei doi vectori 
    xor ebx, ebx
crypt:
    ; copiez caracter cu caracter din esi in edi
    mov byte al, [esi + ebx]
    mov byte [edi + ebx], al
    ; criptez fiecare caracter 
    add dword [edi + ebx], edx
    ; in cazul in care sare de litera Z
    cmp byte [edi + ebx], 'Z'
    jle cont ; cazul in care nu a sarit de litera Z
    ; scad 26 din caracter pentru a relua alfabetul, in cazul in care se sare de Z
    sub byte [edi + ebx], 26
cont:
    ; incrementez ebx 
    inc ebx
    ; daca ebx nu trece de ecx, reiau loop ul
    cmp ebx, ecx
    jl crypt

    ;PRINTF32 `%s\n\x0`, edi

    ;; Your code ends here
    
    ;; DO NOT MODIFY

    popa
    leave
    ret
    
    ;; DO NOT MODIFY
