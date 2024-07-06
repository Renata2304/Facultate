%include "../include/io.mac"

struc point
    .x: resw 1
    .y: resw 1
endstruc

section .text
    global points_distance
    extern printf

points_distance:
    ;; DO NOT MODIFY
    
    push ebp
    mov ebp, esp
    pusha

    mov ebx, [ebp + 8]      ; points
    mov eax, [ebp + 12]     ; distance
    ;; DO NOT MODIFY
   
    ;; Your code starts here
    
    ; copiez in dx si cx, coordonatele x ale punctelor date
    xor edx, edx
    mov dx, word [ebx + point.x]
    ;mov cx, word [ebx + 4*1 + point.x]
    cmp dx, [ebx + 4 + point.x]
    je label_y  ; in cazul in care dreapta formata de puncte este paralela cu Oy
    jmp label_x  ; cazul in care dreapta formata de puncte este paralela cu Ox

label_x:
    cmp dx, [ebx + 4 + point.x]
    jge caz_d_x  ; cazul in care punctul din dx este mai la dreapta punctului din cx
    jmp caz_c_x  ; in caz contrar se va sterge din cx, dx

label_y:
    ; sterg datele din dx si cx si retin coordonatele y ale punctelor date 
    xor edx, edx
    mov dx, word [ebx + point.y]
    cmp dx, [ebx + 4 + point.y]
    jge caz_d_y  ; in cazul in care punctul retinut in dx este mai sus decat cel din cx
    jmp caz_c_y  ; in caz contrar, se sterge din cx, dx

caz_d_x: 
    ; cazul in care se sterge din dx, cx
    sub dx, [ebx + 4 + point.x]
    mov [eax], edx
    jmp end
caz_c_x:
    ; cazul in care se sterge din cx, dx
    xor edx, edx
    mov dx, [ebx + 4 + point.x]
    sub dx, [ebx + point.x]
    mov [eax], edx
    jmp end

caz_d_y: 
    ; cazul in care se sterge din dx, cx
    sub dx, [ebx + 4 + point.y]
    mov [eax], edx
    jmp end
caz_c_y:
    ; cazul in care se sterge din cx, dx
    xor edx, edx
    mov dx, [ebx + 4 + point.y]
    sub dx, [ebx + point.y]
    mov [eax], edx
    jmp end
end:   
    ;; Your code ends here
    
    ;; DO NOT MODIFY
    popa
    leave
    ret

    ;; DO NOT MODIFY