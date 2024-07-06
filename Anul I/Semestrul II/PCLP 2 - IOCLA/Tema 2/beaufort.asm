%include "../include/io.mac"

section .text
    global beaufort
    extern printf

; void beaufort(int len_plain, char *plain, int len_key,
;               char *key, char tabula_recta[26][26], char *enc);
beaufort:
    ;; DO NOT MODIFY
    push ebp
    mov ebp, esp
    pusha

    mov eax, [ebp + 8]  ; len_plain
    mov ebx, [ebp + 12] ; plain (address of first element in string)
    mov ecx, [ebp + 16] ; len_key
    mov edx, [ebp + 20] ; key (address of first element in matrix)
    mov edi, [ebp + 24] ; tabula_recta
    mov esi, [ebp + 28] ; enc
    ;; DO NOT MODIFY
    ;; TODO: Implement spiral encryption
    ;; FREESTYLE STARTS HERE
    
    ; In aceasta rezolvare voi folosi calculul algebric pentru a rezolva cerinta,
    ; asa ca voi rezoriza edi pentru a-mi fi mai usor pentru iteratie
    xor edi, edi ; iteratorul prin plain
    xor esi, esi ; iteratorul prin key
start: 
    ;PRINTF32 `%d %d\n\x0`, edi, esi

    push eax ; pun eax pe stiva
    xor eax, eax ; zerorizez eax
    mov byte al, [edx + esi] ; pun in al litera curenta din edx (key)
    sub al, [ebx + edi] ; scad din litera din key, cea din plain
    cmp byte al, 0 ; compar litera schimbata cu A pentru a vedea
                            ; nu a scazut sub 0
    jl sub_zero ; in acest caz, ii adaug 26
    jmp end
sub_zero:
    add byte al, 26
end: ; aici formez litera propriu-zisa, intrcat in al voi avea doar un numar
    add byte al, 'A'
    inc esi ; trec la urmatoarea litera din key
    cmp esi, ecx 
    je label_1 ; in cazul in care s-a ajuns la finalul lui key, se ia de pe pozitia 0
    jmp label_2 ; in caz contrar, se trece mai departe
label_1:
    mov esi, 0
    jmp label_2
label_2: ; aici 
    mov byte [ebx + edi], al ; mut la loc in ebx (plain) litera schimbata
    pop eax ; scot eax de pe stiva
    inc edi ; mares iteratorul din plain
    cmp edi, eax ; daca nu s-a ajuns la finalul lui plain,
    jl start     ; se reia algoritmul

    ; in caz contrar se salveaza in esi ce aveam in ebx si se termina programul
    mov ecx, 0
    xor esi, esi ; sterg ce avea inainte in esi
    mov esi, [ebp + 28] ; reinitializez esi
copy:
    push eax ; pun iar eax pe stiva
    mov byte al, [ebx + ecx] ; mut in al litera modificata din plain
    mov byte [esi + ecx], al ; mut in esi litera modificata din plain
    pop eax ; scot de pe stiva eax
    inc ecx
    cmp ecx, eax
    jl copy ; daca nu s-au copiat toate literele, se reia copy

    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
