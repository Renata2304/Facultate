%include "../include/io.mac"

section .text
    global is_square
    extern printf

is_square:
    ;; DO NOT MODIFY
    push ebp
    mov ebp, esp
    pusha

    mov ebx, [ebp + 8]      ; dist
    mov eax, [ebp + 12]     ; nr
    mov ecx, [ebp + 16]     ; sq
    ;; DO NOT MODIFY
   
    ;; Your code starts here

    ; folosesc edx pentru a forma patratele perfecte pe care le compar cu distantele 
    xor edx, edx ; golesc edx
    xor edi, edi ; golesc edi
    ; folosesc edi pentru a itera prin vectorul de distante
    mov edi, eax 
    dec edi ; decrementex esi pentru a merge de la 0 la eax-1
start :
    ; tratez separat cazul in care distanta este 0
    cmp word [ebx + 4 * edi], 0
    je zero
    ; construiesc la urmatorul patrat perfect 
    inc edx 
    mov eax, edx
    push edx ; mut edx pe stiva pentru a nu il strica
    mul eax ; fac eax*eax, formand un patrat perfect 
    pop edx ; scot edx de pe stiva, dupa ce am facut inmultirea

    cmp eax, [ebx + 4 * edi] ; compar patratul perfect obtinut cu distanta
    je patrat_perfect
    jg nu_patrat_perfect
    jl start ; daca patratul perfect obtinut in eax e mai mic decat distanta, se reia
zero: ; eticheta pentru cazul in care distanta este 0
    mov dword [ecx + 4 * edi], 1 ; modificarea vectorului de patrate perfecte
    jmp end
patrat_perfect: ; eticheta pentru cazul in care distanta este patrat perfect
    mov dword [ecx + 4 * edi], 1 ; modificarea vectorului de patrate perfecte
    jmp end
nu_patrat_perfect: ; eticheta pentru cazul in care distanta nu e patrat perfect
    mov dword [ecx + 4 * edi], 0 ; modificarea vectorului de patrate perfecte
    jmp end
end :
    dec edi ; trec la urmatoarea distanta
    xor edx, edx ; zerorizez edx
    cmp edi, 0
    jge start 

    ;pop eax
    ;; Your code ends here

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY