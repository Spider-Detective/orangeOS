[SECTION .text]

global  memcpy

; copy the memory, similar to memcpy in C
; In C, void *MemCpy(void* es:pDest, void* ds:pSrc, int iSize);
memcpy:
        push    ebp
        mov     ebp, esp

        push    esi
        push    edi
        push    ecx

        mov     edi, [ebp + 8]  ; destination, pDest
        mov     esi, [ebp + 12] ; source, pSrc
        mov     ecx, [ebp + 16] ; counter, iSize
.1:
        cmp     ecx, 0
        jz      .2              ; exit loop when countdown to 0

        mov     al, [ds:esi]    ; get the source value
        inc     esi

        mov     byte [es:edi], al ; put into dest
        inc     edi             ; move byte by byte

        dec     ecx
        jmp     .1
.2: 
        mov     eax, [ebp + 8]  ; return value

        pop     ecx
        pop     edi
        pop     esi
        mov     esp, ebp
        pop     ebp

        ret
; end of memcpy