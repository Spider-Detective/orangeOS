;; lib.inc
;; library functions for use

; show up a number in AL
DispAL:
        push    ecx
        push    edx
        push    edi

        mov     edi, [dwDispPos]

        mov     ah, 0Fh        ; 0000b: black background, 1111b: red char
        mov     dl, al
        shr     al, 4
        mov     ecx, 2
.begin:
        and     al, 01111b
        cmp     al, 9
        ja      .1             ; convert number to hex in string: if larger than 9, change to ABCDEF
        add     al, '0'        ; convert digit to char
        jmp     .2
.1:
        sub     al, 0Ah
        add     al, 'A'        ; convert number to hex in string
.2:     
        mov     [gs:edi], ax   ; show the string char, edi is the offset
        add     edi, 2

        mov     al, dl
        loop    .begin

        mov     [dwDispPos], edi
        
        pop     edi
        pop     edx
        pop     ecx

        ret
; end of DispAL

;; show up a int
DispInt:
        mov     eax, [esp + 4]   ; [esp + 4]: the argument pushed into stack
        shr     eax, 24
        call    DispAL

        mov     eax, [esp + 4]
        shr     eax, 16
        call    DispAL

        mov     eax, [esp + 4]
        shr     eax, 8
        call    DispAL

        mov     eax, [esp + 4]
        call    DispAL

        mov     ah, 07h         ; black background, grey char
        mov     al, 'h'         ; show up 'h' at the end of each int
        push    edi
        mov     edi, [dwDispPos]
        mov     [gs:edi], ax
        add     edi, 4
        mov     [dwDispPos], edi
        pop     edi

        ret
; end of DispInt

;; show up a string
DispStr:
        push    ebp
        mov     ebp, esp
        push    ebx
        push    esi
        push    edi

        mov     esi, [ebp + 8]   
        mov     edi, [dwDispPos]
        mov     ah, 0Fh
.1:
        lodsb
        test    al, al            ; if encounter \0, stop
        jz      .2                
        cmp     al, 0Ah           ; check if the current char is \n
        jnz     .3                ; jump if (al != 0Ah), and show the char
        push    eax               ; if it is \n, update edi (current display position) to next line
        mov     eax, edi
        mov     bl, 160           ; 160 is the length of the line
        div     bl                ; eax / 160, save result in eax
        and     eax, 0FFh
        inc     eax
        mov     bl, 160
        mul     bl
        mov     edi, eax
        pop     eax
        jmp     .1
.3:
        mov     [gs:edi], ax
        add     edi, 2
        jmp     .1
.2: 
        mov     [dwDispPos], edi

        pop     edi
        pop     esi
        pop     ebx
        pop     ebp
        ret
; end of DispStr

;; show return
DispReturn:
        push    szReturn
        call    DispStr            ; printf("\n");
        add     esp, 4

        ret
; end of DispReturn
