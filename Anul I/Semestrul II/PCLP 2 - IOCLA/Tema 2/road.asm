%include "../include/io.mac"

struc point
    .x: resw 1
    .y: resw 1
endstruc

section .text
    global road
    extern printf

road:
    ;; DO NOT MODIFY
    push ebp
    mov ebp, esp
    pusha

    mov eax, [ebp + 8]      ; points
    mov ecx, [ebp + 12]     ; len
    mov ebx, [ebp + 16]     ; distances
    ;; DO NOT MODIFY
   
    ;; Your code starts here
    ; folosesc edi pentru a itera prin vectorul de puncte
    mov edi, 0
start:
    ; salvez din vector coord. x al punctului curent
    xor edx, edx
    mov dx, word [eax + 4*edi + point.x]
    cmp dx, [eax + 4 + 4*edi + point.x]
    je caz_y
    jmp caz_x
caz_x:
    ; cazul in care punctele formeaza o dreapta paralela cu Ox
    cmp dx, [eax + 4 + 4*edi + point.x]
    jge caz_1
    jmp caz_2

caz_1:
    ;cazul in care se sterge din coord. x a primului punct, coord. x a celui de-al doilea punct
    sub dx, [eax + 4 + 4*edi + point.x]
    mov  [ebx + 4*edi], edx ; salvez in vectorul de rezultate
    jmp end

caz_2:
    ;cazul in care se sterge din coord. x a celui de-al doilea punct, coord. x a primului punct
    xor edx, edx ; interschimb datele din vector
    mov dx, [eax + 4 + 4*edi + point.x]
    sub dx, [eax + 4*edi + point.x]
    mov  [ebx + 4*edi], edx ; salvez in vectorul de rezultate
    jmp end

caz_y:
    ; cazul in care punctele formeaza o dreapta paralela cu Oy
    xor edx, edx ; sterg datele din edx si salvez coord. y a punctului curent
    mov dx, word [eax + 4*edi + point.y]
    cmp dx, [eax + 4 + 4*edi + point.y]
    jge caz_3
    jmp  caz_4
caz_3:
    ;cazul in care se sterge din coord. y a primului punct, coord. y a celui de-al doilea punct
    sub dx, [eax + 4 + 4*edi + point.y]
    mov [ebx + 4*edi], edx ; salvez in vectorul de rezultate
    jmp end
caz_4:
    ;cazul in care se sterge din coord. y a celui de-al doilea punct, coord. y a primului punct
    xor edx, edx ; interschimb datele intre ele 
    mov dx, word [eax + 4 + 4*edi + point.y]
    sub dx, [eax + 4*edi + point.y]
    mov [ebx + 4*edi], edx ; salvez in vectorul de rezultate
    jmp end

end:
    inc edi
    cmp edi, ecx
    jl start ; daca inca mai sunt puncte, se reia algoritmul
    ;; Your code ends here
    
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY