[SECTION .data]
disp_pos        dd     0

[SECTION .text]

global  disp_str

;; show up a string, void disp_str(char * info);
disp_str:
        push    ebp
        mov     ebp, esp

        mov     esi, [ebp + 8]    ; pszInfo, see start.c
        mov     edi, [disp_pos]
        mov     ah, 0Fh
.1:
        lodsb
        test    al, al            ; if encounter \0, stop
        jz      .2                
        cmp     al, 0Ah           ; check if the current char is \n
        jnz     .3                ; jump if (al != 0Ah), and show the char
        push    eax               ; if it is \n, update edi (current display position) to next line
        push    ebx               ; NOTICE: in protect mode, need to protect 
                                  ; ebx when using bl below, still don't know why
        mov     eax, edi
        mov     bl, 160           ; 160 is the length of the line
        div     bl                ; eax / 160, save result in eax
        and     eax, 0FFh
        inc     eax
        mov     bl, 160
        mul     bl
        mov     edi, eax
        pop     ebx 
        pop     eax
        jmp     .1
.3:
        mov     [gs:edi], ax
        add     edi, 2
        jmp     .1
.2: 
        mov     [disp_pos], edi

        pop     ebp
        ret
; end of DispStr