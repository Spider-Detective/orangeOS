[SECTION .text]

global  memcpy
global  memset
global  strcpy
global  strlen

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

; void memset(void* p_dst, char ch, int size);
memset:
        push    ebp
        mov     ebp, esp

        push    esi
        push    edi
        push    ecx

        mov     edi, [ebp + 8]     ; p_dst
        mov     edx, [ebp + 12]    ; ch
        mov     ecx, [ebp + 16]    ; size
.1:  
        cmp     ecx, 0
        jz      .2

        ; copy the byte in dl
        mov     byte [edi], dl
        inc     edi

        dec     ecx
        jmp     .1
.2:
        pop     ecx
        pop     edi
        pop     esi
        mov     esp, ebp
        pop     ebp

        ret
; end of memset

; char* strcpy(char* p_dst, char* p_src);
strcpy:
        push    ebp
        mov     ebp, esp

        mov     esi, [ebp + 12]    ; p_src
        mov     edi, [ebp + 8]     ; p_dst
.1: 
        mov     al, [esi]
        inc     esi
        mov     byte [edi], al
        inc     edi

        cmp     al, 0
        jnz     .1

        mov     eax, [ebp + 8]      ; return value
        pop     ebp
        ret
; end of strcpy

; int strlen(char* p_str);
strlen:
        push    ebp
        mov     ebp, esp

        mov     eax, 0              ; start from 0 
        mov     esi, [ebp + 8]      
.1:
        cmp     byte [esi], 0       ; check if current char is '\0'
        jz      .2
        inc     esi
        inc     eax
        jmp     .1
.2:
        pop     ebp
        ret